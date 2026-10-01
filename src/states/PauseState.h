// PauseState.h - pausa do jogo com cheat mode embutido.

#ifndef __Avenida_Paulista__PauseState__
#define __Avenida_Paulista__PauseState__

#include "State.h"
#include "Text.h"
#include "Music.h"
#include "Timer.h"
#include "ColorRect.h"
#include "CheatMode.h"

class PauseState : public State {

private:
	int nrtxtselected = 0;
	void Input();
    Sprite bg;
	Music music;
    CheatMode cheats;
	
public:
	bool pauseContinue = false;
	bool pauseExit = false;
	bool paused = false;
	Text txContinue, txExit , txOptions;

	PauseState();
	void Update(float dt);
	void Render();

};

#endif /* defined(__Avenida_Paulista__PauseState__) */
