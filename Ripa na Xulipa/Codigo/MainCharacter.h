

#ifndef __Avenida_Paulista__MainCharacter__
#define __Avenida_Paulista__MainCharacter__

#include "GameObject.h"
#include "Sprite.h"
#include "Camera.h"
#include "Timer.h"

class MainCharacter: public GameObject{
	Sprite sp;
    int charpos;
	int maxPos, minPos;
	bool isjumping = false;
	Timer jumptime;
	
public:
	MainCharacter(float x, float y);
	~MainCharacter(){};
    
	static int papers;
    static int plpoints;//pontos do jogador (papeis coletados)
    static int plstrpoints;//pontos para a loja (materiais escolares)
	static MainCharacter *player;
	void Update(float dt);
	void Render();
	bool IsDead();
	void NotifyCollision (GameObject& other);
	bool Is(string type);
};

#endif /* defined(__Avenida_Paulista__MainCharacter__) */
