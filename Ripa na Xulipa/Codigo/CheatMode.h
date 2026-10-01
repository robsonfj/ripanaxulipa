
#ifndef __IDJ__CheatMode__
#define __IDJ__CheatMode__

#include "State.h"
#include "Game.h"
#include "MainCharacter.h"
#include "ColorRect.h"
#include "Text.h"


class CheatMode {

private:
    string cheat;
	ColorRect rectwhite, rectred, rectShadow;
	Text txInput;
	
public:
	bool cheatModeOn = false;
	
    CheatMode();
    ~CheatMode() {};
    
    void Update();
	void Render();
	
};

#endif