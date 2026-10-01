// StoreItem.cpp - item compravel da loja com niveis.

#include "StoreItem.h"


StoreItem::StoreItem(std::string file, std::string description, int value, int id, int nivel, float x, float y) {

    padraoCor.r = padraoCor.g = padraoCor.b = 0;
	background = *new Sprite("arquivos/img/states/store/itembg.png");
	item = Sprite(file);
	this->description = Sprite(description);
	intValue = value;
	snprintf(auxTxValue, sizeof(auxTxValue), "$ %d", intValue);
	this->value = *new Text(FONTE, 50, Text::TEXT_BLENDED, auxTxValue, padraoCor, 80, 50);
	this->id = id;
	this->nivel = nivel;
    
	box.x = x;
	box.y = y;
	box.w = background.GetWidth();
	box.h = background.GetHeight();
	selected = false;
	renderDescription = false;

}


void StoreItem::Render() {

	float itemX, itemY;
	
	itemX = box.x + background.GetWidth() / 2 - item.GetWidth() / 2;
	itemY = box.y + background.GetHeight() / 2 - item.GetHeight() / 2;
	
	value.SetPos(box.x + background.GetWidth() / 2 - 40, itemY + item.GetHeight() + 25);
    
    background.Render(box.x, box.y);
    item.Render(itemX, itemY);
	value.Render();
	if (renderDescription) {
		description.Render(640, 445);
	}

} 


int StoreItem::GetValue() {

	return intValue;

}


void StoreItem::Selected() {

	selected = true;
	renderDescription = true;
	background.Open("arquivos/img/states/store/selecteditembg.png");
	padraoCor.r = 223;
	padraoCor.g = 55;
	padraoCor.b = 38;
	this->value = *new Text(FONTE, 50, Text::TEXT_BLENDED, auxTxValue, padraoCor, 80, 50);

}


void StoreItem::Unselected() {

	selected = false;
	renderDescription = false;
	background.Open("arquivos/img/states/store/itembg.png");
	padraoCor.r = padraoCor.g = padraoCor.b = 0;
	this->value = *new Text(FONTE, 50, Text::TEXT_BLENDED, auxTxValue, padraoCor, 80, 50);

}


bool StoreItem::IsSelected() {

	return selected;

}

