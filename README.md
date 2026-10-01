# Ripa na Xulipa

Jogo 2D de corrida pseudo-3D ("endless runner") feito em C++ com SDL2 + OpenGL
(immediate mode). Você corre pela avenida desviando de obstáculos, coletando
papéis e moedas, comprando power-ups na loja e tentando a maior nota.

> Projeto original de faculdade (2014, Xcode/macOS). Portado para
> Windows 11 + MSVC + CMake. O projeto Xcode em `legacy/` está desativado
> e serve só como referência histórica.

## Controles

| Entrada | Ação |
|---|---|
| Setas / W A S D | Navegar menus, mover o personagem |
| W | Pular |
| ENTER | Confirmar / pular história / começar na tela de load |
| ESC | Voltar / pausar / sair da tela atual |
| Mouse | Clicar nos itens (clique ativa o item sob o cursor; ENTER ativa o da seta) |
| Ctrl+C (na pausa) | Abre o cheat mode (digite o código + ENTER, ESC cancela) |

Cheats: `showfps`, `superpulo on/off`, `intang on/off`, `revive on/off`,
`klapaucius` (+1000 moedas), `wingame[-SS/-MS/-MM/-MI/-II]`, `losegame`.

Regras de pulo: buracos se evitam pulando; obstáculos (gatos, caixas etc.)
perdoam o pulo **alto** (metade de cima do salto passa limpo e em silêncio,
sem som de batida); pulo baixo ainda colide. No ar o personagem segura o
frame mais "aéreo" da corrida (não há arte separada de pulo).

## Fluxo de telas

```
splash (InitState, logo ~4s)
  -> menu (TitleState)
    -> NEW GAME -> história (StoryState, 5s/cena) -> fase (StageState)
                \-> (intro desligada) -> load (LoadState) -> fase
    -> STORE / OPTIONS / RANKING / CREDITS
```

- A história carrega a fase **em paralelo**: se o load terminar antes,
  entra direto na fase; se você pular cedo, cai na tela de load de onde parou.
- Options tem o toggle **INTRO: ON/OFF** (persistido em
  `arquivos/save/config.txt`).

## Estrutura

```
CMakeLists.txt        build (usa SDL do sistema/vcpkg ou baixa sozinho no Win)
run.bat               atalho: configura + compila + executa
src/
  main.cpp            entrada: cria o Game e empilha o estado inicial
  core/               Sync.h: trava global das threads de load
  engine/             infra: Game, State, GameObject, Camera, InputManager,
                      Sprite, Text, Music, Sound, Rect, Point, Timer, ColorRect
  gameplay/           regras e entidades: MainCharacter, Cachorro, Road,
                      RoadSegment, Buildings, Objetos, ObjPespectiva,
                      Nuvem, Money, PowerUp
  states/             telas: Title, Init, Story, Load, Stage, Store(+Item),
                      Option, Pause, End, Ranking, Credits, MessageWindow, CheatMode
assets/arquivos/     imagens, áudios, fonte e saves (copiados p/ junto do .exe)
  img/ | audio/ | font/orangejuice.ttf | save/coins.txt, highscores.txt,
    storehistory.txt, config.txt
legacy/               projeto Xcode original (desativado)
build/                saída do CMake (ignorado pelo git)
```

Notas de implementação relevantes:

- Renderização OpenGL 1.x (`glBegin/glEnd`) num contexto de compatibilidade 2.1.
- `Sprite`/`Text`/`Music`/`Sound` têm cache global por arquivo.
- `Road::Load` e `LoadStageState` rodam em threads com contexto GL próprio
  que **compartilha** texturas com o principal; `g_gfxMutex` (`core/Sync.h`)
  serializa tabelas de assets, upload de texturas e vetores de objetos.
- Áudio 100% OGG/WAV (o build vcpkg do `SDL2_mixer` não tem MP3; os MP3
  originais estão em `assets/descartar/` para remoção).
- Texto usa `TTF_RenderUTF8_*` (fontes + código em UTF-8, flag `/utf-8`).
- Armadilhas do `SDL_image` 2.8 corrigidas no `Sprite::Open`: pitch com padding
  (reempacota linhas) e máscaras R/B trocadas no loader JPEG (sobe bytes direto).

## Dependências

- Compilador C++17 + CMake 3.20+ (VS2022 Build Tools no Windows — já traz
  um CMake; `build-essential` + `cmake` no Linux; Xcode CLT no macOS).
- SDL2 + SDL2_image/mixer/ttf: o build **usa o que já existir** (vcpkg,
  sistema, brew) e **baixa sozinho no Windows** se faltar
  (`cmake/Dependencies.cmake`, pacotes `-devel` oficiais fixados).
  vcpkg é opcional: se `VCPKG_ROOT` existir, `run.bat`/CMake o usam.
- OpenGL do sistema (`opengl32` no Windows, Mesa no Linux, framework no macOS).

## Como buildar e rodar

```bat
:: Windows (VS2022): sem nada manual — deps vêm sozinhas
run.bat
```

Manual (qualquer SO):

```bat
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
ctest --test-dir build -C Release   :: 5 suites de teste
cd build\Release
.\RipaNaXulipa.exe
```

Linux: `sudo apt install build-essential cmake libgl1-mesa-dev libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev`
macOS: `brew install sdl2 sdl2_image sdl2_mixer sdl2_ttf`

O executável precisa da pasta `arquivos/` ao lado (o build copia
automaticamente) e, no Windows, das DLLs SDL (copiadas pelo build).

## Testes (`tests/`, framework próprio `minitest.h`, zero dependências)

| Suíte (`ctest -R`) | O que cobre | Precisa de |
|---|---|---|
| `engine` | Rect/Point/Timer (100%) | nada (puro) |
| `game` | notas, ranking top-10, coins, VerifyLevel, SplitLines | nada (fixture em tempdir) |
| `media` | abre todos os .ogg/.wav + falha graciosa | SDL+Mixer (dummy audio) |
| `assets_gl` | abre todas as imagens/textos com GL real | display (xvfb no Linux CI) |
| `input` | ciclo tecla/mouse, debounce, TEXTINPUT | SDL video dummy |
| `local/test_boot_states` | **só local** (fora do CI): instancia o Game de verdade, empilha todos os estados, roda frames, checa `glGetError` e salva screenshots `.ppm` em `local-shots/` p/ inspeção visual. Ligar com `-DRIPA_ENABLE_LOCAL_TESTS=ON` e rodar o binário com display real (janela de verdade) | GPU + display |

Cobertura (`-DRIPA_COVERAGE=ON` + lcov, job Linux do CI): cobre tudo acima
incluindo `Sprite::Open`/`Text::RemakeTexture` (via `assets_gl` sob xvfb).
Fora do alcance headless: loop do `Game`, `Render()`s e gameplay (GL de
apresentação) — cobertos por playtest com screenshots (ver histórico).

## CI e releases (GitHub Actions)

- `.github/workflows/ci.yml`: a cada push/PR, build + testes em
  Ubuntu/Windows/macOS (com cobertura no Linux).
- `.github/workflows/release.yml`: ao criar tag `v*` (ex
  `git tag v1.0.0 && git push origin v1.0.0`), compila Release nos 3 SOs,
  roda os testes, empacota (`*-windows-x64.zip` com exe+dlls+assets;
  `.tar.gz` no Linux/macOS) e publica no GitHub Release.

## Dívidas técnicas conhecidas (não afetam o gameplay atual)

- `x = *new T(...)` em vários estados: vaza o temporário (trocar por valor
  exige move-semântica em `Text`, cujo destrutor libera a textura — não mexer
  sem esse cuidado).
- `Road::segmentos` e `roadObjcts` nunca liberam os ponteiros ao sair da fase;
  segmentos apagados do vetor seguem vivos porque outros objetos guardam os
  ponteiros (não deletar ali: seria use-after-free).
- `Road::initScena` chama `AddRoadObjs` no construtor (objetos decorativos
  acumulam; sem teto).
- `LoadState`/`Road` chamam `Mix_OpenAudio`/`IMG_Init` de novo nas threads
  (herdado do original; inofensivo).
- `MessageWindow` quebra o texto a cada ~23 caracteres (por espaço; sem
  espaço próximo, corta no limite — sem crash).
