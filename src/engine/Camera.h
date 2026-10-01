// Camera.h - camera fixa (pos 0,0; follow removido por falta de uso).

#ifndef __IDJ__Camera__
#define __IDJ__Camera__

#include "GameObject.h"
#include "InputManager.h"

class Camera {

public:
	static Point pos;
	static void Update (float dt);
	
};

#endif /* defined(__IDJ__Camera__) */
