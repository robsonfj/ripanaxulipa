
#ifndef __IDJ__ObjPespectiva__
#define __IDJ__ObjPespectiva__

#include "GameObject.h"
#include "Road.h"
#include "Sprite.h"
#include "Sound.h"

class ObjPespectiva : public GameObject {

private:
    float scale;
    Sprite sp;
    Sound *SFX = NULL;
    Point pos;
    Rect box2;
    bool isDead = false;
    
public:
    RoadSegment *seg;
    
    ObjPespectiva(string file, int segpos, float x, float y, float scale = 1);
    
    void Update(float dt);
    void Render();
    bool IsDead();
    void NotifyCollision(GameObject& other);
    bool Is(string type);

};

#endif