# cmake/Dependencies.cmake
# Dependencias SDL2/SDL2_image/SDL2_mixer/SDL2_ttf, sem vcpkg obrigatorio:
#  1) Tenta achar pacotes ja instalados (vcpkg, sistema, apt, brew):
#     find_package ... CONFIG. Se achar os 4, usa e pula o resto.
#  2) No Windows, o que faltar baixa os pacotes -devel oficiais
#     (libsdl-org, versoes fixadas abaixo) e cria os targets importados
#     SDL2::SDL2, SDL2::SDL2main, SDL2_image::SDL2_image,
#     SDL2_mixer::SDL2_mixer, SDL2_ttf::SDL2_ttf. Sem admin, sem compilar.
#  3) No Linux/macOS sem pacotes: erro fatal com instrucoes (apt/brew),
#     pois la o gerenciador do SO e o caminho padrao (o CI instala).
#
# Ao final, RIPA_SDL_DLLS tem as DLLs runtime (Windows) para copiar
# para junto do executavel.

set(RIPA_SDL_VERSION "2.32.10")
set(RIPA_SDL_IMAGE_VERSION "2.8.8")
set(RIPA_SDL_MIXER_VERSION "2.8.2")
set(RIPA_SDL_TTF_VERSION "2.24.0")

find_package(SDL2 CONFIG QUIET)
find_package(SDL2_image CONFIG QUIET)
find_package(SDL2_mixer CONFIG QUIET)
find_package(SDL2_ttf CONFIG QUIET)

# Nomes de target que o jogo usa (iguais aos do vcpkg e dos configs oficiais).
if(SDL2_FOUND AND SDL2_image_FOUND AND SDL2_mixer_FOUND AND SDL2_ttf_FOUND)
    message(STATUS "Dependencias SDL encontradas no sistema (sem download).")
    set(RIPA_SDL_DLLS "" CACHE INTERNAL "")
else()
    if(NOT WIN32)
        message(FATAL_ERROR
            "SDL2/SDL2_image/SDL2_mixer/SDL2_ttf nao encontrados.\n"
            "Ubuntu/Debian: sudo apt install libsdl2-dev libsdl2-image-dev "
            "libsdl2-mixer-dev libsdl2-ttf-dev libgl1-mesa-dev\n"
            "macOS: brew install sdl2 sdl2_image sdl2_mixer sdl2_ttf")
    endif()

    # --- Windows: baixa os -devel oficiais e cria targets importados ---
    set(RIPA_DEPS_DIR "${CMAKE_BINARY_DIR}/_sdldeps" CACHE PATH "Pasta das deps baixadas")
    file(MAKE_DIRECTORY "${RIPA_DEPS_DIR}")
    set(RIPA_SDL_DLLS "" CACHE INTERNAL "")

    # Baixa e extrai um pacote -devel; devolve a raiz em <OUT_VAR>.
    # DIRPREFIX e o prefixo da pasta interna (ex SDL2, SDL2_image).
    macro(ripa_fetch_devel ORG PROJ VER DIRPREFIX OUT_VAR)
        set(_zip "${RIPA_DEPS_DIR}/${PROJ}-devel-${VER}-VC.zip")
        if(NOT EXISTS "${_zip}")
            message(STATUS "Baixando ${PROJ} ${VER} (oficial, pre-compilado)...")
            file(DOWNLOAD
                "https://github.com/libsdl-org/${ORG}/releases/download/release-${VER}/${PROJ}-devel-${VER}-VC.zip"
                "${_zip}" SHOW_PROGRESS STATUS _st)
            list(GET _st 0 _code)
            if(NOT _code EQUAL 0)
                message(FATAL_ERROR "Download falhou (${PROJ} ${VER}): ${_st}")
            endif()
        else()
            message(STATUS "Reutilizando ${PROJ} ${VER} ja baixado.")
        endif()
        set(_stamp "${RIPA_DEPS_DIR}/${PROJ}-${VER}.extracted")
        if(NOT EXISTS "${_stamp}")
            file(ARCHIVE_EXTRACT INPUT "${_zip}" DESTINATION "${RIPA_DEPS_DIR}")
            file(TOUCH "${_stamp}")
        endif()
        file(GLOB _roots "${RIPA_DEPS_DIR}/${DIRPREFIX}-*")
        list(FILTER _roots EXCLUDE REGEX ".*\\.zip$")
        list(FILTER _roots EXCLUDE REGEX ".*\\.extracted$")
        list(GET _roots 0 ${OUT_VAR})
    endmacro()

    # Cria target SHARED importado a partir da pasta extraida.
    # _lib: nome base sem extensao (ex SDL2). DLLs coletadas p/ install.
    macro(ripa_import_shared TARGET INC_DIR LIB_DIR LIB_BASENAME)
        if(NOT TARGET ${TARGET})
            add_library(${TARGET} SHARED IMPORTED GLOBAL)
            file(GLOB _dll "${LIB_DIR}/${LIB_BASENAME}.dll")
            set_target_properties(${TARGET} PROPERTIES
                INTERFACE_INCLUDE_DIRECTORIES "${INC_DIR}"
                IMPORTED_LOCATION "${_dll}"
                IMPORTED_IMPLIB "${LIB_DIR}/${LIB_BASENAME}.lib")
            file(GLOB _alldlls "${LIB_DIR}/*.dll")
            list(APPEND RIPA_SDL_DLLS ${_alldlls})
        endif()
    endmacro()

    if(NOT SDL2_FOUND)
        ripa_fetch_devel(SDL SDL2 ${RIPA_SDL_VERSION} SDL2 _sdl2)
        add_library(SDL2::SDL2 SHARED IMPORTED GLOBAL)
        set_target_properties(SDL2::SDL2 PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${_sdl2}/include"
            IMPORTED_LOCATION "${_sdl2}/lib/x64/SDL2.dll"
            IMPORTED_IMPLIB "${_sdl2}/lib/x64/SDL2.lib")
        add_library(SDL2::SDL2main STATIC IMPORTED GLOBAL)
        set_target_properties(SDL2::SDL2main PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${_sdl2}/include"
            IMPORTED_LOCATION "${_sdl2}/lib/x64/SDL2main.lib")
        file(GLOB _d "${_sdl2}/lib/x64/*.dll")
        list(APPEND RIPA_SDL_DLLS ${_d})
        set(SDL2_FOUND TRUE)
    endif()

    if(NOT SDL2_image_FOUND)
        ripa_fetch_devel(SDL_image SDL2_image ${RIPA_SDL_IMAGE_VERSION} SDL2_image _img)
        ripa_import_shared(SDL2_image::SDL2_image
            "${_img}/include" "${_img}/lib/x64" "SDL2_image")
        set(SDL2_image_FOUND TRUE)
    endif()

    if(NOT SDL2_mixer_FOUND)
        ripa_fetch_devel(SDL_mixer SDL2_mixer ${RIPA_SDL_MIXER_VERSION} SDL2_mixer _mix)
        ripa_import_shared(SDL2_mixer::SDL2_mixer
            "${_mix}/include" "${_mix}/lib/x64" "SDL2_mixer")
        set(SDL2_mixer_FOUND TRUE)
    endif()

    if(NOT SDL2_ttf_FOUND)
        ripa_fetch_devel(SDL_ttf SDL2_ttf ${RIPA_SDL_TTF_VERSION} SDL2_ttf _ttf)
        ripa_import_shared(SDL2_ttf::SDL2_ttf
            "${_ttf}/include" "${_ttf}/lib/x64" "SDL2_ttf")
        set(SDL2_ttf_FOUND TRUE)
    endif()

    set(RIPA_SDL_DLLS ${RIPA_SDL_DLLS} CACHE INTERNAL "")
    list(REMOVE_DUPLICATES RIPA_SDL_DLLS)
endif()
