
#include "PowerUp.h"


PowerUp::PowerUp(string file, int segpos, float x, float y, float scale) : spObj(file) {
	
    this->tipo = file.substr(35, 8);
	this->scale = scale;
	this->seg = Road::segmentos[segpos];
    jumpOver = true;
	
	spObj.SetScaleX(scale * seg->GetScreenRect1().w / Game::GetInstance().GetWindowHeight() / 2);
	spObj.SetScaleY(scale * seg->GetScreenRect1().w / Game::GetInstance().GetWindowHeight() / 2);
    
	box.x = pos.x = x;
	box.y = pos.y = y;
    
    SFX = new Sound("arquivos/audio/Efeitchos Sonoricos/powerup.ogg");
    
	//  Atribuicao da lane para os PowerUp
	if (x >= 0.2 && x <= 0.4) {
		lanex = 0;
	}
	if (x >= 0.6 && x <= 0.7) {
		lanex = 1;
	}
	if (x >= 0.8 && x <= 0.9) {
		lanex = 2;
	}
    if (y >= 2 && y <= 4) {
		if (lanex == 0) {
			laney = 0;
		}
		if (lanex == 1) {
			laney = 1;
		}
		if (lanex == 2) {
			laney = 2;
		}
        lanex = -1;
    }

}


void PowerUp::Update(float dt) {
	
	spObj.SetScaleX(scale * seg->GetScreenRect1().w/Game::GetInstance().GetWindowHeight()/2);
	spObj.SetScaleY(scale * seg->GetScreenRect1().w/Game::GetInstance().GetWindowHeight()/2);
	
	box.x = seg->GetScreenRect1().x - spObj.GetWidth()/2 + seg->GetScreenRect1().w*pos.x;
	box.y = seg->GetScreenRect1().y - spObj.GetHeight() * pos.y;
	
	//	Habilita ou desabilita a colisao de acordo com a distancia do objeto
	if (seg == Road::segmentos[3]) {
		actCollision = true;
	}
	else {
		actCollision = false;
	}
	
}


void PowerUp::Render() {

	spObj.Render(box.x, box.y);

}


void PowerUp::NotifyCollision(GameObject& other) {
	
    if (other.Is("personagem") && actCollision == true) {
        isDead = true;
		if (SFX) {
			SFX->Play(0);
		}
    }

}


bool PowerUp::Is(string type) {
	
	if (type == tipo) {
		return true;
	}	
	return false;

}


bool PowerUp::IsDead() {
	
	if (seg->IsDead() || isDead) {
		return true;
	}	
	return false;
	
}