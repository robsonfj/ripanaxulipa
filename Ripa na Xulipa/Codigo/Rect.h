
#ifndef __IDJ__Rect__
#define __IDJ__Rect__

#include "Point.h"

class Rect {

public:
	float x, y, w, h;
	
	Rect() : x(0), y(0), w(0), h(0) {};
	Rect(float px, float py, float width, float height) : x(px), y(py), w(width), h(height) {};
	
	Point GetRectCenter() const;
	float GetRectCenterX() { return x + w / 2; };
	float GetRectCenterY() { return y + h / 2; };
	float GetRectCenterDist(Rect rect);
	void  SetAngle(float dist, float px, float py, double angle);
	void Soma(float px, float py) { x += px; y += py; };
	bool IsInside(float px, float py);
	
	Rect operator+(const Point& rhs) const {
	    return Rect(x + rhs.x, y + rhs.y, w, h);
	}
	
	Rect operator-(const Point& rhs) const {
	    return Rect(x - rhs.x, y - rhs.y, w, h);
	}

};

#endif /* defined(__IDJ__Rect__) */
