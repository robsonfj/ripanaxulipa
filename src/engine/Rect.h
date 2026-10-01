// Rect.h - retangulo com centro e teste ponto-dentro.

#ifndef __IDJ__Rect__
#define __IDJ__Rect__

#include "Point.h"

class Rect {

public:
	float x, y, w, h;
	
	Rect() : x(0), y(0), w(0), h(0) {};
	Rect(float px, float py, float width, float height) : x(px), y(py), w(width), h(height) {};
	
	Point GetRectCenter() const;
	bool IsInside(float px, float py);

};

#endif /* defined(__IDJ__Rect__) */
