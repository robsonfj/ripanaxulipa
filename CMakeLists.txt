cmake_minimum_required(VERSION 3.20)
project(RipaNaXulipa LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Evita warnings de fopen/fscanf no MSVC (codigo antigo usa API C)
add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

# Fontes em UTF-8 com acentos (ç, ã, é): força MSVC a ler strings como UTF-8
if(MSVC)
    add_compile_options(/utf-8)
endif()

# Opcoes:
#  RIPA_ENABLE_TESTS - compila e registra os testes (CTest). Padrao ON.
#  RIPA_COVERAGE     - flags --coverage (GCC/Clang, ex CI Linux + lcov).
option(RIPA_ENABLE_TESTS "Compilar testes (CTest)" ON)
option(RIPA_COVERAGE "Instrumentar cobertura (GCC/Clang)" OFF)
if(RIPA_COVERAGE)
    if(NOT CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        message(WARNING "RIPA_COVERAGE so funciona com GCC/Clang")
    endif()
    add_compile_options(--coverage -O0 -g)
    add_link_options(--coverage)
endif()

# SDL2 + satelites: usa o que ja existe (vcpkg/sistema) ou baixa
# pre-compilados oficiais no Windows. Ver cmake/Dependencies.cmake.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(Dependencies)

# Layout: src/{core,engine,gameplay,states}, assets/arquivos.
# Includes sao por nome de arquivo ("Sprite.h"), entao todos os
# diretorios entram no include path (sem mudar #includes).
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")

set(GAME_SOURCES
    # engine (infra: janela, estados base, input, midia, primitivas)
    "${SRC_DIR}/engine/Game.cpp"
    "${SRC_DIR}/engine/State.cpp"
    "${SRC_DIR}/engine/Camera.cpp"
    "${SRC_DIR}/engine/InputManager.cpp"
    "${SRC_DIR}/engine/Sprite.cpp"
    "${SRC_DIR}/engine/Text.cpp"
    "${SRC_DIR}/engine/Music.cpp"
    "${SRC_DIR}/engine/Sound.cpp"
    "${SRC_DIR}/engine/Rect.cpp"
    "${SRC_DIR}/engine/Point.cpp"
    "${SRC_DIR}/engine/ColorRect.cpp"
    # gameplay (regras e entidades da fase)
    "${SRC_DIR}/gameplay/MainCharacter.cpp"
    "${SRC_DIR}/gameplay/Cachorro.cpp"
    "${SRC_DIR}/gameplay/Road.cpp"
    "${SRC_DIR}/gameplay/RoadSegment.cpp"
    "${SRC_DIR}/gameplay/Buildings.cpp"
    "${SRC_DIR}/gameplay/Objetos.cpp"
    "${SRC_DIR}/gameplay/ObjPespectiva.cpp"
    "${SRC_DIR}/gameplay/Nuvem.cpp"
    "${SRC_DIR}/gameplay/Money.cpp"
    "${SRC_DIR}/gameplay/PowerUp.cpp"
    # states (telas do jogo)
    "${SRC_DIR}/states/TitleState.cpp"
    "${SRC_DIR}/states/InitState.cpp"
    "${SRC_DIR}/states/StoryState.cpp"
    "${SRC_DIR}/states/LoadState.cpp"
    "${SRC_DIR}/states/StageState.cpp"
    "${SRC_DIR}/states/StoreState.cpp"
    "${SRC_DIR}/states/StoreItem.cpp"
    "${SRC_DIR}/states/OptionState.cpp"
    "${SRC_DIR}/states/PauseState.cpp"
    "${SRC_DIR}/states/EndState.cpp"
    "${SRC_DIR}/states/RankingState.cpp"
    "${SRC_DIR}/states/Credits.cpp"
    "${SRC_DIR}/states/MessageWindow.cpp"
    "${SRC_DIR}/states/CheatMode.cpp"
)

# Biblioteca estatica com todo o jogo (menos main): o executavel e os
# testes linkam contra ela, sem duplicar compilacao.
add_library(ripalib STATIC ${GAME_SOURCES})
target_include_directories(ripalib PUBLIC
    "${SRC_DIR}"
    "${SRC_DIR}/core"
    "${SRC_DIR}/engine"
    "${SRC_DIR}/gameplay"
    "${SRC_DIR}/states"
)
target_link_libraries(ripalib PUBLIC
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_mixer::SDL2_mixer
    SDL2_ttf::SDL2_ttf
)
# OpenGL legacy por plataforma (o jogo usa immediate mode 1.x).
if(WIN32)
    target_link_libraries(ripalib PUBLIC opengl32 winmm version imm32)
elseif(APPLE)
    target_link_libraries(ripalib PUBLIC "-framework OpenGL")
else()
    target_link_libraries(ripalib PUBLIC GL)
endif()

add_executable(RipaNaXulipa "${SRC_DIR}/main.cpp")
target_link_libraries(RipaNaXulipa PRIVATE ripalib SDL2::SDL2main)

# Assets: o jogo abre arquivos com caminho relativo "arquivos/...".
# Copia a pasta para o diretorio do executavel apos o build.
add_custom_command(TARGET RipaNaXulipa POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets/arquivos"
        "$<TARGET_FILE_DIR:RipaNaXulipa>/arquivos"
    COMMENT "Copiando assets arquivos/ para junto do exe"
)

# DLLs runtime no Windows (vindo do vcpkg applocal ou dos -devel baixados).
if(WIN32)
    if(DEFINED VCPKG_INSTALLED_DIR)
        # vcpkg copia via applocal automaticamente.
    elseif(RIPA_SDL_DLLS)
        foreach(_dll IN LISTS RIPA_SDL_DLLS)
            add_custom_command(TARGET RipaNaXulipa POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${_dll}" "$<TARGET_FILE_DIR:RipaNaXulipa>"
                COMMENT "Copiando ${_dll}")
        endforeach()
    endif()
endif()

if(RIPA_ENABLE_TESTS)
    include(CTest)
    add_subdirectory(tests)
endif()
