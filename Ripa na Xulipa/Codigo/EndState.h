//
//  EndState.h
//  Avenida Paulista
//
//  Created by Robson Ferreira Jacomini on 12/06/14.
//
//
#ifndef __IDJ__EndState__
#define __IDJ__EndState__

#include "State.h"
#include "Text.h"
#include "ColorRect.h"

class EndState: public State{
    void Input();
	Text txNota;
	Text txResultado;
    Text txScore, txNscore;
    Text txHighscore, txNhighscore;
    Text txContinue, txRepeat;
    
    ColorRect rectRed;
    
public:
    EndState(bool win);
    void Update(float dt);
    void Render();
    
};

#endif