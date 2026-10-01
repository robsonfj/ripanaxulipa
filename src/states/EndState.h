// EndState.h - tela de vitoria/derrota com nota e ranking.

#ifndef __IDJ__EndState__
#define __IDJ__EndState__

#include "State.h"
#include "Sprite.h"
#include "Text.h"
#include "Music.h"
#include "ColorRect.h"

class EndState : public State {

private:
    void Input();
    int nrtxtselected = 0;
	Text txNota, txScore, txNscore, txHighscore, txNhighscore, txMenu, txRepeat;
    Music endMusic;
	Sprite bg;
    
public:
    EndState(bool win);
    void Update(float dt);
    void Render();
    
};

#endif