
#ifndef __IDJ__StoreItem__
#define __IDJ__StoreItem__

#include "Sprite.h"
#include "Text.h"

class StoreItem {

private:
	Sprite background, item, description;
	Text value;
	char auxTxValue[10];
	int intValue, id, nivel;
	SDL_Color padraoCor;
	bool selected;

public:
	Rect box;
	bool locked;
	bool renderDescription;

    StoreItem() {};
	StoreItem(std::string file, std::string description, int value, int id, int nivel, float x, float y);

	void SetItem(std::string file);
	void SetDescription(std::string file);
	void SetValue(int value);
	void SetPosition(float x, float y);
	int GetValue();
	int GetId();
	int GetNivel();
	void Render();
	void Selected();
	void Unselected();
	bool IsSelected();

};

#endif /* defined(__IDJ__StoreItem__) */
