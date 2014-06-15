
#ifndef __IDJ__Point__
#define __IDJ__Point__

#include <iostream>
#include <cmath>

#define PI 3.14159

class Point{
public:
	float x, y;
	
	Point(): x(0), y(0){};
	Point(float px, float py) : x(px), y(py){};
	
	float GetDistance(Point pt);
	void SetAngle(float dist, float px, float py, double angle);
	void SetLineSpeed(float speed, Point pt, Point pt2);
	

	Point operator+(const Point& rhs) const {
	    return Point(x + rhs.x, y + rhs.y);
	}
	
	Point operator-(const Point& rhs) const {
	    return Point(x - rhs.x, y - rhs.y);
	}
	
	Point operator*(const float rhs) const {
	    return Point(x * rhs, y * rhs);
	}
};

#endif /* defined(__IDJ__Point__) */
