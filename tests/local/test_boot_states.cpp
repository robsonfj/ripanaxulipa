// test_boot_states.cpp - teste LOCAL com GPU (NAO roda no CI).
//
// Habilitar: cmake -DRIPA_ENABLE_LOCAL_TESTS=ON .. ; compila
// test_boot_states; execute com um display de verdade (janela real).
// Ele instancia o Game de verdade, empilha CADA estado, roda alguns
// frames de Update/Render em cada um, checa glGetError e salva
// screenshots .ppm em build/local-shots/ para inspecao visual.
//
// Por que local e nao CI: precisa de janela + GL completo + ~1min
// (StageState constroi a fase inteira na hora). No CI ficam os testes
// headless (ver tests/CMakeLists.txt).
#define SDL_MAIN_HANDLED

#include <cstdio>
#include <filesystem>
#include <string>
#include <utility>
#include <vector>

#include <SDL.h>
#include <SDL_opengl.h>

#include "Game.h"
#include "TitleState.h"
#include "InitState.h"
#include "StoryState.h"
#include "StageState.h"
#include "StoreState.h"
#include "OptionState.h"
#include "PauseState.h"
#include "EndState.h"
#include "RankingState.h"
#include "Credits.h"

namespace {

int g_failures = 0;

// Salva o backbuffer atual como PPM (P6 binario, sem libs extras).
void Screenshot(const std::string& path, int w, int h) {
    std::vector<unsigned char> px((size_t)w * h * 3);
    glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, px.data());
    FILE* fp = fopen(path.c_str(), "wb");
    if (!fp) {
        printf("FALHA ao salvar %s\n", path.c_str());
        ++g_failures;
        return;
    }
    // PPM e bottom-up: escreve de baixo para cima.
    fprintf(fp, "P6\n%d %d\n255\n", w, h);
    for (int y = h - 1; y >= 0; y--) {
        fwrite(px.data() + (size_t)y * w * 3, 1, (size_t)w * 3, fp);
    }
    fclose(fp);
}

bool CheckGL(const char* where) {
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        printf("FALHA glGetError em %s: 0x%x\n", where, (unsigned)err);
        ++g_failures;
        return false;
    }
    return true;
}

// Roda N frames fixos (dt de 60fps) num estado ja empilhado.
void RunFrames(State& st, int n, const std::string& shot) {
    for (int i = 0; i < n; i++) {
        st.Update(0.016f);
        st.Render();
        char where[128];
        snprintf(where, sizeof(where), "%s frame %d", shot.c_str(), i);
        CheckGL(where);
    }
    int w = Game::GetInstance().GetWindowWidth();
    int h = Game::GetInstance().GetWindowHeight();
    Screenshot("local-shots/" + shot + ".ppm", w, h);
}

// Empilha e devolve o topo (Push sozinho nao basta fora do Run).
template <typename T, typename... Args>
T& PushState(Args&&... args) {
    Game::GetInstance().Push(new T(std::forward<Args>(args)...));
    Game::GetInstance().FlushForTest();
    return static_cast<T&>(Game::GetCurrentState());
}

}  // namespace

int main() {
    std::filesystem::create_directories("local-shots");
    Game jogo("RIPA NA XULIPA - teste local", 1024, 600);

    // Splash: rapido (4s de timer; 5 frames bastam p/ render).
    RunFrames(PushState<InitState>(), 5, "01-init");

    // Menu principal.
    RunFrames(PushState<TitleState>(), 5, "02-title");

    // Historia (slot 0; thread de load da fase comeca em paralelo).
    RunFrames(PushState<StoryState>(), 5, "03-story");

    // Telas simples (sem dependencia de jogo salvo).
    RunFrames(PushState<StoreState>(), 5, "04-store");
    RunFrames(PushState<OptionState>(), 5, "05-options");
    RunFrames(PushState<RankingState>(), 5, "06-ranking");
    RunFrames(PushState<Credits>(), 5, "07-credits");

    RunFrames(PushState<PauseState>(), 5, "08-pause");
    RunFrames(PushState<EndState>(false), 5, "09-end-lose");
    RunFrames(PushState<EndState>(true), 5, "10-end-win");

    // Fase completa construida na main thread (~15-20s com os delays
    // originais de load). Roda 3 frames e fotografa gameplay parado.
    RunFrames(PushState<StageState>(), 3, "11-stage");

    printf(g_failures == 0 ? "LOCAL OK: sem erro de GL\n"
                           : "LOCAL COM FALHAS: %d\n", g_failures);
    return g_failures;
}
