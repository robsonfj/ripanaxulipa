
#include "EndState.h"
#include "LoadState.h"


EndState::EndState(bool win) {

    std::stringstream s;
    
	if (win) {
		bg = *new Sprite("arquivos/img/states/end/win.png");
        endMusic = *new Music("arquivos/audio/Vitoria.mp3");
        s.str("");
		s << Game::GetInstance().CalculateScore(MainCharacter::papers);
		txNota = *new Text(FONTE, 50, Text::TEXT_BLENDED, s.str(), padraoCor, 505, 145);
		txScore = *new Text(FONTE, 60, Text::TEXT_BLENDED, "Points", padraoCor, 120, 320);
		txHighscore = *new Text(FONTE, 60, Text::TEXT_BLENDED, "High Score", padraoCor, 875, 320);
		txNscore = *new Text(FONTE, 50, Text::TEXT_BLENDED, "0", padraoCor, 125, 380);
		txNhighscore = *new Text(FONTE, 50, Text::TEXT_BLENDED, "0", padraoCor, 825, 380);
		txMenu = *new Text(FONTE, 50, Text::TEXT_BLENDED, "Menu", padraoCor, 175, 540);
		txRepeat = *new Text(FONTE, 50, Text::TEXT_BLENDED, "Repeat?", padraoCor, 835, 540);
	}
	else {
		bg = *new Sprite("arquivos/img/states/end/lose.png");
		txNota = *new Text(FONTE, 50, Text::TEXT_BLENDED, "Nota: SR", padraoCor, 505, 155);
		txScore = *new Text(FONTE, 60, Text::TEXT_BLENDED, "Points", padraoCor, 120, 320);
		txHighscore = *new Text(FONTE, 60, Text::TEXT_BLENDED, "High Score", padraoCor, 875, 320);
		txNscore = *new Text(FONTE, 50, Text::TEXT_BLENDED, "0", padraoCor, 125, 380);
		txNhighscore = *new Text(FONTE, 50, Text::TEXT_BLENDED, "0", padraoCor, 825, 380);
		txMenu = *new Text(FONTE, 50, Text::TEXT_BLENDED, "Menu", padraoCor, 130, 525);
		txRepeat = *new Text(FONTE, 50, Text::TEXT_BLENDED, "Repeat?", padraoCor, 860, 525);
	}
    
}


void EndState::Input() {
    
//	Se o cursor estiver se movendo, desabilita selecao pelo teclado
	if (InputManager::GetInstance().mouseMoving) {
		nrtxtselected = -1;
	}
    
//	Habilita a selecao das opcoes pelo teclado
    if (InputManager::GetInstance().KeyPress(UP_ARROW_KEY)) {
        nrtxtselected -= 1;
		if (nrtxtselected < 0) {
			nrtxtselected = 2;
		}
    }
    if (InputManager::GetInstance().KeyPress(DOWN_ARROW_KEY)) {
        nrtxtselected += 1;
		if (nrtxtselected > 1) {
			nrtxtselected = 0;
		}
    }
    
//  Se a tecla for ESC, setar a flag para deletar esse estado
    if ((InputManager::GetInstance().KeyPress(ESCAPE_KEY))) {
        endMusic.Stop();
        requestDelete = true;
    }
    
//	Se a condicao de saida for atendida
	if (InputManager::GetInstance().ShouldQuit()) {
		requestQuit = true;
	}
    
//	Verifica se esta selecionado e se foi clicado
    if (InputManager::GetInstance().IsMouseInside(txMenu.box) || nrtxtselected == 0) {
        txMenu.selected = true;
        txMenu.SetColor(padraoCorSelect);
        if (InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON)|| InputManager::GetInstance().KeyPress(ENTER_KEY)) {
            endMusic.Stop();
            requestDelete = true;
        }
    }
    else {
        if (txMenu.selected) {
            txMenu.selected = false;
            txMenu.SetColor(padraoCor);
        }
    }
    
//	Verifica se esta selecionado e se foi clicado
    if (InputManager::GetInstance().IsMouseInside(txRepeat.box) || nrtxtselected == 1) {
        txRepeat.selected = true;
        txRepeat.SetColor(padraoCorSelect);
        if (InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON)|| InputManager::GetInstance().KeyPress(ENTER_KEY)) {
            requestDelete = true;
            endMusic.Stop();
            Game::GetInstance().Push(new LoadState);
        }
    }
    else {
        if (txRepeat.selected) {
            txRepeat.selected = false;
            txRepeat.SetColor(padraoCor);
        }
    }
    
}


void EndState::Update(float dt) {

    std::stringstream s;

    Input();
    
	if (!endMusic.IsPlaying()) {
		endMusic.Play(-1);
	}
    
    s.str("");
    s << MainCharacter::plpoints;
    txNscore.SetText(s.str());
    s.str("");
    s << Game::GetInstance().GetHighScore();
    txNhighscore.SetText(s.str());

}


void EndState::Render() {
    
    bg.Render();
	txNota.Render();
    txScore.Render();
    txHighscore.Render();
    txNscore.Render();
    txNhighscore.Render();
    txMenu.Render();
    txRepeat.Render();
    
}