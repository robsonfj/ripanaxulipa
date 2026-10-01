
#include "CheatMode.h"


CheatMode::CheatMode() : rectwhite(200, 200, 342, 30), rectShadow(210, 210, 350, 40), rectred(200, 200, 350, 40) {
	
	SDL_Color padraoCor;
    padraoCor.a = 1;
	padraoCor.r = padraoCor.g = padraoCor.b = 20;
	txInput = *new Text(FONTE, 30, Text::TEXT_BLENDED," ", padraoCor, 100, 200);
	
}


void CheatMode::Update() {
	
	if (InputManager::GetInstance().inputTexto.str() != "") {
		txInput.SetText(InputManager::GetInstance().inputTexto.str());
	}

	if (InputManager::GetInstance().KeyPress(ENTER_KEY)) {
		cheat = InputManager::GetInstance().Text();
		cheatModeOn = false;
		txInput.SetText(" ");
		SDL_StopTextInput();

//		MOSTRA FPS
		if (cheat == "showfps") {
			if (Game::GetInstance().showFPS) {
				Game::GetInstance().showFPS = false;
			}
			else {
				Game::GetInstance().showFPS = true;
			}
		}

//      ATIVA SUPERPULO
		if (cheat == "superpulo on") {
			MainCharacter::player->pulo = 1200;
            MainCharacter::player->powerupAtivado = true;
            MainCharacter::player->tipoPowerUp = "superpulo";
            MainCharacter::player->poweruplevel = 1000;
		}
//      DESATIVA SUPERPULO
		if (cheat == "superpulo off") {
			MainCharacter::player->pulo = 700;
            MainCharacter::player->powerupAtivado = false;
            MainCharacter::player->tipoPowerUp = "";
            MainCharacter::player->poweruplevel = 0;
		}
//      ATIVA INTANGIBILIDADE
		if (cheat == "intang on") {
			MainCharacter::player->SetIntang(0.6);
            MainCharacter::player->powerupAtivado = true;
            MainCharacter::player->tipoPowerUp = "intangibilidade";
            MainCharacter::player->poweruplevel = 1000;
		}
//      DESATIVA INTANGIBILIDADE
		if (cheat == "intang off") {
            MainCharacter::player->SetIntang(1);
            MainCharacter::player->powerupAtivado = false;
            MainCharacter::player->tipoPowerUp = "";
            MainCharacter::player->poweruplevel = 0;
		}
//      ATIVA REVIVE
		if (cheat == "revive on") {
			MainCharacter::player->canDie = false;
            MainCharacter::player->tipoPowerUp2 = "revive";
            MainCharacter::player->poweruplives = -1;
		}
//      DESATIVA REVIVE
        if (cheat == "revive off") {
            MainCharacter::player->canDie = true;
            MainCharacter::player->tipoPowerUp2 = "";
            MainCharacter::player->poweruplives = 0;
        }
//      GANHA 1000 DINHEIRO NA STORE
		if (cheat == "klapaucius") {
			MainCharacter::plstrpoints = 1000;
		}
//      GANHA O JOGO COM SS
		if (cheat == "wingame-SS") {
			Game::GetCurrentState().deletedpapercount = 100;
			MainCharacter::papers = 100;
			MainCharacter::plstrpoints = 100;
			MainCharacter::plpoints = 10000;
		}
//      GANHA O JOGO COM MS
		if (cheat == "wingame-MS") {
			Game::GetCurrentState().deletedpapercount = 100;
			MainCharacter::papers = 70;
			MainCharacter::plstrpoints = 70;
			MainCharacter::plpoints = 7000;
		}
//      GANHA O JOGO COM MM
		if (cheat == "wingame-MM") {
			Game::GetCurrentState().deletedpapercount = 100;
			MainCharacter::papers = 50;
			MainCharacter::plstrpoints = 50;
			MainCharacter::plpoints = 5000;
		}
//      GANHA O JOGO COM MI
		if (cheat == "wingame-MI") {
			Game::GetCurrentState().deletedpapercount = 100;
			MainCharacter::papers = 30;
			MainCharacter::plstrpoints = 30;
			MainCharacter::plpoints = 3000;
		}
//      GANHA O JOGO COM II
		if (cheat == "wingame-II") {
			Game::GetCurrentState().deletedpapercount = 100;
			MainCharacter::papers = 1;
			MainCharacter::plstrpoints = 1;
			MainCharacter::plpoints = 100;
		}
//      GANHA O JOGO
		if (cheat == "wingame") {
			Game::GetCurrentState().deletedpapercount = 100;
			MainCharacter::papers = 0;
			MainCharacter::plpoints = 0;
		}
//      PERDE O JOGO
		if (cheat == "losegame") {
			MainCharacter::player = NULL;
		}
//      limpa a string cheat
		if (cheat != "") {
			cheat = "";
		}
    }

}


void CheatMode::Render() {

	rectShadow.Render(BLACK, 0.7);
	rectred.Render(RED);
	rectwhite.Render(WHITE);
	txInput.Render();
	
}
