// OptionState.h - opcoes: mudo, volume, reset de save e liga/desliga da intro.

#ifndef __Avenida_Paulista__OptionState__
#define __Avenida_Paulista__OptionState__

#include "State.h"
#include "Sprite.h"
#include "Text.h"
#include "Timer.h"
#include "ColorRect.h"
#include "Music.h"
#include "MessageWindow.h"

class OptionState : public State {

private: 
	int nrtxtselected = 0;
	Sprite bg, bg2;
	Text txMute, txPlus, txMinus, txVolume, txBack, txReset, txIntro;
	Music music;

	void Input();
	
public:
	OptionState();
	void Update(float dt);
	void Render();
	
};

#endif /* defined(__Avenida_Paulista__OptionState__) */
