//
//  Cachorro.h
//  Avenida Paulista
//
//  Created by Robson Ferreira Jacomini on 12/06/14.
//
//

#ifndef __Avenida_Paulista__Cachorro__
#define __Avenida_Paulista__Cachorro__

#include "GameObject.h"
#include "Sprite.h"
#include "Camera.h"
#include "Timer.h"
#include "Road.h"

class Cachorro: public GameObject{
    Sprite sp;
    float scale;
    int i = 3;
    Timer timer;
    
public:
    Cachorro(float x, float y, float scale = 1);
    ~Cachorro(){};
    
    void Update(float dt);
    void Render();
    bool IsDead();
    void NotifyCollision (GameObject& other){};
    bool Is(string type);
};

#endif