// Objetos.h - obstaculos e coletaveis posicionados na pista.

#ifndef __Avenida_Paulista__Objetos__
#define __Avenida_Paulista__Objetos__

#include "Game.h"
#include "Road.h"
#include "Sound.h"

class Objetos : public GameObject {

private:
	Sprite spObj;
    Sound *SFX = NULL;
	float scale;
    string tipo;
	Point pos;
    bool isDead = false;
    
public:
    RoadSegment *seg;
    
	Objetos(string tipo, string file, int segpos, float x, float y, float scale = 1);

	void Update(float dt);
	void Render();
	bool IsDead();
	void NotifyCollision(GameObject& other);
	bool Is(string type);

};

#endif /* defined(__Avenida_Paulista__Objetos__) */
