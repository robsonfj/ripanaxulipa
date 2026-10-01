// MainCharacter.h - jogador: corrida, pulo, powerups e placar (membros estaticos).

#ifndef __Avenida_Paulista__MainCharacter__
#define __Avenida_Paulista__MainCharacter__

#include "GameObject.h"
#include "Sprite.h"
#include "Sound.h"
#include "Camera.h"
#include "Timer.h"
#include "Text.h"

class MainCharacter : public GameObject {

private:
	Sprite powersp, powersp2, powersp3;
	int minPos, limitTime = 0;
	Timer jumptime, poweruptime;
	float speedY, gravity;
    Sound jumpFX, collisionFX;
    Text txTime;
    
public:
    Sprite sp;
    std::string tipoPowerUp, tipoPowerUp2;
    bool powerupAtivado, canDie, isjumping = false;;
    int pulo, poweruplevel, poweruplives;
	static int papers;
	static int plpoints; // pontos do jogador (papeis coletados)
	static int plstrpoints; // pontos para a loja (materiais escolares)
	static MainCharacter *player;
    
	MainCharacter(float x, float y);
	~MainCharacter() {};

    void SetIntang(float alpha) {sp.SetAlpha(alpha);};
    // Pulo alto (subiu 45px+ dos 98px max): passa limpo por obstaculos.
    bool IsHighJump() const { return isjumping && (minPos - box.y) > 45.0f; };
    void Update(float dt);
	void Render();
	bool IsDead();
	void NotifyCollision(GameObject& other);
	bool Is(string type);

};

#endif /* defined(__Avenida_Paulista__MainCharacter__) */
