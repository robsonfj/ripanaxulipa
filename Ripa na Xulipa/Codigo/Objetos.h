

#ifndef __Avenida_Paulista__Objetos__
#define __Avenida_Paulista__Objetos__

#include "Game.h"
#include "Road.h"

class Objetos: public GameObject{
	Sprite spObj;
	RoadSegment *seg;
	float scale;
    string tipo;
	Point pos;
    bool isDead = false;
    
public:
	Objetos(string tipo, string file, int segpos, float x, float y, float scale = 1);
	
	void Update(float dt);
	void Render();
	bool IsDead();
	
	void NotifyCollision (GameObject& other);
	bool Is(string type);
};

#endif /* defined(__Avenida_Paulista__Objetos__) */
