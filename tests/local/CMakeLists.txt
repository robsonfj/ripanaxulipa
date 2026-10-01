# tests/local/CMakeLists.txt - testes LOCAIS com GPU (fora do CI).
#
# So entra com -DRIPA_ENABLE_LOCAL_TESTS=ON (padrao OFF: o CI nunca ve).
# Precisa de display real com OpenGL (janela de verdade). NENHUM teste
# daqui e registrado no CTest: sao binarios manuais para rodar na
# maquina do desenvolvedor.
#
#  test_boot_states - instancia o Game de verdade, empilha todos os
#    estados, roda frames de Update/Render em cada um, checa glGetError
#    e salva screenshots .ppm em <cwd>/local-shots/ p/ inspecao visual.
#    Rodar na raiz do build com os assets por perto, ex:
#      cd build && ./tests/local/test_boot_states   (ou Release\ no Win)

add_executable(test_boot_states test_boot_states.cpp)
target_link_libraries(test_boot_states PRIVATE ripalib)
# SDL_MAIN_HANDLED no fonte: main() proprio, sem SDL2main.
