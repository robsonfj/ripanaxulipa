
#ifndef __IDJ__Camera__
#define __IDJ__Camera__

#include "GameObject.h"
#include "InputManager.h"

class Camera {

private:
	static GameObject* focus;
	
public:
	static Point pos;
	static float speed;
	static void Follow(GameObject *newFocus) { focus = newFocus; };
	static void Unfollow();
	static void Update (float dt);
	
};

#endif /* defined(__IDJ__Camera__) */
