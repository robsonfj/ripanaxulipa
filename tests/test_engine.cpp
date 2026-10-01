// test_engine.cpp - testes das primitivas puras (Rect, Point, Timer).
//
// Nao precisa de SDL nem OpenGL: so matematica e estado local.
// Roda headless em qualquer SO.
// Testes usam main() proprio: SDL_MAIN_HANDLED evita o WinMain do SDL2main.
#define SDL_MAIN_HANDLED

#include <cstdio>

#include "minitest.h"
#include "Point.h"
#include "Rect.h"
#include "Timer.h"

// --- Point::GetDistance ---

TEST(Point, DistanciaOrigem) {
    // 3-4-5 classico.
    EXPECT_NEAR(Point(0, 0).GetDistance(Point(3, 4)), 5.0, 1e-4);
}

TEST(Point, DistanciaZero) {
    EXPECT_NEAR(Point(2, -7).GetDistance(Point(2, -7)), 0.0, 1e-6);
}

TEST(Point, DistanciaSimetrica) {
    // d(a,b) == d(b,a), inclusive com negativos.
    Point a(-1, -1), b(2, 3);
    EXPECT_NEAR(a.GetDistance(b), b.GetDistance(a), 1e-6);
    EXPECT_NEAR(a.GetDistance(b), 5.0, 1e-4);
}

// --- Rect::GetRectCenter ---

TEST(Rect, Centro) {
    Rect r(10, 20, 100, 60);
    Point c = r.GetRectCenter();
    EXPECT_NEAR(c.x, 60.0, 1e-6);
    EXPECT_NEAR(c.y, 50.0, 1e-6);
}

TEST(Rect, CentroPadraoZero) {
    // Rect() comeca em 0,0,0,0.
    Point c = Rect().GetRectCenter();
    EXPECT_NEAR(c.x, 0.0, 1e-6);
    EXPECT_NEAR(c.y, 0.0, 1e-6);
}

// --- Rect::IsInside (bordas inclusivas) ---

TEST(Rect, DentroFora) {
    Rect r(10, 20, 100, 60);
    EXPECT_TRUE(r.IsInside(50, 50));    // meio
    EXPECT_TRUE(r.IsInside(10, 20));    // canto sup-esq (borda vale)
    EXPECT_TRUE(r.IsInside(110, 80));   // canto inf-dir (borda vale)
    EXPECT_FALSE(r.IsInside(9.9f, 50)); // fora esq
    EXPECT_FALSE(r.IsInside(50, 81));    // fora baixo
}

TEST(Rect, RetZeroNaoConteudo) {
    // w=h=0: so o ponto exato passa.
    Rect r(5, 5, 0, 0);
    EXPECT_TRUE(r.IsInside(5, 5));
    EXPECT_FALSE(r.IsInside(5.1f, 5));
}

// --- Timer ---

TEST(Timer, ContagemERestart) {
    Timer t;
    EXPECT_NEAR(t.Get(), 0.0, 1e-9);
    t.Update(16.0);
    t.Update(17.0);
    EXPECT_NEAR(t.Get(), 33.0, 1e-9);
    t.Restart();
    EXPECT_NEAR(t.Get(), 0.0, 1e-9);
}

TEST(Timer, AcumulaFracao) {
    // dt de segundos fracionados (uso real: dt*1000).
    Timer t;
    for (int i = 0; i < 10; i++) {
        t.Update(33.3f);
    }
    EXPECT_NEAR(t.Get(), 333.0, 0.5);
}

MINITEST_MAIN()
