# tests/CMakeLists.txt - suites de teste (CTest).
#
# 5 executaveis, um por area (isolamento: falha num nao trava outro):
#  test_engine    - Rect/Point/Timer (puro, headless, sem SDL/GL)
#  test_game      - saves e regras puras (fixture com CWD temporario)
#  test_media     - abre todos os audios (SDL+Mixer, sem placa/janela)
#  test_assets_gl - abre todas as imagens/textos/musicas com GL real
#                   (precisa display: xvfb no Linux CI)
#  test_input     - InputManager com video dummy + eventos sinteticos
#
# Todos linkam a libripa (biblioteca estatica do jogo) e usam minitest.h
# (framework proprio, zero dependencias). RIPATEST_ASSETS_DIR aponta
# para assets/arquivos (caminho absoluto do source).

set(RIPA_TEST_ASSETS "${CMAKE_SOURCE_DIR}/assets/arquivos")

# test_engine nao precisa de SDL em runtime, mas linka libripa
# (que ja traz os includes/libs) para reaproveitar Rect/Point/Timer.
foreach(suite engine game media assets_gl input)
    add_executable(test_${suite} test_${suite}.cpp)
    target_link_libraries(test_${suite} PRIVATE ripalib)
    target_compile_definitions(test_${suite} PRIVATE
        RIPATEST_ASSETS_DIR="${RIPA_TEST_ASSETS}")
    add_test(NAME ${suite} COMMAND test_${suite})
    ripa_bundle_sdl_dlls(test_${suite})
endforeach()

# test_game manipula arquivos relativos: roda num CWD proprio e limpo.
# (A fixture troca o CWD sozinha; o WORKING_DIRECTORY so ancora.)
set_tests_properties(game PROPERTIES WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

# Testes locais com GPU: fora do CI, so com -DRIPA_ENABLE_LOCAL_TESTS=ON.
# Ver tests/local/CMakeLists.txt (binarios manuais, sem registro no CTest).
option(RIPA_ENABLE_LOCAL_TESTS "Testes locais com GPU (fora do CI)" OFF)
if(RIPA_ENABLE_LOCAL_TESTS)
    add_subdirectory(local)
endif()
