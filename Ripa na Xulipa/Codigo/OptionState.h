

#ifndef __Avenida_Paulista__OptionState__
#define __Avenida_Paulista__OptionState__

#include "State.h"
#include "Sprite.h"
#include "Text.h"
#include "Timer.h"
#include "ColorRect.h"
#include "Music.h"
#include "MessageWindow.h"

class OptionState: public State{
	void Input();
	Sprite bg;
	ColorRect rectRed, rectOrange;
	Text txMute, txPlus, txMinus, txVolume, txBack, txReset;
	Music music;
	
public:
	OptionState();
	void Update(float dt);
	void Render();
	
};
#endif /* defined(__Avenida_Paulista__OptionState__) */
