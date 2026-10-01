
#ifndef __IDJ__GameObject__
#define __IDJ__GameObject__

#include <vector>
#include <queue> 
#include <string>

#include "Rect.h"

using std::string;

class GameObject {
	
public:
	bool actCollision = false, jumpOver = false;
	int lanex = -1, laney = -2;
	Rect box;

	virtual ~GameObject() {};
	virtual void Update(float dt) = 0;
	virtual void Render() = 0;
	virtual bool IsDead() = 0;
	virtual void NotifyCollision(GameObject& other) = 0;
	virtual bool Is(string type) = 0;

};

#endif /* defined(__IDJ__GameObject__) */
