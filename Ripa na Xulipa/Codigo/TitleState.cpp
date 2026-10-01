
#include "TitleState.h"
#include "Game.h"
#include "StoryState.h"

TitleState::TitleState() : bg("arquivos/img/states/Pattern.jpg"), bg2("arquivos/img/states/title/menu.png"), music("arquivos/audio/titlemusic.mp3"){
	
	txPlay = *new Text(FONTE, 40, Text::TEXT_BLENDED,"NEW GAME", padraoCor, 512, 320);
	txStore = *new Text(FONTE, 40, Text::TEXT_BLENDED,"STORE", padraoCor, 512, 370);
	txOptions = *new Text(FONTE, 40, Text::TEXT_BLENDED,"OPTIONS", padraoCor, 512, 420);
	txRanking = *new Text(FONTE, 40, Text::TEXT_BLENDED, "RANKING", padraoCor, 512, 470);
	txExit = *new Text(FONTE, 40, Text::TEXT_BLENDED, "EXIT", padraoCor, 512, 520);
	txCredits = *new Text(FONTE, 40, Text::TEXT_BLENDED, "CREDITS", padraoCor, 512, 570);
	
	music.Play(-1);

}


void TitleState::Input() {

//	Se o cursor estiver se movendo, desabilita selecao pelo teclado
	if (InputManager::GetInstance().mouseMoving) {
		nrtxtselected = -1;
	}
	
//	Habilita a selecao das opcoes pelo teclado
	if (InputManager::GetInstance().KeyPress(UP_ARROW_KEY)) {
		nrtxtselected -= 1;
		if (nrtxtselected < 0) {
			nrtxtselected = 5;
		}
	}
	if (InputManager::GetInstance().KeyPress(DOWN_ARROW_KEY)) {
		nrtxtselected += 1;
		if (nrtxtselected > 5) {
			nrtxtselected = 0;
		}
	}
	
//  Se a tecla for ESC, setar a flag de quit
	if ((InputManager::GetInstance().KeyPress(ESCAPE_KEY)) || (InputManager::GetInstance().ShouldQuit())) {
		requestQuit = true;
	}	

//	Verifica se esta selecionado e se foi clicado
	if ((InputManager::GetInstance().IsMouseInside(txPlay.box))  || (nrtxtselected == 0)) {
        txPlay.selected = true;
		txPlay.SetColor(padraoCorSelect);
		if (InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON) || InputManager::GetInstance().KeyPress(ENTER_KEY)) {
			music.Stop();
			if (Game::GetInstance().skipIntro) {
				Game::GetInstance().Push(new LoadState);
			}
			else {
				Game::GetInstance().Push(new StoryState);
			}
		}
	}
	else {
        if (txPlay.selected) {
            txPlay.selected = false;
            txPlay.SetColor(padraoCor);
        }
		
	}
    
//	Verifica se esta selecionado e se foi clicado
	if ((InputManager::GetInstance().IsMouseInside(txStore.box)) || (nrtxtselected == 1)) {
        txStore.selected = true;
		txStore.SetColor(padraoCorSelect);
		if (InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON) || InputManager::GetInstance().KeyPress(ENTER_KEY)) {
			music.Stop();
			Game::GetInstance().Push(new StoreState);
		}
	}
	else {
        if (txStore.selected) {
            txStore.selected = false;
            txStore.SetColor(padraoCor);
        }
	}
    
//	Verifica se esta selecionado e se foi clicado
	if (InputManager::GetInstance().IsMouseInside(txOptions.box) || (nrtxtselected == 2)) {
        txOptions.selected = true;
		txOptions.SetColor(padraoCorSelect);
		if (InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON) || InputManager::GetInstance().KeyPress(ENTER_KEY)) {
			Game::GetInstance().Push(new OptionState);
		}
	}
	else {
        if (txOptions.selected) {
            txOptions.selected = false;
            txOptions.SetColor(padraoCor);
        }
	}
    
//	Verifica se esta selecionado e se foi clicado
	if (InputManager::GetInstance().IsMouseInside(txRanking.box) || (nrtxtselected == 3)) {
        txRanking.selected = true;
		txRanking.SetColor(padraoCorSelect);
		if (InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON) || InputManager::GetInstance().KeyPress(ENTER_KEY)) {
			Game::GetInstance().Push(new RankingState);
		}
	}
	else {
        if (txRanking.selected) {
            txRanking.selected = false;
            txRanking.SetColor(padraoCor);
        }
	}
    
//	Verifica se esta selecionado e se foi clicado
	if ((InputManager::GetInstance().IsMouseInside(txExit.box)) || (nrtxtselected == 4)) {
        txExit.selected = true;
		txExit.SetColor(padraoCorSelect);
		if (InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON) || InputManager::GetInstance().KeyPress(ENTER_KEY)) {
			requestQuit = true;
		}
	}
	else {
        if (txExit.selected) {
            txExit.selected = false;
            txExit.SetColor(padraoCor);
        }
	}
	
//	Verifica se esta selecionado e se foi clicado
	if ((InputManager::GetInstance().IsMouseInside(txCredits.box)) || (nrtxtselected == 5)) {
        txCredits.selected = true;
		txCredits.SetColor(padraoCorSelect);
		if (InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON) || InputManager::GetInstance().KeyPress(ENTER_KEY)) {
			music.Stop();
			Game::GetInstance().Push(new Credits);
		}
	}
	else {
        if (txCredits.selected) {
            txCredits.selected = false;
            txCredits.SetColor(padraoCor);
        }
	}
}


void TitleState::Update(float dt) {
    
//	Se a musica nao estiver sendo tocada, coloca para reproduzir novamente
    if (!music.IsPlaying()) {
		music.Play(-1);
    }
	Input();
    
}


void TitleState::Render() {

    bg.Render();
	bg2.Render(512-bg2.GetWidth() / 2, 10);
	txPlay.Render();
	txStore.Render();
	txOptions.Render();
	txRanking.Render();
	txExit.Render();
	txCredits.Render();

}

