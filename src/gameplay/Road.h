// Road.h - pista em pseudo-3D com spawner de obstaculos (usa thread propria).

#ifndef __Avenida_Paulista__Road__
#define __Avenida_Paulista__Road__

#include "Game.h"
#include "Buildings.h"
#include "Objetos.h"
#include "ObjPespectiva.h"
#include "PowerUp.h"
#include "Timer.h"
#include "RoadSegment.h"

using std::string;

class Road {

private:
	GameObject *obsMatrix[3][3];
	Sprite spRoad;
	float roadwidth, segheight;
    int papercount = 0;
	int pos = 2;
	SDL_Thread *loadThread = nullptr;

    void initScena();
    void initThread();

public:
    bool stopload;
    float segcount, speed;
    void StopLoader(); // para a thread de load (chamar antes de destruir os vetores)
    Timer objTimer, obsTimer, speedTimer, paperTimer, powerTimer, materialTimer;
	static std::vector<RoadSegment*> segmentos;
//	Acesso seguro: clampa OOB, retorna nullptr se vazio.
	static RoadSegment* SegmentAt(int pos);
	std::vector<GameObject*> roadObjcts;
	std::vector<string> used;
    
	Road() {};
	Road(string arq, int segcount);
	~Road();
	
	void AddRoadObjs(int pos);
	void AddObstacles();
	void Update(float dt);
	void Render();

};

#endif /* defined(__Avenida_Paulista__Road__) */
