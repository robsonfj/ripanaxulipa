
#ifndef __Avenida_Paulista__PauseState__
#define __Avenida_Paulista__PauseState__

#include "State.h"
#include "Text.h"
#include "Music.h"
#include "Timer.h"
#include "ColorRect.h"

class PauseState: public State {
	int nrtxtselected = -1;
	void Input();
	Music music;
    ColorRect rectWhite;
	
public:
	Text txContinue, txExit , txOptions;
	PauseState();
	void Update(float dt);
	void Render();
	bool pauseContinue = false;
	bool pauseExit = false;
	bool paused = false;
};
#endif /* defined(__Avenida_Paulista__PauseState__) */
