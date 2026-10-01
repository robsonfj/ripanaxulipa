// PauseState.cpp - pausa do jogo com cheat mode embutido.

#include "PauseState.h"
#include "Game.h"
#include "OptionState.h"


PauseState::PauseState() : music("arquivos/audio/titlemusic.ogg"), bg("arquivos/img/states/pause/pausebg.png") {
	
	txContinue = *new Text(FONTE, 50, Text::TEXT_BLENDED,"CONTINUE", padraoCor, 512, 350);
	txOptions = *new Text(FONTE, 50, Text::TEXT_BLENDED,"OPTIONS", padraoCor, 512, 420);
	txExit = *new Text(FONTE, 50, Text::TEXT_BLENDED,"EXIT", padraoCor, 512, 490);

}


void PauseState::Input() {

//	Se o cursor estiver se movendo, desabilita selecao pelo teclado
	if (InputManager::GetInstance().mouseMoving) {
		nrtxtselected = -1;
	}
    
    if (!cheats.cheatModeOn) {
//	Habilita a selecao das opcoes pelo teclado
        if (InputManager::GetInstance().KeyPress(UP_ARROW_KEY)) {
            nrtxtselected -= 1;
			if (nrtxtselected < 0) {
				nrtxtselected = 2;
			}
        }
        if (InputManager::GetInstance().KeyPress(DOWN_ARROW_KEY)) {
            nrtxtselected += 1;
			if (nrtxtselected > 2) {
				nrtxtselected = 0;
			}
        }
        
//  Se a tecla ESC for pressionada, setar a flag para deletar esse estado
        if ((InputManager::GetInstance().KeyPress(ESCAPE_KEY))) {
            pauseExit = true;
        }
        
//	Se a condicao de saida for atendida
		if (InputManager::GetInstance().ShouldQuit()) {
			requestQuit = true;
		}
        
//	Verifica se esta selecionado e se foi clicado
        if (InputManager::GetInstance().IsMouseInside(txContinue.box) || (nrtxtselected == 0)) {
            txContinue.SetColor(padraoCorSelect);
//			Clique ativa o item sob o mouse; ENTER ativa o item do teclado.
            if ((InputManager::GetInstance().IsMouseInside(txContinue.box) && InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON)) || (nrtxtselected == 0 && InputManager::GetInstance().KeyPress(ENTER_KEY))) {
                music.Stop();
                pauseContinue = true;
            }
        }
		else {
			txContinue.SetColor(padraoCor);
		}
        
//	Verifica se esta selecionado e se foi clicado
        if (InputManager::GetInstance().IsMouseInside(txOptions.box) || (nrtxtselected == 1)) {
            txOptions.SetColor(padraoCorSelect);
//			Clique ativa o item sob o mouse; ENTER ativa o item do teclado.
            if ((InputManager::GetInstance().IsMouseInside(txOptions.box) && InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON)) || (nrtxtselected == 1 && InputManager::GetInstance().KeyPress(ENTER_KEY))) {
                Game::GetInstance().Push(new OptionState);
            }
        }
		else {
			txOptions.SetColor(padraoCor);
		}        
        
//	Verifica se esta selecionado e se foi clicado
        if (InputManager::GetInstance().IsMouseInside(txExit.box) || (nrtxtselected == 2)) {
            txExit.SetColor(padraoCorSelect);
//			Clique ativa o item sob o mouse; ENTER ativa o item do teclado.
            if ((InputManager::GetInstance().IsMouseInside(txExit.box) && InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON)) || (nrtxtselected == 2 && InputManager::GetInstance().KeyPress(ENTER_KEY))) {
                pauseExit = true;
            }
        }
		else {
			txExit.SetColor(padraoCor);
		}
    }

//  Entra em cheat mode
    if (InputManager::GetInstance().IsKeyDown(SDLK_LCTRL)) {
        if (InputManager::GetInstance().IsKeyDown(SDLK_c)) {
//          Se ctrl + C for pressionado entra em cheatmode
			SDL_StartTextInput();
			cheats.cheatModeOn = true;
        }
    }

}


void PauseState::Update(float dt) {

	if (!music.IsPlaying()) {
		music.Play(-1);
	}
	Input();
    
	if (cheats.cheatModeOn) {
		cheats.Update();
	}
	
}


void PauseState::Render() {

    bg.Render();
	txContinue.Render();
	txOptions.Render();
	txExit.Render();
	
	if (cheats.cheatModeOn) {
		cheats.Render();
	}

}

