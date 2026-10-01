
#ifndef __Ripa_na_Xulipa__InitState__
#define __Ripa_na_Xulipa__InitState__

#include "Game.h"
#include "TitleState.h"
#include "Timer.h"

class InitState : public State {

private:
    void Input();
    void GoToMenu();
	Sprite logo;
    Music logoMusic;
    Timer timer, enterTimer;
	
public:
    InitState();
    void Update(float dt);
    void Render();
    
};

#endif /* defined(__Ripa_na_Xulipa__InitState__) */
