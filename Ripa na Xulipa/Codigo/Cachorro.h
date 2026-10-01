
#ifndef __Avenida_Paulista__Cachorro__
#define __Avenida_Paulista__Cachorro__

#include "GameObject.h"
#include "Sprite.h"
#include "Camera.h"
#include "Timer.h"

class Cachorro : public GameObject {

private:
	int i = 7;
	float scale;
	Sprite sp;
    Timer timer;
    
public:
    Cachorro(float x, float y, float scale = 1);
    ~Cachorro() {};
    void Update(float dt);
    void Render();
    bool IsDead();
    void NotifyCollision(GameObject& other) {};
    bool Is(string type);

};

#endif