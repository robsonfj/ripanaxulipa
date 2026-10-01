// Objetos.cpp - obstaculos e coletaveis posicionados na pista.

#include "Objetos.h"
#include "MainCharacter.h"


Objetos::Objetos(string tipo, string file, int segpos, float x, float y, float scale) : spObj(file) {
	
    this->tipo = tipo;
	this->scale = scale;
	this->seg = Road::SegmentAt(segpos);
    jumpOver = false;
    
	spObj.SetScaleX(scale * seg->GetScreenRect1().w / Game::GetInstance().GetWindowHeight() / 2);
	spObj.SetScaleY(scale * seg->GetScreenRect1().w / Game::GetInstance().GetWindowHeight() / 2);
    
	box.x = pos.x = x;
	box.y = pos.y = y;
    
//  Se nao for um material
    if (file.substr(36,3) != "mat") {
        int num = std::atoi(file.substr(39,1).c_str());
        if (num == 1) {
            SFX = new Sound("arquivos/audio/efeitchos sonoricos/trashcan.wav");
        }
        if (num == 2) {
            SFX = new Sound("arquivos/audio/efeitchos sonoricos/box.wav");
        }
        if (num == 3) {
            SFX = new Sound("arquivos/audio/efeitchos sonoricos/hit.wav");
        }
        if (num == 4 || num == 5) {
            SFX = new Sound("arquivos/audio/efeitchos sonoricos/cat.wav");
        }
        if (num == 6 || num == 8) {
            SFX = new Sound("arquivos/audio/efeitchos sonoricos/Queda no Humano.ogg");
        }
        if (num == 7) {
            SFX = new Sound("arquivos/audio/efeitchos sonoricos/Queda na Barraca.ogg");
        }
    }
	else {
		SFX = new Sound("arquivos/audio/efeitchos sonoricos/material.ogg");
	}
    
//  Atribuicao da lane para os objetos
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


void Objetos::Update(float dt) {
	
	if (!seg) {
		actCollision = false;
		return;
	}
	spObj.SetScaleX(scale * seg->GetScreenRect1().w / Game::GetInstance().GetWindowHeight() / 2);
	spObj.SetScaleY(scale * seg->GetScreenRect1().w / Game::GetInstance().GetWindowHeight() / 2);

	box.x = seg->GetScreenRect1().x - spObj.GetWidth()/2 + seg->GetScreenRect1().w * pos.x;
	box.y = seg->GetScreenRect1().y - spObj.GetHeight() * pos.y;
	
//	Habilita ou desabilita a colisao de acordo com a distancia do objeto
	if (seg && seg == Road::SegmentAt(3)) {
		actCollision = true;
	}
	else {
		actCollision = false;
	}

}


void Objetos::Render() {

	spObj.Render(box.x, box.y);

}


void Objetos::NotifyCollision(GameObject& other) {
    
    if (MainCharacter::player) {
		if (MainCharacter::player->powerupAtivado && MainCharacter::player->tipoPowerUp == "superpulo" && MainCharacter::player->isjumping) {
			jumpOver = true;
		}
        else
            jumpOver = false;
        
        if (other.Is("personagem") && actCollision == true) {
            if (tipo == "paper" || tipo == "material") {
                isDead = true;
				if (SFX) {
					SFX->Play(0);
				}
            }
            else if (!MainCharacter::player->sp.IsTransparent() && !jumpOver) {
//				Pulo alto passa limpo e em silencio (sem SFX de batida).
				if (MainCharacter::player->IsHighJump()) {
					jumpOver = true;
				}
				else {
					isDead = true;
					if (SFX) {
						SFX->Play(0);
					}
				}
            }
        }
    }

}


bool Objetos::Is(string type) {
	
	if (type == tipo) {
		return true;
	}	
	return false;

}


bool Objetos::IsDead() {
	
	if (!seg || seg->IsDead() || isDead) {
		return true;
	}	
	return false;

}
