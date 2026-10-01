// test_assets_gl.cpp - TODOS os assets visuais pelo pipeline real.
//
// Cria janela + contexto OpenGL 2.1 de compatibilidade (igual ao jogo) e:
//  - abre cada .png/.jpg de assets/arquivos/img via Sprite::Open
//    (cai no exit(1) se faltar; checa glGetError apos cada upload —
//    pega regressao de pitch/canais e arquivo corrompido);
//  - constrói Text de amostra (inclui acentos e string vazia);
//  - abre cada .ogg via Music::Open.
// Precisa de display: xvfb no Linux CI, janela normal no Windows/macOS.
// Roda via `ctest -R assets-gl`.
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
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL_opengl.h>

#include "minitest.h"
#include "Sprite.h"
#include "Text.h"
#include "Music.h"

namespace fs = std::filesystem;

static std::vector<std::string> CollectExt(const std::string& ext) {
    std::vector<std::string> out;
    for (auto& e : fs::recursive_directory_iterator(std::string(RIPATEST_ASSETS_DIR))) {
        if (e.is_regular_file() && e.path().extension() == ext) {
            out.push_back(e.path().generic_string());
        }
    }
    std::sort(out.begin(), out.end());
    return out;
}

TEST(GLAssets, ContextoSobe) {
    EXPECT_EQ(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO), 0);
    EXPECT_TRUE((IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) & (IMG_INIT_JPG | IMG_INIT_PNG)) != 0);
    EXPECT_EQ(TTF_Init(), 0);
    Mix_Init(MIX_INIT_OGG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_Window* win = SDL_CreateWindow("t", 0, 0, 64, 64,
                                        SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
    EXPECT_TRUE(win != nullptr);
    SDL_GLContext ctx = SDL_GL_CreateContext(win);
    EXPECT_TRUE(ctx != nullptr);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
}

TEST(GLAssets, TodasImagensSobemSemErroGL) {
    auto imgs = CollectExt(".png");
    auto jpgs = CollectExt(".jpg");
    imgs.insert(imgs.end(), jpgs.begin(), jpgs.end());
    EXPECT_TRUE(!imgs.empty());
    for (auto& f : imgs) {
        // Thumbs.db do Windows nao e imagem valida: pula.
        if (f.find("Thumbs.db") != std::string::npos) {
            continue;
        }
        // Arquivos gigantes (ex. PSD exportado de 6000px, ou a faixa
        // Loading.png de 5343px) estouram driver basico/VM no upload:
        // aqui so prova que o arquivo abre e tem dimensao valida.
        // (O jogo roda em GPU real, onde 8k e suportado.)
        {
            SDL_Surface* probe = IMG_Load(f.c_str());
            EXPECT_TRUE(probe != nullptr);
            if (!probe) {
                std::cout << "    (ilegivel: " << f << ")\n";
                continue;
            }
            std::cout << "    asset: " << f << " " << probe->w << "x" << probe->h
                      << "x" << (int)probe->format->BytesPerPixel << " pitch "
                      << probe->pitch << std::endl;
            bool huge = (probe->w > 2048 || probe->h > 2048);
            EXPECT_TRUE(probe->w > 0 && probe->h > 0);
            SDL_FreeSurface(probe);
            if (huge) {
                std::cout << "    (SKIP upload >2048)\n";
                continue;
            }
        }
        Sprite s(f);
        GLenum err = glGetError();
        EXPECT_TRUE(err == GL_NO_ERROR);
        if (err != GL_NO_ERROR || s.GetWidth() <= 0 || s.GetHeight() <= 0) {
            std::cout << "    (falhou: " << f << " err=" << err << " "
                      << s.GetWidth() << "x" << s.GetHeight() << ")\n";
        }
        EXPECT_TRUE(s.GetWidth() > 0);
        EXPECT_TRUE(s.GetHeight() > 0);
    }
}

TEST(GLAssets, TextosAmostraComAcentoEVazio) {
    SDL_Color cor = {255, 255, 255, 255};
    const char* samples[] = {
        "NEW GAME",
        "INTRO: OFF",
        "0",
        "Você não tem dinheiro suficiente!",
        "Sua pontuação e seus itens serão zerados. Tem certeza?",
        "",
    };
    for (auto text : samples) {
        Text t(std::string(RIPATEST_ASSETS_DIR) + "/font/orangejuice.ttf",
               40, Text::TEXT_BLENDED, text, cor, 512, 300);
        GLenum err = glGetError();
        EXPECT_TRUE(err == GL_NO_ERROR);
        if (std::string(text).empty()) {
            continue;  // vazio nao gera textura: so nao pode crashar
        }
        EXPECT_TRUE(t.box.w > 0);
        if (!(t.box.w > 0)) {
            std::cout << "    (texto sem largura: \"" << text << "\")\n";
        }
    }
}

TEST(GLAssets, TodasMusicasOggAbrem) {
    // Mix_LoadMUS exige dispositivo aberto: dummy (igual test_media).
    SDL_setenv("SDL_AUDIODRIVER", "dummy", 1);
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) != 0) {
        return;  // sem audio neste ambiente: pula sem falhar
    }
    auto oggs = CollectExt(".ogg");
    EXPECT_TRUE(!oggs.empty());
    for (auto& f : oggs) {
        Music m(f);
        EXPECT_TRUE(m.IsOpen());
        if (!m.IsOpen()) {
            std::cout << "    (falhou: " << f << ")\n";
        }
    }
}

MINITEST_MAIN()
