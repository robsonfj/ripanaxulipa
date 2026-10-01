// Point.h - ponto 2D com distancia.

#ifndef __IDJ__Point__
#define __IDJ__Point__

#include <iostream>
#include <cmath>

#define PI 3.14159

class Point {

public:
	float x, y;
	
	Point() : x(0), y(0) {};
	Point(float px, float py) : x(px), y(py) {};
	
	float GetDistance(Point pt);

};

#endif /* defined(__IDJ__Point__) */
