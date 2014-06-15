
#include "PauseState.h"
#include "Game.h"
#include "OptionState.h"

PauseState::PauseState(): music("arquivos/audio/El corridon com piano pesado.mp3"){
	
    rectWhite = *new ColorRect(380, 240, 260, 260);
	txContinue = *new Text(FONTE, 50, Text::TEXT_BLENDED,"CONTINUE", padraoCor, 512, 300);
	txOptions = *new Text(FONTE, 50, Text::TEXT_BLENDED,"OPTIONS", padraoCor, 512, 380);
	txExit = *new Text(FONTE, 50, Text::TEXT_BLENDED,"EXIT", padraoCor, 512, 460);
	
}

void PauseState::Input(){
	
//	habilita a selecao das opcoes pelo teclado
//	if (InputManager::GetInstance().KeyPress(UP_ARROW_KEY)) {
//		nrtxtselected -= 1;
//		if (nrtxtselected < 0)
//			nrtxtselected = 1;
//	}
//	if (InputManager::GetInstance().KeyPress(DOWN_ARROW_KEY)) {
//		nrtxtselected += 1;
//		if (nrtxtselected > 1)
//			nrtxtselected = 0;
//	}
	
//  Se a tecla for ESC, setar a flag para deletar esse estado
	if((InputManager::GetInstance().KeyPress(ESCAPE_KEY))){
		pauseExit = true;
	}
	
//	se condicao de saida for atendido
	if(InputManager::GetInstance().ShouldQuit())
		requestQuit = true;
	
//	mudar a Cor do texto e se ele for clicado entra na tela correspondente
	if (InputManager::GetInstance().IsMouseInside(txContinue.box) || (nrtxtselected == 0)) {
		
		txContinue.SetColor(padraoCorSelect);
		if(InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON) || InputManager::GetInstance().KeyPress(ENTER_KEY)){
			music.Stop();
			pauseContinue = true;
		}
	}
	else
		txContinue.SetColor(padraoCor);
	
//	mudar a Cor do texto e se ele for clicado entra na tela correspondente
	if (InputManager::GetInstance().IsMouseInside(txOptions.box) || (nrtxtselected == 0)) {
		
		txOptions.SetColor(padraoCorSelect);
		if(InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON) || InputManager::GetInstance().KeyPress(ENTER_KEY)){
			Game::GetInstance().Push(new OptionState);
		}
	}
	else
		txOptions.SetColor(padraoCor);
	
	
//	mudar a Cor do texto e se ele for clicado entra na tela correspondente
	if (InputManager::GetInstance().IsMouseInside(txExit.box) || (nrtxtselected == 1)) {
		
		txExit.SetColor(padraoCorSelect);
		if(InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON) || InputManager::GetInstance().KeyPress(ENTER_KEY)){
			pauseExit = true;
		}
	}
	else
		txExit.SetColor(padraoCor);
	
}

void PauseState::Update(float dt) {
	if (!music.IsPlaying()) {
		music.Play(-1);
	}
	Input();
	
}

void PauseState::Render(){
    rectWhite.Render(WHITE);
	txContinue.Render();
	txOptions.Render();
	txExit.Render();
	
}

