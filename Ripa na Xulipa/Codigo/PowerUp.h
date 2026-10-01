

#ifndef __Ripa_na_Xulipa__PowerUp__
#define __Ripa_na_Xulipa__PowerUp__

#include "GameObject.h"
#include "Sprite.h"
#include "Sound.h"
#include "Road.h"

class PowerUp : public GameObject {

private:
	Sprite spObj;
    Sound *SFX = NULL;
	float scale;
    string tipo;
	Point pos;
    bool isDead = false;
    
public:
    RoadSegment *seg;
    
	PowerUp(string file, int segpos, float x, float y, float scale = 1);
	
	void Update(float dt);
	void Render();
	bool IsDead();
	void NotifyCollision(GameObject& other);
	bool Is(string type);

};

#endif /* defined(__Ripa_na_Xulipa__PowerUp__) */
