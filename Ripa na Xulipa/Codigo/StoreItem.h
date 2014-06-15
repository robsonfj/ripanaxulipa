

#ifndef __IDJ__StoreItem__
#define __IDJ__StoreItem__

#include "Sprite.h"
#include "Text.h"

class StoreItem {
	Sprite background;
	Sprite item;
	Text value;
	char auxTxValue[10];
	int intValue;
	SDL_Color padraoCor;
	bool selected;
public:
    StoreItem(){};
	StoreItem(std::string file, int value, float x, float y);
	void SetItem(std::string file);
	void SetValue(int value);
	void SetPosition(float x, float y);
	int GetValue();
	void Render();
	void Selected();
	void Unselected();
	bool IsSelected();
	Rect box;
	bool locked;
};

#endif /* defined(__IDJ__StoreItem__) */
