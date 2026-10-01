// Point.cpp - ponto 2D com distancia.

#include "Point.h"


// Calcula a distancia entre pontos
float Point::GetDistance(Point pt) {

	float distance;	
	distance = sqrt(pow((pt.x - x), 2) + pow((pt.y - y), 2));
	return distance;

}
