// MainCharacter.cpp - jogador: corrida, pulo, powerups e placar (membros estaticos).

#include "MainCharacter.h"
#include "Game.h"
#include "StoreState.h"

MainCharacter* MainCharacter::player = NULL;
int MainCharacter::plpoints;
int MainCharacter::plstrpoints;
int MainCharacter::papers;


MainCharacter::MainCharacter(float x, float y) : sp("arquivos/img/Personagens/running.png", 5, 80) {

    SDL_Color padraoCor;
    padraoCor.a = 1;
    padraoCor.b = padraoCor.r = padraoCor.g = 20;
	
    
    jumpFX = *new Sound("arquivos/audio/efeitchos sonoricos/Haps do Salto/hap 4.ogg");
    
    powersp = *new Sprite("arquivos/img/states/stage/powerups/imagenzinhas/superpulo-img.png");
    powersp2 = *new Sprite("arquivos/img/states/stage/powerups/imagenzinhas/intangibilidade-img.png");
    powersp3 = *new Sprite("arquivos/img/states/stage/powerups/imagenzinhas/revive-3-img.png");
    
    txTime = *new Text(FONTE, 30, Text::TEXT_BLENDED, "0", padraoCor, 790, 120);
    
    lanex = 1;
	player = this;
    plpoints = 0;
    plstrpoints = 0;
    papers = 0;
	this->minPos = y;
	box.x = x - sp.GetWidth()/2;
	box.y = y - sp.GetHeight()/2;
	canDie = true;
	pulo = 700;
//	Inicializacao explicita: speedY/powerup* sem valor liam lixo da
//	pilha e jogavam o personagem para fora da tela no primeiro frame
//	(personagem "em branco" no primeiro load).
	speedY = 0;
	powerupAtivado = false;
	poweruplevel = 0;
	poweruplives = 0;

}


void MainCharacter::Update(float dt) {
    std::stringstream s;
    float xpos[3] = {100, 400, 590};
	gravity = 2500;
//	No ar segura o frame mais "aereo" da corrida (frame 5: braco
//	levantado); no chao a animacao corre normal. Sem arte nova de
//	pulo, reutiliza a pose existente do personagem.
	if (isjumping) {
		sp.SetFrame(5);
	}
	else {
		sp.Update(dt);
	}
    
	box.y += speedY * Game::GetInstance().GetDeltaTime();
	speedY += gravity * Game::GetInstance().GetDeltaTime();

    if (box.y > minPos) {
        box.y = minPos;
        isjumping = false;
        laney =  -1;
    }
    
    if (InputManager::GetInstance().KeyPress(D_KEY)) {
        if (lanex < 2) {
            lanex++;
        }
    }
    if (InputManager::GetInstance().KeyPress(A_KEY)) {
        if (lanex > 0) {
            lanex--;
        }
    }
    box.x = xpos[lanex];
	
//	Controla o pulo do personagem
    jumptime.Update(dt * 1000);
    if (jumptime.Get() > 700) {
        if (InputManager::GetInstance().KeyPress(W_KEY) && isjumping == false) {
            speedY = -pulo;
            isjumping = true;
            laney =  lanex;
            jumpFX.Play(0);
            jumptime.Restart();
        }
    }

// Controla o tempo de duracao do powerup
    if (powerupAtivado) { // algum powerup esta ativo
        if (limitTime == 0) { // pra ele passar so uma vez por esse switch
            switch (poweruplevel) {
                case 1: limitTime = 3;
                    break;
                case 2: limitTime = 6;
                    break;
                case 3: limitTime = 10;
                    break;
                case 1000:limitTime = 999999;
                    break;
            }
        }
        
        poweruptime.Update(dt);
        txTime.SetText(std::to_string((int)(limitTime-poweruptime.Get())));
        if (poweruptime.Get() >= limitTime) { // se acabar o tempo do powerup
            pulo = 700;
            SetIntang(1);
            powerupAtivado = false;
            tipoPowerUp = "";
            poweruptime.Restart();
            limitTime = 0;
        }
    }
    
    if(tipoPowerUp2 == "revive" && poweruplives>0){
        if(poweruplives > 3)
            powersp3.Open("arquivos/img/states/stage/powerups/imagenzinhas/revive-img.png");
        else{
            s<<"arquivos/img/states/stage/powerups/imagenzinhas/revive-"<<poweruplives<<"-img.png";
            powersp3.Open(s.str());
        }
        s.str("");
    }
    
}


void MainCharacter::Render() {

	sp.Render(box.x, box.y);
    
    if (powerupAtivado) {
		if (tipoPowerUp == "superpulo") {
			powersp.Render(800 - powersp.GetWidth() / 2, 50 - powersp.GetHeight() / 2);
		}
		if (tipoPowerUp == "intangibilidade") {
			powersp2.Render(800 - powersp.GetWidth() / 2, 50 - powersp.GetHeight() / 2);
		}
        txTime.Render();
    }
    
	if (tipoPowerUp2 == "revive") {
		powersp3.Render(900 - powersp2.GetWidth() / 2, 50 - powersp2.GetHeight() / 2);
	}

}


bool MainCharacter::IsDead() {

	return false;

}


void MainCharacter::NotifyCollision(GameObject& other) {
    
	if (powerupAtivado && tipoPowerUp == "superpulo" && isjumping) {
		other.jumpOver = true;
	}
	else if (IsHighJump()) {
//		Pulo alto sem superpulo tambem passa limpo (perdoa ~metade
//		superior do pulo; pulo baixo ainda colide).
		other.jumpOver = true;
	}
	else {
		other.jumpOver = false;
	}
    
    if (other.actCollision == true) {
        if (other.Is("obstaculo")) {
            if (!sp.IsTransparent() && canDie && !other.jumpOver) {
                player = NULL;
            }
            if (!canDie && tipoPowerUp2 == "revive" && !other.jumpOver && !sp.IsTransparent()) {
                if (poweruplives > -1 && poweruplives < 2) {
                    canDie = true;
                    tipoPowerUp2 = "";
                }
				else {
					poweruplives -= 1;
				}
            }
        }
        
        if (other.Is("paper")) {
            papers += 1;
            plpoints += 100;
			plstrpoints += 1 + rand() % 4;
            collisionFX.Open("arquivos/audio/efeitchos sonoricos/paper.wav");
            collisionFX.Play(0);
        }
        
        if (other.Is("material")) {
            plpoints += 100 + rand() % 100;
            plstrpoints += 30 + rand() % 30;
        }
        
        if (other.Is("buraco")) {
            if (!isjumping && canDie) {
                player = NULL;
            }
            if (!canDie && tipoPowerUp2 == "revive" && !other.jumpOver) {
                if (poweruplives < 2) {
                    canDie = true;
                    tipoPowerUp2 = "";
                }
				else {
					poweruplives -= 1;
				}
            }
        }
        
        if (other.Is("powerup1")) {
            powerupAtivado = true;
            tipoPowerUp = "superpulo";
            poweruplevel = StoreState::VerifyLevel(0);
            pulo = 1200;
            SetIntang(1);
            poweruptime.Restart();
        }
        
        if (other.Is("powerup2")) {
            powerupAtivado = true;
            tipoPowerUp = "intangibilidade";
            poweruplevel = StoreState::VerifyLevel(1);
            SetIntang(0.6);
            pulo = 700;
            poweruptime.Restart();
        }
        
        if (other.Is("powerup3")) {
            tipoPowerUp2 = "revive";
            poweruplives = StoreState::VerifyLevel(2);
            cout<<poweruplives<<std::endl;
            canDie = false;
        }
    }

}


bool MainCharacter::Is(string type) {
	
	if (type == "personagem") {
		return true;
	}	
	return false;

}