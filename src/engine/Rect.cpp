// Rect.cpp - retangulo com centro e teste ponto-dentro.

#include "Rect.h"


// Retorna Point com os valores do centro do rect
Point Rect::GetRectCenter() const {

	Point ponto;
	
	ponto.x = x + w / 2;
	ponto.y = y + h / 2;

	return ponto;

}


bool Rect::IsInside(float px, float py) {

//	Verifica se os pontos estao dentro do rect
	if ((px >= x) && (px <= x + w)) {
		if ((py >= y) && (py <= y + h)) {
			return true;
		}
	}
	return false;

}
