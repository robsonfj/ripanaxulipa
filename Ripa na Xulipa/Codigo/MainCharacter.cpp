
#include "MainCharacter.h"
#include "Game.h"

MainCharacter* MainCharacter::player = NULL;
int MainCharacter::plpoints;
int MainCharacter::plstrpoints;
int MainCharacter::papers;

MainCharacter::MainCharacter(float x, float y): sp("arquivos/img/runningback.png", 8, 50){
	sp.SetScaleX(3);
	sp.SetScaleY(3);
    
    charpos = 1;
	player = this;
    plpoints = 0;
    plstrpoints = 0;
    papers = 0;
	this->maxPos = 280;
	this->minPos = y;
	box.x = x - sp.GetWidth()/2;
	box.y = y - sp.GetHeight()/2;
}

void MainCharacter::Update(float dt) {
    float xpos[3] = {180, 440, 630};
    
	sp.Update(dt);
    
    if (InputManager::GetInstance().KeyPress(D_KEY)){
        if (charpos < 2) {
            charpos++;
        }
    }
    if (InputManager::GetInstance().KeyPress(A_KEY)){
        if (charpos > 0) {
            charpos--;
        }
    }
    box.x = xpos[charpos];
    
//	conta o tempo para o pesonagem ser permitido pular a cada 1 segundo
	jumptime.Update(dt*1000);
	if (jumptime.Get() > 600) {
		if (InputManager::GetInstance().KeyPress(SPACE_KEY)) {
			isjumping = true;
			jumptime.Restart();
		}
	}
	
//	controla o pulo do personagem
	if (isjumping) {
		if (box.y > maxPos) {
            box.y -= 800* Game::GetInstance().GetDeltaTime();
		}
		else
			isjumping = false;
	}
	else{
		if (box.y < minPos) {
			box.y += 800* Game::GetInstance().GetDeltaTime();
        }
        else{
			box.y = minPos;
            sp.SetScaleX(3);
            sp.SetScaleY(3);
        }
	}
	
}

void MainCharacter::Render(){
	sp.Render(box.x, box.y);
	
}

bool MainCharacter::IsDead(){

	return false;
}

void MainCharacter::NotifyCollision (GameObject& other){

	if (other.Is("obstaculo") && other.actCollision == true) {
        if (!sp.IsTransparent())
            player = NULL;
	}
    
	if (other.Is("paper") && other.actCollision == true) {
		papers += 1;
        plpoints += 100;
    }
    
    if (other.Is("buraco") && other.actCollision == true) {
        if (!isjumping)
            player = NULL;
    }
}

bool MainCharacter::Is(string type){
	
	if (type == "personagem")
		return true;
	
	return false;
}


