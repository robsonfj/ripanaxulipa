
#ifndef __IDJ__StillAnimation__
#define __IDJ__StillAnimation__

#include "GameObject.h"
#include "Camera.h"
#include "Sprite.h"
#include "Timer.h"

using std::string;

class StillAnimation: public GameObject{
	Timer endTimer;
	Sprite sp;
	float timeLimit;
	bool oneTimeOnly;
	
public:
	StillAnimation(float x, float y, float rotation, Sprite sprite, float timeLimit, bool ends);
	
	void Update(float dt);
	void Render();
	bool IsDead();
	void NotifyCollision (GameObject& other){};
	bool Is(string type);
	
	
};

#endif /* defined(__IDJ__StillAnimation__) */
