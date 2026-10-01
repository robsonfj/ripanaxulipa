
#include "OptionState.h"
#include "Game.h"

static std::string IntroLabel() {

	if (Game::GetInstance().skipIntro) {
		return "INTRO: OFF";
	}
	return "INTRO: ON";

}

OptionState::OptionState() : music("arquivos/audio/titlemusic.mp3"), bg("arquivos/img/states/Pattern.jpg"), bg2("arquivos/img/states/options/optionsbg.png"){
    
	txMute = *new Text(FONTE, 50, Text::TEXT_BLENDED,"MUTE", padraoCor, 512, 200);
	txMinus = *new Text(FONTE, 80, Text::TEXT_BLENDED,"-", padraoCor, 450, 300);
	txPlus = *new Text(FONTE, 80, Text::TEXT_BLENDED,"+", padraoCor, 570, 300);
	txVolume = *new Text(FONTE, 50, Text::TEXT_BLENDED,std::to_string(Game::GetInstance().volume), padraoCor, 512, 300);
	txReset = *new Text(FONTE, 50, Text::TEXT_BLENDED, "RESET", padraoCor, 512, 400);
	txBack = *new Text(FONTE, 50, Text::TEXT_BLENDED,"BACK", padraoCor, 80, 50);
	txIntro = *new Text(FONTE, 50, Text::TEXT_BLENDED, IntroLabel(), padraoCor, 512, 500);

}


void OptionState::Input() {

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
    
//  Se a tecla ESC for pressionada, setar a flag para deletar esse estado
	if ((InputManager::GetInstance().KeyPress(ESCAPE_KEY))) {
		requestDelete = true;
	}

//	Se a condicao de saida for atendida
	if (InputManager::GetInstance().ShouldQuit()) {
		requestQuit = true;
	}
	
    
//	Verifica se esta selecionado e se foi clicado (volta)
    if ((InputManager::GetInstance().IsMouseInside(txBack.box)) || (nrtxtselected == 0)) {
        txBack.SetColor(padraoCorSelect2);
        if (InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON)|| InputManager::GetInstance().KeyPress(ENTER_KEY)) {
            requestDelete = true;
        }
    }
	else {
		txBack.SetColor(padraoCor);
	}
    
    
//	Verifica se esta selecionado e se foi clicado (seta o mute)
	if ((InputManager::GetInstance().IsMouseInside(txMute.box)) || (nrtxtselected == 1)) {
		txMute.SetColor(padraoCorSelect);
		if (InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON)|| InputManager::GetInstance().KeyPress(ENTER_KEY)) {
			Game::GetInstance().SetMute();
		}
	}
	else {
		if (Game::GetInstance().mute) {
			txMute.SetColor(padraoCorSelect);
		}
		else {
			txMute.SetColor(padraoCor);
		}
	}
	
//	Verifica se esta selecionado e se foi clicado (aumenta o volume)
	if ((InputManager::GetInstance().IsMouseInside(txPlus.box)) || (nrtxtselected == 2)) {
		
		txPlus.SetColor(padraoCorSelect);
		if (InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON)|| InputManager::GetInstance().KeyPress(ENTER_KEY)) {
			if (Game::GetInstance().volume < 100) {
				Game::GetInstance().volume += 10;
			}
		}
	}
	else {
		txPlus.SetColor(padraoCor);
	}
	
//	Verifica se esta selecionado e se foi clicado (diminui o volume)
	if ((InputManager::GetInstance().IsMouseInside(txMinus.box)) || (nrtxtselected == 3)) {
		txMinus.SetColor(padraoCorSelect);
		if (InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON)|| InputManager::GetInstance().KeyPress(ENTER_KEY)) {
			if (Game::GetInstance().volume > 0) {
				Game::GetInstance().volume -= 10;
			}
		}
	}
	else {
		txMinus.SetColor(padraoCor);
	}
	
//	Verifica se esta selecionado e se foi clicado (reseta)
	if ((InputManager::GetInstance().IsMouseInside(txReset.box)) || (nrtxtselected == 4)) {
		txReset.SetColor(padraoCorSelect);
		if (InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON)|| InputManager::GetInstance().KeyPress(ENTER_KEY)) {
			this->nextState = new MessageWindow(2, "Sua pontuação e seus itens serão zerados. Tem certeza?");
			this->nextState->previousState = this;
			Game::GetInstance().Push(this->nextState);
		}
	}
	else {
		txReset.SetColor(padraoCor);
	}

//	Verifica se esta selecionado e se foi clicado (liga/desliga a intro da historia)
	if ((InputManager::GetInstance().IsMouseInside(txIntro.box)) || (nrtxtselected == 5)) {
		txIntro.SetColor(padraoCorSelect);
		if (InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON)|| InputManager::GetInstance().KeyPress(ENTER_KEY)) {
			Game::GetInstance().SetSkipIntro(!Game::GetInstance().skipIntro);
		}
	}
	else {
		txIntro.SetColor(padraoCor);
	}
	
}


void OptionState::Update(float dt) {

//	Se a musica nao estiver sendo tocada, coloca para reproduzir novamente
	if (!music.IsPlaying()) {
		music.Play(-1);
	}

//	Verifica se deve resetar as pontuacoes e os itens
	if (resultados.size() > 0 && strcmp(resultados.front().descricao, "escolha") == 0) {
		if (resultados.front().boolValue) {
			FILE *fp;
			fp = fopen("arquivos/save/coins.txt", "w");
			if (fp != NULL) {
				fprintf(fp, "0\n");
				fclose(fp);
			}
			fp = fopen("arquivos/save/highscores.txt", "w");
			if (fp != NULL) {
				fclose(fp);
			}
			fp = fopen("arquivos/save/storehistory.txt", "w");
			if (fp != NULL) {
				fclose(fp);
			}
		}
		resultados.clear();
	}

	Input();

	txVolume.SetText(std::to_string(Game::GetInstance().volume));
	txIntro.SetText(IntroLabel());
	
}


void OptionState::Render() {
	
	bg.Render();
	bg2.Render(512 - bg2.GetWidth() / 2, 120);
	txMute.Render();
	txVolume.Render();
	txPlus.Render();
	txMinus.Render();
	txReset.Render();
	txIntro.Render();
	txBack.Render();
	
}
