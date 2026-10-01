// Money.h - HUD de moedas exibido na loja.

#ifndef __IDJ__Money__
#define __IDJ__Money__

#include "Sprite.h"
#include "Text.h"

class Money {

private:
	char auxTxValue[32];
	SDL_Color padraoCor;
	Sprite coins;
	Text value;

public:
	Money() {}
	Money(int value);
	void SetValue(int value);
	void Render(float x, float y);

};

#endif /* defined(__IDJ__Money__) */
