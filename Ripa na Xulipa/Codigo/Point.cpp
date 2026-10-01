
#include "Point.h"


// Calcula a distancia entre pontos
float Point::GetDistance(Point pt) {

	float distance;	
	distance = sqrt(pow((pt.x - x), 2) + pow((pt.y - y), 2));
	return distance;

}


// Calcula a distancia e angulo do ponto escolhido
void Point::SetAngle(float dist, float px, float py, double angle) {
	
//	Ajusta angulo de graus para radianos
	angle = angle * PI/180;	
//	Calcula os valores do x e y com a distancia e angulo escolhidos
	x = cos(angle) * dist;
	y = sin(angle) * dist;
	
	x += px;
	y += py;
	
}


// Calcula os pontos necessarios para se mover em linha reta com a velocidade dada
void Point::SetLineSpeed(float speed, Point pt, Point pt2) {
	
//	Calcula os pontos x e y para o movimento em linha reta
	x = ((pt2.x - pt.x)/pt.GetDistance(pt2)) * speed;
	y = ((pt2.y - pt.y)/pt.GetDistance(pt2)) * speed;
	
}


