//
//  Nuvem.h
//  Avenida Paulista
//
//  Created by Robson Ferreira Jacomini on 14/06/14.
//
//

#ifndef __IDJ__Nuvem__
#define __IDJ__Nuvem__

#include <sstream>

#include "Point.h"
#include "Sprite.h"

class Nuvem{
    std::stringstream s;
    Sprite sp;
    Point pos;
    float speed;
    float scale;
    
public:
    Nuvem(float x, float y);
    ~Nuvem(){};
    
    void Update(float dt);
    void Render();
};

#endif