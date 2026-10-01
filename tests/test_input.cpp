// test_input.cpp - InputManager sem janela, com video dummy.
//
// SDL_VIDEODRIVER=dummy permite SDL_Init(VIDEO) headless (sem X/Wayland).
// Eventos sinteticos via SDL_PushEvent exercitam o ciclo de cada tecla:
// JUST_PRESSED (KeyPress) -> PRESSED (IsKeyDown) -> JUST_RELEASED
// (KeyRelease) -> RELEASED. Cada caso usa tecla/botao proprios porque o
// InputManager e singleton e o estado persiste entre casos.
#ifndef RIPATEST_ASSETS_DIR
#error "RIPATEST_ASSETS_DIR nao definido (tests/CMakeLists.txt define)"
#endif

// Testes usam main() proprio: SDL_MAIN_HANDLED evita o WinMain do SDL2main.
#define SDL_MAIN_HANDLED

#include <SDL.h>

#include "minitest.h"
#include "InputManager.h"
#include "Rect.h"

static InputManager& IM() { return InputManager::GetInstance(); }
static bool g_video_ok = false;

// Bombeia a fila uma vez (igual ao Game::Run faz por frame).
static void Pump() { IM().Update(); }

// Fabrica evento de teclado pronto para SDL_PushEvent.
static void PushKey(Uint32 type, SDL_Keycode sym) {
    SDL_Event e;
    SDL_memset(&e, 0, sizeof(e));
    e.type = type;
    e.key.keysym.sym = sym;
    e.key.state = (type == SDL_KEYDOWN) ? SDL_PRESSED : SDL_RELEASED;
    SDL_PushEvent(&e);
}

static void PushMouseButton(Uint32 type, Uint8 button) {
    SDL_Event e;
    SDL_memset(&e, 0, sizeof(e));
    e.type = type;
    e.button.button = button;
    e.button.state = (type == SDL_MOUSEBUTTONDOWN) ? SDL_PRESSED : SDL_RELEASED;
    SDL_PushEvent(&e);
}

static void PushMotion(int x, int y) {
    SDL_Event e;
    SDL_memset(&e, 0, sizeof(e));
    e.type = SDL_MOUSEMOTION;
    e.motion.x = x;
    e.motion.y = y;
    SDL_PushEvent(&e);
}

TEST(Input, VideoDummySobe) {
    SDL_setenv("SDL_VIDEODRIVER", "dummy", 1);
    g_video_ok = (SDL_Init(SDL_INIT_VIDEO) == 0);
    EXPECT_TRUE(g_video_ok);
}

// Pula o caso (sem falhar) quando o video nao subiu: sem SDL de video
// nao ha fila de eventos e forcar ia crashar em vez de testar.
#define NEED_VIDEO()                                       \
    do {                                                   \
        if (!g_video_ok) {                                 \
            std::cout << "    (SKIP sem video)\n";          \
            return;                                        \
        }                                                  \
    } while (0)

TEST(Input, CicloTecla) {
    NEED_VIDEO();
    // DOWN: so KeyPress; outro pump sem evento: vira IsKeyDown.
    PushKey(SDL_KEYDOWN, SDLK_F1);
    Pump();
    EXPECT_TRUE(IM().KeyPress(SDLK_F1));
    EXPECT_FALSE(IM().IsKeyDown(SDLK_F1));
    Pump();
    EXPECT_FALSE(IM().KeyPress(SDLK_F1));
    EXPECT_TRUE(IM().IsKeyDown(SDLK_F1));
    // UP: so KeyRelease; depois some.
    PushKey(SDL_KEYUP, SDLK_F1);
    Pump();
    EXPECT_TRUE(IM().KeyRelease(SDLK_F1));
    Pump();
    EXPECT_FALSE(IM().KeyRelease(SDLK_F1));
    EXPECT_FALSE(IM().IsKeyDown(SDLK_F1));
}

TEST(Input, KeyUpSemKeyDownNaoCrasha) {
    NEED_VIDEO();
    // Regressao P0-2: KEYUP orfao (Alt-Tab etc.) desreferenciava end().
    PushKey(SDL_KEYUP, SDLK_F2);
    Pump();  // nao pode crashar
    EXPECT_FALSE(IM().KeyRelease(SDLK_F2));
}

TEST(Input, TeclasIndependentes) {
    NEED_VIDEO();
    PushKey(SDL_KEYDOWN, SDLK_F3);
    Pump();
    EXPECT_TRUE(IM().KeyPress(SDLK_F3));
    EXPECT_FALSE(IM().KeyPress(SDLK_F4));
    PushKey(SDL_KEYUP, SDLK_F3);
    Pump();
    Pump();
}

TEST(Input, CicloBotaoMouse) {
    NEED_VIDEO();
    PushMouseButton(SDL_MOUSEBUTTONDOWN, SDL_BUTTON_LEFT);
    Pump();
    EXPECT_TRUE(IM().MousePress(SDL_BUTTON_LEFT));
    Pump();
    EXPECT_FALSE(IM().MousePress(SDL_BUTTON_LEFT));
    PushMouseButton(SDL_MOUSEBUTTONUP, SDL_BUTTON_LEFT);
    Pump();
    EXPECT_FALSE(IM().MousePress(SDL_BUTTON_LEFT));
}

TEST(Input, DebounceJitter) {
    NEED_VIDEO();
    // Jitter de <=2px nao e "movimento" (nao desliga teclado dos menus).
    PushMotion(0, 0);
    Pump();
    PushMotion(1, 1);
    Pump();
    EXPECT_FALSE(IM().mouseMoving);
    // Salto real conta.
    PushMotion(100, 100);
    Pump();
    EXPECT_TRUE(IM().mouseMoving);
}

TEST(Input, MouseInsideOrigem) {
    NEED_VIDEO();
    Pump();  // dummy video: cursor em (0,0)
    EXPECT_TRUE(IM().IsMouseInside(Rect(0, 0, 1024, 600)));
    EXPECT_FALSE(IM().IsMouseInside(Rect(10, 10, 100, 100)));
}

TEST(Input, TextoAcumulaELimpa) {
    NEED_VIDEO();
    SDL_Event e;
    SDL_memset(&e, 0, sizeof(e));
    e.type = SDL_TEXTINPUT;
    SDL_strlcpy(e.text.text, "ab", sizeof(e.text.text));
    int pushed = SDL_PushEvent(&e);
    std::cout << "    (push=" << pushed << ")" << std::endl;
    if (pushed <= 0) {
        std::cout << "    (SDL_PushEvent recusou TEXTINPUT)\n";
        return;
    }
    Pump();
    std::cout << "    (pump ok)" << std::endl;
    // Captura em variavel local: cada Text() consome (limpa) o buffer.
    std::string primeira = IM().Text();
    std::cout << "    (lido [" << primeira.size() << " chars])" << std::endl;
    EXPECT_STREQ(primeira.c_str(), "ab");
    std::string segunda = IM().Text();
    EXPECT_STREQ(segunda.c_str(), "");
}

MINITEST_MAIN()
