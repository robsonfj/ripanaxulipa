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

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)

set(GAME_SRC_DIR "${CMAKE_SOURCE_DIR}/Ripa na Xulipa/Codigo")

set(GAME_SOURCES
    "${GAME_SRC_DIR}/main.cpp"
    "${GAME_SRC_DIR}/Game.cpp"
    "${GAME_SRC_DIR}/Sprite.cpp"
    "${GAME_SRC_DIR}/Text.cpp"
    "${GAME_SRC_DIR}/Music.cpp"
    "${GAME_SRC_DIR}/Sound.cpp"
    "${GAME_SRC_DIR}/InputManager.cpp"
    "${GAME_SRC_DIR}/Camera.cpp"
    "${GAME_SRC_DIR}/Point.cpp"
    "${GAME_SRC_DIR}/Rect.cpp"
    "${GAME_SRC_DIR}/State.cpp"
    "${GAME_SRC_DIR}/ColorRect.cpp"
    "${GAME_SRC_DIR}/Buildings.cpp"
    "${GAME_SRC_DIR}/Cachorro.cpp"
    "${GAME_SRC_DIR}/MainCharacter.cpp"
    "${GAME_SRC_DIR}/MessageWindow.cpp"
    "${GAME_SRC_DIR}/Money.cpp"
    "${GAME_SRC_DIR}/Nuvem.cpp"
    "${GAME_SRC_DIR}/Objetos.cpp"
    "${GAME_SRC_DIR}/ObjPespectiva.cpp"
    "${GAME_SRC_DIR}/Road.cpp"
    "${GAME_SRC_DIR}/RoadSegment.cpp"
    "${GAME_SRC_DIR}/StillAnimation.cpp"
    "${GAME_SRC_DIR}/StoreItem.cpp"
    "${GAME_SRC_DIR}/StoryState.cpp"
    "${GAME_SRC_DIR}/CheatMode.cpp"
    "${GAME_SRC_DIR}/Credits.cpp"
    "${GAME_SRC_DIR}/EndState.cpp"
    "${GAME_SRC_DIR}/InitState.cpp"
    "${GAME_SRC_DIR}/LoadState.cpp"
    "${GAME_SRC_DIR}/OptionState.cpp"
    "${GAME_SRC_DIR}/PauseState.cpp"
    "${GAME_SRC_DIR}/PowerUp.cpp"
    "${GAME_SRC_DIR}/RankingState.cpp"
    "${GAME_SRC_DIR}/StageState.cpp"
    "${GAME_SRC_DIR}/StoreState.cpp"
    "${GAME_SRC_DIR}/TitleState.cpp"
)

add_executable(RipaNaXulipa ${GAME_SOURCES})

target_include_directories(RipaNaXulipa PRIVATE "${GAME_SRC_DIR}")

target_link_libraries(RipaNaXulipa PRIVATE
    SDL2::SDL2
    SDL2::SDL2main
    $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
)

# SDL2_mixer / SDL2_ttf (nomes de target variam entre versoes do vcpkg)
if(TARGET SDL2_mixer::SDL2_mixer)
    target_link_libraries(RipaNaXulipa PRIVATE SDL2_mixer::SDL2_mixer)
else()
    target_link_libraries(RipaNaXulipa PRIVATE SDL2_mixer::SDL2_mixer-static)
endif()

if(TARGET SDL2_ttf::SDL2_ttf)
    target_link_libraries(RipaNaXulipa PRIVATE SDL2_ttf::SDL2_ttf)
else()
    target_link_libraries(RipaNaXulipa PRIVATE SDL2_ttf::SDL2_ttf-static)
endif()

# OpenGL legacy (immediate mode) no Windows
target_link_libraries(RipaNaXulipa PRIVATE opengl32 winmm version imm32)

# Assets: o jogo abre arquivos com caminho relativo "arquivos/...".
# Copia a pasta para o diretorio do executavel apos o build.
add_custom_command(TARGET RipaNaXulipa POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Ripa na Xulipa/arquivos"
        "$<TARGET_FILE_DIR:RipaNaXulipa>/arquivos"
    COMMENT "Copiando assets arquivos/ para junto do exe"
)

# Copia DLLs do vcpkg (SDL2, etc) para junto do exe
include("${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/share/SDL2/SDL2Config.cmake" OPTIONAL)
if(DEFINED VCPKG_INSTALLED_DIR)
    add_custom_command(TARGET RipaNaXulipa POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Vcpkg applocal deps via install"
    )
endif()
