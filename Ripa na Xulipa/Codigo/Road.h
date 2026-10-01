
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
	GLuint TextureID = 0;
    GameObject *obsMatrix[3][3];
	Sprite spRoad;
	float roadwidth, segheight;
    int papercount = 0;
	int pos = 2;

    void initScena();
    void initThread();

public:
    bool stopload;
    float segcount, speed;
    Timer objTimer, obsTimer, speedTimer, paperTimer, powerTimer, materialTimer;
	static std::vector<RoadSegment*> segmentos;
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
