// test_game.cpp - testes de saves e regras puras (sem janela/SDL/GL).
//
// Cobre: Game::CalculateScore (tabela de notas + buracos 29/49/69),
// Game::AddToRanking (ordem, top-10, arquivo ausente/cheio),
// Game::SetCoins/GetCoins (roundtrip), StoreState::VerifyLevel e
// MessageWindow::SplitLines. Tudo via arquivos num diretorio temporario
// (fixture troca o CWD: as funcoes usam paths relativos "arquivos/...").
// Testes usam main() proprio: SDL_MAIN_HANDLED evita o WinMain do SDL2main.
#define SDL_MAIN_HANDLED

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include "minitest.h"
#include "Game.h"
#include "StoreState.h"
#include "MessageWindow.h"

namespace fs = std::filesystem;

// Prepara CWD isolado com arquivos/save/ vazio. Idempotente.
static void UseFixture() {
    fs::path dir = fs::temp_directory_path() / "ripa_test_saves";
    fs::create_directories(dir / "arquivos" / "save");
    fs::current_path(dir);
}

// Apaga os saves para cada teste comecar do zero.
static void CleanSaves() {
    std::error_code ec;
    fs::remove("arquivos/save/coins.txt", ec);
    fs::remove("arquivos/save/highscores.txt", ec);
    fs::remove("arquivos/save/storehistory.txt", ec);
    fs::remove("arquivos/save/config.txt", ec);
}

// Le o arquivo inteiro como vetor de linhas (para conferir ordenacao).
static std::vector<std::string> ReadLines(const char* path) {
    std::vector<std::string> out;
    std::ifstream f(path);
    std::string line;
    while (std::getline(f, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();  // tolera CRLF (Windows)
        }
        out.push_back(line);
    }
    return out;
}

// --- Game::CalculateScore ---

TEST(Score, TabelaCompleta) {
    EXPECT_STREQ(Game::CalculateScore(0), "Nota: SR");
    EXPECT_STREQ(Game::CalculateScore(1), "Nota: II");
    EXPECT_STREQ(Game::CalculateScore(28), "Nota: II");
    EXPECT_STREQ(Game::CalculateScore(30), "Nota: MI");
    EXPECT_STREQ(Game::CalculateScore(48), "Nota: MI");
    EXPECT_STREQ(Game::CalculateScore(50), "Nota: MM");
    EXPECT_STREQ(Game::CalculateScore(68), "Nota: MM");
    EXPECT_STREQ(Game::CalculateScore(70), "Nota: MS");
    EXPECT_STREQ(Game::CalculateScore(88), "Nota: MS");
    EXPECT_STREQ(Game::CalculateScore(89), "Nota: SS");
    EXPECT_STREQ(Game::CalculateScore(100), "Nota: SS");
}

TEST(Score, SemBuracosNosLimites) {
    // Regressao: 29/49/69 caiam nos "vaos" das faixas e voltavam "".
    EXPECT_NE(Game::CalculateScore(29), "");
    EXPECT_NE(Game::CalculateScore(49), "");
    EXPECT_NE(Game::CalculateScore(69), "");
    EXPECT_STREQ(Game::CalculateScore(29), "Nota: II");
    EXPECT_STREQ(Game::CalculateScore(49), "Nota: MI");
    EXPECT_STREQ(Game::CalculateScore(69), "Nota: MM");
}

// --- Game::SetCoins/GetCoins ---

TEST(Coins, Roundtrip) {
    UseFixture();
    CleanSaves();
    Game::SetCoins(1234);
    EXPECT_EQ(Game::GetCoins(), 1234);
    Game::SetCoins(0);
    EXPECT_EQ(Game::GetCoins(), 0);
}

TEST(Coins, AusenteValeZero) {
    UseFixture();
    CleanSaves();
    // Sem arquivo: nao crasha e retorna 0.
    EXPECT_EQ(Game::GetCoins(), 0);
}

// --- Game::AddToRanking ---

TEST(Ranking, ArquivoAusenteCria) {
    UseFixture();
    CleanSaves();
    Game::AddToRanking(500, 10);
    auto lines = ReadLines("arquivos/save/highscores.txt");
    EXPECT_EQ(lines.size(), 1u);
    EXPECT_STREQ(lines[0], "500 10");
}

TEST(Ranking, OrdenaDesc) {
    UseFixture();
    CleanSaves();
    Game::AddToRanking(100, 1);
    Game::AddToRanking(300, 3);
    Game::AddToRanking(200, 2);
    auto lines = ReadLines("arquivos/save/highscores.txt");
    EXPECT_EQ(lines.size(), 3u);
    EXPECT_STREQ(lines[0], "300 3");
    EXPECT_STREQ(lines[1], "200 2");
    EXPECT_STREQ(lines[2], "100 1");
}

TEST(Ranking, EmpateEntraDepois) {
    UseFixture();
    CleanSaves();
    Game::AddToRanking(100, 1);
    Game::AddToRanking(100, 2);
    auto lines = ReadLines("arquivos/save/highscores.txt");
    EXPECT_EQ(lines.size(), 2u);
    EXPECT_STREQ(lines[0], "100 1");
    EXPECT_STREQ(lines[1], "100 2");
}

TEST(Ranking, Top10Mantem10SemEstouro) {
    UseFixture();
    CleanSaves();
    // Enche com 10..100 e tenta um 11o baixo: top-10 intacto, sem OOB.
    for (int i = 1; i <= 10; i++) {
        Game::AddToRanking(i * 100, i);
    }
    Game::AddToRanking(50, 0);
    auto lines = ReadLines("arquivos/save/highscores.txt");
    EXPECT_EQ(lines.size(), 10u);
    EXPECT_STREQ(lines[0], "1000 10");
    EXPECT_STREQ(lines[9], "100 1");
}

TEST(Ranking, NovoRecordeEntraNoTopo) {
    UseFixture();
    CleanSaves();
    for (int i = 1; i <= 10; i++) {
        Game::AddToRanking(i * 100, i);
    }
    Game::AddToRanking(9999, 99);
    auto lines = ReadLines("arquivos/save/highscores.txt");
    EXPECT_EQ(lines.size(), 10u);
    EXPECT_STREQ(lines[0], "9999 99");
    // O ultimo (100) caiu fora.
    EXPECT_STREQ(lines[9], "200 2");
}

// --- StoreState::VerifyLevel ---

TEST(Store, HistoricoAusenteNivelZero) {
    UseFixture();
    CleanSaves();
    EXPECT_EQ(StoreState::VerifyLevel(0), 0);
    EXPECT_EQ(StoreState::VerifyLevel(2), 0);
}

TEST(Store, UltimoNivelVence) {
    UseFixture();
    CleanSaves();
    {
        std::ofstream f("arquivos/save/storehistory.txt");
        f << "0 1\n1 2\n0 3\n";
    }
    EXPECT_EQ(StoreState::VerifyLevel(0), 3);
    EXPECT_EQ(StoreState::VerifyLevel(1), 2);
    EXPECT_EQ(StoreState::VerifyLevel(2), 0);
}

// --- MessageWindow::SplitLines ---

TEST(Split, CurtaVaiInteira) {
    auto v = MessageWindow::SplitLines("Oi");
    EXPECT_EQ(v.size(), 1u);
    EXPECT_STREQ(v[0], "Oi");
}

TEST(Split, Exatos23NaoQuebra) {
    std::string s(23, 'x');
    auto v = MessageWindow::SplitLines(s);
    EXPECT_EQ(v.size(), 1u);
    EXPECT_STREQ(v[0], s);
}

TEST(Split, QuebraNoEspaco) {
    auto v = MessageWindow::SplitLines("Sua pontuacao e seus itens serao zerados. Tem certeza?");
    EXPECT_TRUE(v.size() >= 2u);
    for (auto& line : v) {
        EXPECT_TRUE(line.size() <= 22u);
    }
    // Rejunta: nao perde palavra (junta com espaco e compara prefixo).
    std::string joined;
    for (auto& line : v) {
        if (!joined.empty()) {
            joined += " ";
        }
        joined += line;
    }
    EXPECT_STREQ(joined, "Sua pontuacao e seus itens serao zerados. Tem certeza?");
}

TEST(Split, PalavraLongaSemEspacoNaoTrava) {
    // Regressao: loop `while (max...)` sem piso ia a -1 e crashava.
    // Hoje corta (com perda) mas nunca sai do vetor nem trava.
    auto v = MessageWindow::SplitLines(std::string(30, 'z'));
    for (auto& line : v) {
        EXPECT_TRUE(line.size() <= 22u);
    }
}

TEST(Split, Vazia) {
    auto v = MessageWindow::SplitLines("");
    EXPECT_EQ(v.size(), 1u);
    EXPECT_STREQ(v[0], "");
}

MINITEST_MAIN()
