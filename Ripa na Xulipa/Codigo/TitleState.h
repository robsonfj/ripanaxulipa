
#ifndef __IDJ__TitleState__
#define __IDJ__TitleState__

#include "LoadState.h"
#include "OptionState.h"
#include "StoreState.h"
#include "RankingState.h"
#include "Credits.h"
#include "Text.h"
#include "Music.h"
#include "Timer.h"
#include "ColorRect.h"

class TitleState : public State {

private:
	int nrtxtselected = 0;
	Sprite bg, bg2;
	Music music;
	Text txPlay, txOptions, txStore, txRanking, txExit, txCredits;

	void Input();
	
public:
	TitleState();
	void Update(float dt);
	void Render();
	
};

#endif /* defined(__IDJ__TitleState__) */
