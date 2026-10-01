// LoadState.h - tela de load: constroi a fase em thread com progresso.

#ifndef __Ripa_na_Xulipa__LoadState__
#define __Ripa_na_Xulipa__LoadState__

#include <SDL_thread.h>
#include "State.h"
#include "StageState.h"
#include "Sprite.h"
#include "Music.h"
#include "Text.h"

class LoadState : public State {

private: 
    SDL_Thread *thread = nullptr;

public:
	bool sair = false, loaded = false;
    StageState *stageState = nullptr;
    static int percent;
	Sprite loadsp, bg, bg2;
    Music loadMusic;
    Text txPercent, txPlay, txBack;
	
	LoadState();
    ~LoadState() { DiscardLoad(); };
	
	void ThreadLoad();
	void Input();
	void Update(float dt);
	void Render();

//	Retira o stage pronto da thread de load (para a StoryState usar o
//	resultado sem passar pela tela de load). Faz detach da thread.
	StageState* ClaimStage();
//	Abandona o load em background (ao sair sem jogar): espera a thread
//	terminar e libera tudo com seguranca.
	void DiscardLoad();
	
};

#endif /* defined(__Ripa_na_Xulipa__LoadState__) */
