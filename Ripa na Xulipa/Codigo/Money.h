

#ifndef __IDJ__Money__
#define __IDJ__Money__

#include "Sprite.h"
#include "Text.h"

class Money{
	Sprite coins;
	Text value;
	char auxTxValue[10];
	SDL_Color padraoCor;
public:
	Money() {}
	Money(int value);
	void SetValue(int value);
	void Render(float x, float y);
};

#endif /* defined(__IDJ__Money__) */
