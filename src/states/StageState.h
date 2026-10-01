// StageState.h - gameplay principal: pista, jogador, HUD, pausa e fim.

#ifndef __IDJ__StageState__
#define __IDJ__StageState__

#include <sstream>

#include "State.h"
#include "Sprite.h"
#include "Road.h"
#include "Text.h"
#include "Music.h"
#include "Sound.h"
#include "PauseState.h"
#include "EndState.h"
#include "MainCharacter.h"
#include "Cachorro.h"
#include "Timer.h"
#include "Nuvem.h"

class StageState : public State {

private:
	Sprite bg, bg2, paper, coin;
	MainCharacter *personagem;
    Cachorro *cachorro;
	Road road;
	Text txpoints, txcoins, txpause, txpapers;
    Music mainmusic, block1, block2, block3;
	PauseState pause;
	std::vector<Nuvem*> nuvens;

	void Input();
	void LoadMusic();
    bool playingB1, playingB2;
    void PlayBlocos();
    
public:
	 Sound cachorroFX;
	
	StageState();
	~StageState();
	
	void Update(float dt);
	void Render();
	void Pause();
	
};
	
#endif /* defined(__IDJ__StageState__) */
