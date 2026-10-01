
#ifndef __Avenida_Paulista__RankingState__
#define __Avenida_Paulista__RankingState__

#include "State.h"
#include "Sprite.h"
#include "Text.h"
#include "Timer.h"
#include "ColorRect.h"
#include "Music.h"
#include "MessageWindow.h"

class RankingState : public State {

private:
	int nrtxtselected = 0, tam = 0;
	Sprite bg, bg2;
	Text txBack, txRanking[10], txTitle;
	Music music;
	struct ranking scores[10];

	void Input();

public:
	RankingState();
	void Update(float dt);
	void Render();

};

#endif /* defined(__Avenida_Paulista__RankingState__) */
