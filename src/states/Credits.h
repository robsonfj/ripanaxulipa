// Credits.h - tela de creditos da equipe.

#ifndef __Avenida_Paulista__Credits__
#define __Avenida_Paulista__Credits__

#include "State.h"
#include "Sprite.h"
#include "Text.h"
#include "Timer.h"
#include "ColorRect.h"
#include "Music.h"
#include "MessageWindow.h"

class Credits : public State {

private:
	int nrtxtselected = 0;
	Music credMusic;
	Sprite bg, bg2;
	Text txBack, txTitle, txArt, txArt1, txArt2, txDev, txDev1, txDev2, txMus, txMus1;

	void Input();
    
public:
	Credits();
	void Update(float dt);
	void Render();

};

#endif /* defined(__Avenida_Paulista__Credits__) */
