

#ifndef __Avenida_Paulista__RoadSegment__
#define __Avenida_Paulista__RoadSegment__

#include "Sprite.h"
#include "GameObject.h"
#include "Camera.h"
#include "InputManager.h"

class RoadSegment{
	Rect screenRect1, screenRect2;
	float x_world, y_world;
	float z_worldpt1, z_worldpt2;
	float speed, roadWidth;
	
public:
	RoadSegment(float z_worldpt1, float z_worldpt2, float speed, float roadWidth);
	~RoadSegment(){};
	
	void Update(float dt);
	void Render(Sprite spRoad);
	bool IsDead();
	Rect GetScreenRect1(){return screenRect1;};
	Rect GetScreenRect2(){return screenRect2;};
	float GetZ_World1(){return z_worldpt1;};
	float GetZ_World2(){return z_worldpt2;};
	void NotifyCollision (GameObject& other){};
	bool Is(string type){return false;};
};

#endif /* defined(__Avenida_Paulista__RoadSegment__) */
