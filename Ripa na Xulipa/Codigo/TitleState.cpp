
#include "TitleState.h"
#include "Game.h"

TitleState::TitleState() : bg("arquivos/img/Fundo/logoripanaxulipa.png"), music("arquivos/audio/El corridon com piano pesado.mp3"){
	
    
    rectRed = *new ColorRect(0, 0, 1024, 600);
    rectorange = *new ColorRect(300, 260, 424, 340);
	
	txPlay = *new Text(FONTE, 45, Text::TEXT_BLENDED,"NEW GAME", padraoCor, 512, 340);
	txStore = *new Text(FONTE, 45, Text::TEXT_BLENDED,"STORE", padraoCor, 512, 400);
	txOptions = *new Text(FONTE, 45, Text::TEXT_BLENDED,"OPTIONS", padraoCor, 512, 460);
	txExit = *new Text(FONTE, 45, Text::TEXT_BLENDED,"EXIT", padraoCor, 512, 550);
	
	music.Play(-1);
	bg.SetScaleX(0.35);
	bg.SetScaleY(0.35);
	
}

void TitleState::Input(){
//	se o mouse estiver movendo desabilita selecao pelo teclado
//	if (InputManager::GetInstance().mouseMoving)
//		nrtxtselected = -1;
//	
////	habilita a selecao das opcoes pelo teclado
//	if (InputManager::GetInstance().KeyPress(UP_ARROW_KEY)) {
//		nrtxtselected -= 1;
//		if (nrtxtselected < 1)
//			nrtxtselected = 4;
//	}
//	if (InputManager::GetInstance().KeyPress(DOWN_ARROW_KEY)) {
//		nrtxtselected += 1;
//		if (nrtxtselected > 4)
//			nrtxtselected = 1;
//	}
	
//  Se a tecla for ESC, setar a flag de quit
	if((InputManager::GetInstance().KeyPress(ESCAPE_KEY)) || (InputManager::GetInstance().ShouldQuit()))
		requestQuit = true;
	
	
//	mudar a Cor do texto e se ele for clicado entra na tela correspondente
	if ((InputManager::GetInstance().IsMouseInside(txPlay.box))  || (nrtxtselected == 1)) {
		
		txPlay.SetColor(padraoCorSelect);
		txPlay.SetFontSize(45);
		if(InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON) || InputManager::GetInstance().KeyPress(ENTER_KEY)){
			music.Stop();
			Game::GetInstance().Push(new StageState);
		}
	}
	else{
		txPlay.SetColor(padraoCor);
		txPlay.SetFontSize(40);
	}
//	mudar a Cor do texto e se ele for clicado entra na tela correspondente
	if ((InputManager::GetInstance().IsMouseInside(txStore.box)) || (nrtxtselected == 2)) {
		
		txStore.SetColor(padraoCorSelect);
		txStore.SetFontSize(45);
		if(InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON) || InputManager::GetInstance().KeyPress(ENTER_KEY)){
			Game::GetInstance().Push(new StoreState);
		}
	}
	else{
		txStore.SetColor(padraoCor);
		txStore.SetFontSize(40);
	}
//	mudar a Cor do texto e se ele for clicado entra na tela correspondente
	if (InputManager::GetInstance().IsMouseInside(txOptions.box) || (nrtxtselected == 3)) {
		
		txOptions.SetColor(padraoCorSelect);
		txOptions.SetFontSize(45);
		if(InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON) || InputManager::GetInstance().KeyPress(ENTER_KEY)){
			Game::GetInstance().Push(new OptionState);
		}
	}
	else{
		txOptions.SetColor(padraoCor);
		txOptions.SetFontSize(40);
	}
//	mudar a Cor do texto e se ele for clicado entra na tela correspondente
	if ((InputManager::GetInstance().IsMouseInside(txExit.box)) || (nrtxtselected == 4)) {
		
		txExit.SetColor(padraoCorSelect);
		txExit.SetFontSize(45);
		if(InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON) || InputManager::GetInstance().KeyPress(ENTER_KEY)){
			requestQuit = true;
		}
	}
	else{
		txExit.SetColor(padraoCor);
		txExit.SetFontSize(40);
	}
}


void TitleState::Update(float dt) {
//	se a musica nao estiver sendo tocada ele coloca para reproduzir novamente
    if (!music.IsPlaying()){
		music.Play(-1);
    }
	Input();
	
}


void TitleState::Render(){

    rectRed.Render(RED);
    rectorange.Render(ORANGE);
	bg.Render(512-bg.GetWidth()/2);
	txPlay.Render();
	txStore.Render();
	txOptions.Render();
	txExit.Render();
}

