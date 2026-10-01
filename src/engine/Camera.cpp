// Camera.cpp - camera fixa (pos 0,0; follow removido por falta de uso).

#include "Camera.h"
#include "Game.h"

Point Camera::pos;


void Camera::Update (float dt) {

//	Por enquanto a camera e fixa (pos sempre 0,0).
//	(Follow/Unfollow/speed removidos: nunca usados.)
	(void)dt;
	pos.x = 0;
	pos.y = 0;

}
