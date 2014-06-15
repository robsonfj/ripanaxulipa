

#include "Camera.h"
#include "Game.h"

GameObject* Camera::focus = NULL;
Point Camera::pos;
float Camera::speed = 0;

void Camera::Update (float dt){
//	velocidade baseado na velocidade de execucao do programa
//	se estiver demorando mais o dt fica maior e a velocidade aumenta
	speed = dt;
	
	if (focus != NULL) {
//		calcula a posicao da camera necessaria para o foco no centro do objeto
		pos.x = focus->box.x + focus->box.w/2 - Game::GetInstance().GetWindowWidth()/2;
		pos.y = focus->box.y + focus->box.h/2 - Game::GetInstance().GetWindowHeight()/2;
		
	}
	else{
//		
//		if (InputManager::GetInstance().IsKeyDown(D_KEY))
//			pos.x += 0.2;
//		
//		if (InputManager::GetInstance().IsKeyDown(A_KEY))
//			pos.x -= 0.2;
		
	}
}

void Camera::Unfollow(){
	pos.x = focus->box.x + focus->box.w / 2 - Game::GetInstance().GetWindowWidth() / 2;
	pos.y = focus->box.y + focus->box.h / 2 - Game::GetInstance().GetWindowHeight() / 2;
	focus = NULL; 
}