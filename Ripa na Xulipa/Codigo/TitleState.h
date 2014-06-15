
#ifndef __IDJ__TitleState__
#define __IDJ__TitleState__

#include "StageState.h"
#include "OptionState.h"
#include "StoreState.h"
#include "Text.h"
#include "Music.h"
#include "Timer.h"
#include "ColorRect.h"


class TitleState: public State{
	int nrtxtselected = -1;
	void Input();
	Sprite bg;
	Music music;
    ColorRect rectRed, rectorange;
	Text txPlay, txOptions, txStore, txExit;
	
public:
	TitleState();
	void Update(float dt);
	void Render();
	
};

#endif /* defined(__IDJ__TitleState__) */
