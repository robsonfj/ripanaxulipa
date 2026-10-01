
#ifndef __Ripa__Sync__
#define __Ripa__Sync__

// Trava global (recursiva) para recursos compartilhados entre a main
// thread e as threads de load (Road::Load e LoadStageState):
// tabelas de assets (Sprite/Text/Music/Sound), upload de texturas
// OpenGL e vetores de objetos (objectArray, roadObjcts, segmentos).
// Regras: secoes criticas curtas; nunca segurar durante SDL_Delay ou
// SDL_WaitThread; recursiva porque AddObstacles() chama AddObject()
// com a trava ja tomada. Nao muda logica do jogo, so serializa.
#include <mutex>

inline std::recursive_mutex g_gfxMutex;

#endif /* defined(__Ripa__Sync__) */
