

#include "StoreItem.h"

StoreItem::StoreItem(std::string file, int value, float x, float y) {
    padraoCor.r = padraoCor.g = padraoCor.b = 0;
	background = *new Sprite("arquivos/img/store/itembg.png");
	item = Sprite::Sprite(file);
	intValue = value;
	sprintf(auxTxValue, "$ %d", intValue);
	this->value = *new Text(FONTE, 50, Text::TEXT_BLENDED, auxTxValue, padraoCor, 80, 50);
    
	box.x = x;
	box.y = y;
	box.w = background.GetWidth();
	box.h = background.GetHeight();
	selected = true;
}

void StoreItem::Render() {
	float itemX, itemY;
	
	itemX = box.x + background.GetWidth() / 2 - item.GetWidth() / 2;
	itemY = box.y + background.GetHeight() / 2 - item.GetHeight() / 2;
	
	value.SetPos(box.x + background.GetWidth() / 2 - 40, itemY + item.GetHeight() + 25);
    
    background.Render(box.x, box.y);
    item.Render(itemX, itemY);
	value.Render();
}

void StoreItem::SetItem(std::string file) {
	item = Sprite::Sprite(file);
}

void StoreItem::SetValue(int value) {
	sprintf(auxTxValue, "$ %d", value);
	this->value = *new Text(FONTE, 50, Text::TEXT_BLENDED, auxTxValue, padraoCor, 80, 50);
}

void StoreItem::SetPosition(float x, float y) {
	box.x = x;
	box.y = y;
	box.w = background.GetWidth();
	box.h = background.GetHeight();
}

int StoreItem::GetValue() {
	return intValue;
}

void StoreItem::Selected() {
	selected = true;
	background.Open("arquivos/img/store/selecteditembg.png");
	padraoCor.r = padraoCor.g = padraoCor.b = 50;
	this->value = *new Text(FONTE, 50, Text::TEXT_BLENDED, auxTxValue, padraoCor, 80, 50);

}

void StoreItem::Unselected() {
	selected = false;
	background.Open("arquivos/img/store/itembg.png");
	padraoCor.r = padraoCor.g = padraoCor.b = 0;
	this->value = *new Text(FONTE, 50, Text::TEXT_BLENDED, auxTxValue, padraoCor, 80, 50);

}

bool StoreItem::IsSelected() {
	return selected;
}

