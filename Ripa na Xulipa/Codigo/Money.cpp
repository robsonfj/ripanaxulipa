

#include "Money.h"

Money::Money(int value) {
	padraoCor.r = padraoCor.g = padraoCor.b = 0;
	coins = Sprite::Sprite("arquivos/img/store/coins.png");
	sprintf(auxTxValue, "$ %d", value);
	this->value = *new Text(FONTE, 50, Text::TEXT_BLENDED, auxTxValue, padraoCor, 80, 50);
}

void Money::Render(float x, float y) {
	coins.Render(x, y);
	value.SetPos(x + 120, y + 20);
	value.Render();
}

void Money::SetValue(int value) {
	sprintf(auxTxValue, "$ %d", value);
    this->value.SetText(auxTxValue);
}

