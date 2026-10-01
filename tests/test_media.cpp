// test_media.cpp - abre TODOS os audios do jogo pelo pipeline real.
//
// Music::Open (Mix_LoadMUS) para cada .ogg e Sound::Open (Mix_LoadWAV)
// para cada .wav/.ogg em assets/arquivos/audio. Sem placa de som e sem
// janela: so decodifica (Mix_Init), nao da Play. Headless-safe.
// Tambem prova o caminho de falha graciosa (arquivo inexistente nao
// fecha o programa: Open so registra, sem exit).
#ifndef RIPATEST_ASSETS_DIR
#error "RIPATEST_ASSETS_DIR nao definido (tests/CMakeLists.txt define)"
#endif

// Testes usam main() proprio: SDL_MAIN_HANDLED evita o WinMain do SDL2main.
#define SDL_MAIN_HANDLED

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include <SDL.h>
#include <SDL_mixer.h>

#include "minitest.h"
#include "Music.h"
#include "Sound.h"

namespace fs = std::filesystem;

// Coleta arquivos por extensao, ordenado (deterministico).
static std::vector<std::string> Collect(const std::string& ext) {
    std::vector<std::string> out;
    for (auto& e : fs::recursive_directory_iterator(std::string(RIPATEST_ASSETS_DIR) + "/audio")) {
        if (e.is_regular_file() && e.path().extension() == ext) {
            out.push_back(e.path().generic_string());
        }
    }
    std::sort(out.begin(), out.end());
    return out;
}

TEST(Media, InitSemPlaca) {
    // SDL_Init(0) nao pede video nem audio; Mix_Init prepara decoders.
    EXPECT_EQ(SDL_Init(0), 0);
    int got = Mix_Init(MIX_INIT_OGG | MIX_INIT_FLAC);
    EXPECT_TRUE((got & MIX_INIT_OGG) != 0);
    Mix_Quit();
    SDL_Quit();
}

TEST(Media, TodasMusicasOggAbrem) {
    EXPECT_EQ(SDL_Init(0), 0);
    Mix_Init(MIX_INIT_OGG | MIX_INIT_FLAC);
    // Mix_LoadMUS tambem exige dispositivo aberto: driver dummy.
    SDL_setenv("SDL_AUDIODRIVER", "dummy", 1);
    EXPECT_TRUE(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == 0);
    auto oggs = Collect(".ogg");
    EXPECT_TRUE(!oggs.empty());  // sanidade: achou arquivos
    for (auto& f : oggs) {
        Music m(f);
        EXPECT_TRUE(m.IsOpen());
        if (!m.IsOpen()) {
            std::cout << "    (falhou: " << f << ")\n";
        }
    }
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
}

TEST(Media, TodosEfeitosWavOggAbrem) {
    EXPECT_EQ(SDL_Init(0), 0);
    Mix_Init(MIX_INIT_OGG | MIX_INIT_FLAC);
    // Mix_LoadWAV exige dispositivo aberto: driver dummy (sem placa).
    SDL_setenv("SDL_AUDIODRIVER", "dummy", 1);
    EXPECT_TRUE(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == 0);
    auto wavs = Collect(".wav");
    EXPECT_TRUE(!wavs.empty());
    for (auto& f : wavs) {
        Sound s(f);
        EXPECT_TRUE(s.IsOpen());
        if (!s.IsOpen()) {
            std::cout << "    (falhou: " << f << ")\n";
        }
    }
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
}

TEST(Media, AusenteNaoMata) {
    // Caminho inexistente: IsOpen falso, sem exit/crash.
    EXPECT_EQ(SDL_Init(0), 0);
    Mix_Init(MIX_INIT_OGG);
    Music m("arquivos/audio/nao-existe-xyz.ogg");
    EXPECT_FALSE(m.IsOpen());
    Sound s("arquivos/audio/nao-existe-xyz.wav");
    EXPECT_FALSE(s.IsOpen());
    Mix_Quit();
    SDL_Quit();
}

MINITEST_MAIN()
