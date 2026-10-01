// minitest.h - micro-framework de testes sem dependencias externas.
//
// Uso:
//   #include "minitest.h"
//   TEST(NomeSuite, NomeCaso) {
//       EXPECT_EQ(a, b);
//       EXPECT_TRUE(cond);
//   }
//   MINITEST_MAIN()  // gera o main() que roda tudo e retorna falhas
//
// Macros disponiveis: EXPECT_EQ, EXPECT_NE, EXPECT_TRUE, EXPECT_FALSE,
// EXPECT_STREQ (const char*/std::string), EXPECT_NEAR (float, epsilon).
// Cada falha imprime arquivo:linha, valores e o nome do teste. O resumo
// final sai no formato "[  PASSED  ] N tests" (estilo gtest, sem gtest).
#pragma once

#include <cmath>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace minitest {

// Um caso de teste registrado automaticamente na construcao estatica.
struct TestCase {
    std::string suite;
    std::string name;
    std::function<void()> func;
};

// Registro global (vive ate o fim do main).
inline std::vector<TestCase>& Registry() {
    static std::vector<TestCase> reg;
    return reg;
}

// Contadores do teste em execucao (thread-unsafe de proposito: testes sao seriais).
inline int& FailCount() {
    static int n = 0;
    return n;
}
inline std::string& CurrentTest() {
    static std::string t;
    return t;
}

inline void Check(bool ok, const char* file, int line, const std::string& msg) {
    if (!ok) {
        ++FailCount();
        std::cout << file << ":" << line << ": FALHA em " << CurrentTest()
                  << "\n    " << msg << "\n";
    }
}

// Converte qualquer valor comparavel para string (para mensagens de erro).
template <typename T>
std::string ToStr(const T& v) {
    std::ostringstream s;
    s << v;
    return s.str();
}

// Roda todos os testes e retorna o numero de falhas (0 = ok).
inline int RunAll() {
    int failedTests = 0;
    int totalTests = 0;
    for (auto& t : Registry()) {
        CurrentTest() = t.suite + "." + t.name;
        FailCount() = 0;
        ++totalTests;
        try {
            t.func();
        } catch (const std::exception& e) {
            ++FailCount();
            std::cout << "EXCECAO em " << CurrentTest() << ": " << e.what() << "\n";
        } catch (...) {
            ++FailCount();
            std::cout << "EXCECAO desconhecida em " << CurrentTest() << "\n";
        }
        if (FailCount() == 0) {
            std::cout << "[       OK ] " << CurrentTest() << "\n";
        } else {
            std::cout << "[  FALHOU ] " << CurrentTest() << "\n";
            ++failedTests;
        }
    }
    std::cout << "[==========] " << totalTests << " testes, " << failedTests
              << " com falha\n";
    if (failedTests == 0) {
        std::cout << "[  PASSED  ] todos os testes\n";
    }
    return failedTests;
}

}  // namespace minitest

// Registra o caso: cria um objeto estatico que se auto-inscreve.
#define TEST(suite, name)                                                      \
    static void minitest_body_##suite##_##name();                              \
    static const bool minitest_reg_##suite##_##name = [] {                      \
        ::minitest::Registry().push_back(                                      \
            {#suite, #name, &minitest_body_##suite##_##name});                  \
        return true;                                                           \
    }();                                                                       \
    static void minitest_body_##suite##_##name()

#define EXPECT_TRUE(cond)                                                      \
    ::minitest::Check((cond), __FILE__, __LINE__, "esperava verdadeiro: " #cond)

#define EXPECT_FALSE(cond)                                                     \
    ::minitest::Check(!(cond), __FILE__, __LINE__, "esperava falso: " #cond)

#define EXPECT_EQ(a, b)                                                        \
    ::minitest::Check(((a) == (b)), __FILE__, __LINE__,                        \
                      "esperava " #a " == " #b " (" +                           \
                          ::minitest::ToStr(a) + " vs " +                      \
                          ::minitest::ToStr(b) + ")")

#define EXPECT_NE(a, b)                                                        \
    ::minitest::Check(((a) != (b)), __FILE__, __LINE__,                        \
                      "esperava " #a " != " #b)

#define EXPECT_STREQ(a, b)                                                     \
    ::minitest::Check((std::string(a) == std::string(b)), __FILE__, __LINE__,  \
                      std::string("esperava \"") + (a) + "\" == \"" + (b) + "\"")

#define EXPECT_NEAR(a, b, eps)                                                 \
    ::minitest::Check(std::fabs(double(a) - double(b)) <= double(eps),         \
                      __FILE__, __LINE__,                                      \
                      "esperava " #a " ~= " #b " (+/-" #eps ")")

// Gera o main() padrao de cada executavel de teste.
#define MINITEST_MAIN()                                                        \
    int main() { return ::minitest::RunAll(); }
