
#include "Rect.h"


// Retorna Point com os valores do centro do rect
Point Rect::GetRectCenter() const {

	Point ponto;
	
	ponto.x = x + w / 2;
	ponto.y = y + h / 2;

	return ponto;

}


// Retorna a distancia entre os centros dos rects
float Rect::GetRectCenterDist(Rect rect) {

	Point pt1, pt2;
	
//	Pega o ponto central de cada rect
	pt1 = GetRectCenter();
	pt2 = rect.GetRectCenter();
	
//	Retorna a distrancia entre os pontos
	return pt1.GetDistance(pt2);

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


// Calcula a distancia (a partir do centro) e angulo do rect escolhido
void Rect::SetAngle(float dist, float px, float py, double angle) {
	
//	Ajusta angulo em graus para radianos
	angle = angle * PI / 180;
	
//	Calcula os valores do x e y com a distancia e angulo escolhidos
	x = cos(angle) * dist;
	y = sin(angle) * dist;
	
//	Passa os novos valores corrigindo para o centro do rect
	x += px - w / 2;
	y += py - h / 2;
	
}