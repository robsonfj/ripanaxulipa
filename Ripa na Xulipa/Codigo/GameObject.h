

#ifndef __IDJ__GameObject__
#define __IDJ__GameObject__

#include <vector>
#include <queue> 

#include "Rect.h"

using std::string;

class GameObject{
	
public:
	virtual ~GameObject(){};
	virtual void Update(float dt) = 0;
	virtual void Render() = 0;
	virtual bool IsDead() = 0;
	virtual void NotifyCollision (GameObject& other) = 0;
	virtual bool Is(string type) = 0;
	bool actCollision = false;
	Rect box;
	float rotation = 0;
};

#endif /* defined(__IDJ__GameObject__) */
