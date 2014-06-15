//
//  ObjPespectiva.h
//  Avenida Paulista
//
//  Created by Robson Ferreira Jacomini on 14/06/14.
//
//

#ifndef __IDJ__ObjPespectiva__
#define __IDJ__ObjPespectiva__

#include "GameObject.h"
#include "Road.h"
#include "Sprite.h"

class ObjPespectiva: public GameObject{
    Sprite sp;
    RoadSegment *seg;
    float scale;
    Point pos;
    Rect box2;
    bool isDead = false;
    
public:
    ObjPespectiva(string file, int segpos, float x, float y, float scale = 1);
    
    void Update(float dt);
    void Render();
    bool IsDead();
    
    void NotifyCollision (GameObject& other){};
    bool Is(string type);
};

#endif