
#ifndef __Avenida_Paulista__Buildings__
#define __Avenida_Paulista__Buildings__

#include "RoadSegment.h"
#include "Game.h"

class Buildings : public GameObject {

private:
	Sprite sp1, sp2;
	bool isMirror;
    float scale;
	RoadSegment *seg1, *seg2;
	Rect box1, box2;
	
public:
	Buildings(string pred1, string pred2, int segpos1, int segpos2, bool isMirror = false, float scale = 1);
	
	void Update(float dt);
	void Render();
	bool IsDead();
	
	void NotifyCollision(GameObject& other) {};
	bool Is(string type);

};

#endif /* defined(__Avenida_Paulista__Buildings__) */
