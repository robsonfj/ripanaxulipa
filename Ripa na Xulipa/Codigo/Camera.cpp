
#include "Camera.h"
#include "Game.h"

GameObject* Camera::focus = NULL;
Point Camera::pos;
float Camera::speed = 0;


void Camera::Update (float dt) {

//	Velocidade baseada na velocidade de execucao do programa
//	Se estiver demorando, o dt fica maior e a velocidade aumenta
	speed = dt;
	
	if (focus != NULL) {
//		Calcula a posicao da camera necessaria para o foco no centro do objeto
		pos.x = focus->box.x + focus->box.w / 2 - Game::GetInstance().GetWindowWidth() / 2;
		//pos.y = focus->box.y + focus->box.h / 2 - Game::GetInstance().GetWindowHeight() / 2;
	}

}


void Camera::Unfollow() {

	pos.x = focus->box.x + focus->box.w / 2 - Game::GetInstance().GetWindowWidth() / 2;
	pos.y = focus->box.y + focus->box.h / 2 - Game::GetInstance().GetWindowHeight() / 2;
	focus = NULL; 

}