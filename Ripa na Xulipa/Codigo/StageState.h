

#ifndef __IDJ__StageState__
#define __IDJ__StageState__
#include <sstream>

#include "State.h"
#include "Sprite.h"
#include "Road.h"
#include "Text.h"
#include "Music.h"
#include "PauseState.h"
#include "EndState.h"
#include "MainCharacter.h"
#include "Cachorro.h"
#include "Timer.h"
#include "Nuvem.h"

class StageState: public State{
	void Input();
	Sprite bg, bg2, paper;
	MainCharacter *personagem;
    Cachorro *cachorro;
	Road road;
	Text txpoints, txPause, txpapers;
	Music mainMusic;
	PauseState pause;
	std::vector<Nuvem*> nuvens;
    
public:
	StageState();
	~StageState();
	
	void Update(float dt);
	void Render();
	void Pause();
	
};
	
#endif /* defined(__IDJ__StageState__) */
