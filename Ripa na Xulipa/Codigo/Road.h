

#ifndef __Avenida_Paulista__Road__
#define __Avenida_Paulista__Road__

#include "Game.h"
#include "RoadSegment.h"
#include "Objetos.h"
#include "Buildings.h"
#include "ObjPespectiva.h"
#include "Timer.h"

using std::string;
class Road{
	GLuint TextureID = 0;
	Sprite spRoad;
	float segcount, roadwidth, segheight, speed;
	Timer objTimer, obsTimer;
    int papercount = 0;
	int pos = 2;
    void initScene();
    
public:
	Road(){};
	Road(string arq, int segcount);
	~Road(){};
	
	static std::vector<RoadSegment*> segmentos;
	void AddRoadObjs(int pos);
	void AddObstacles();
	void Update(float dt);
	void Render();
};
#endif /* defined(__Avenida_Paulista__Road__) */
