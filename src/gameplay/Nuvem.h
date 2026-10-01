// Nuvem.h - nuvens do fundo com deriva horizontal.

#ifndef __IDJ__Nuvem__
#define __IDJ__Nuvem__

#include <sstream>

#include "Point.h"
#include "Sprite.h"

class Nuvem {

private:
	float speed, scale;
    std::stringstream s;
    Sprite sp;
    Point pos;
    
public:
    Nuvem(float x, float y);
    ~Nuvem() {};
    
    void Update(float dt);
    void Render();

};

#endif