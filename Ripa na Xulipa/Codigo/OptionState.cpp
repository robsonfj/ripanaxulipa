
#include "OptionState.h"
#include "Game.h"

OptionState::OptionState(): music("arquivos/audio/El corridon com piano pesado.mp3"){
	
//    retangulo vermelho
	rectRed = *new ColorRect(0, 0, 1024, 600);
//    retangulo laranja
	rectOrange = *new ColorRect(300, 180, 424, 320);
    
	txMute = *new Text(FONTE, 50, Text::TEXT_BLENDED,"MUTE", padraoCor, 512, 250);
	txMinus = *new Text(FONTE, 80, Text::TEXT_BLENDED,"-", padraoCor, 450, 350);
	txPlus = *new Text(FONTE, 80, Text::TEXT_BLENDED,"+", padraoCor, 570, 350);
	txVolume = *new Text(FONTE, 50, Text::TEXT_BLENDED,std::to_string(Game::GetInstance().volume), padraoCor, 512, 350);
	txReset = *new Text(FONTE, 50, Text::TEXT_BLENDED, "RESET", padraoCor, 512, 450);
	txBack = *new Text(FONTE, 50, Text::TEXT_BLENDED,"BACK", padraoCor, 80, 50);
}

void OptionState::Input(){
	
//  Se a tecla for ESC, setar a flag para deletar esse estado
	if((InputManager::GetInstance().KeyPress(ESCAPE_KEY))){
		requestDelete = true;
	}

//	se condicao de saida for atendido
	if(InputManager::GetInstance().ShouldQuit())
		requestQuit = true;
	
//	mudar a Cor do texto e se ele for clicado entra na tela correspondente
	if (InputManager::GetInstance().IsMouseInside(txMute.box)) {
		
		txMute.SetColor(padraoCorSelect);
		if(InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON)){
			Game::GetInstance().SetMute();
		}
	}
	else{
		if (Game::GetInstance().mute)
			txMute.SetColor(padraoCorSelect);
		else
			txMute.SetColor(padraoCor);
	}
	
//	mudar a Cor do texto e se ele for clicado entra na tela correspondente
	if (InputManager::GetInstance().IsMouseInside(txPlus.box)) {
		
		txPlus.SetColor(padraoCorSelect);
		if(InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON)){
			if (Game::GetInstance().volume < 100)
				Game::GetInstance().volume += 10;
		}
	}
	else
		txPlus.SetColor(padraoCor);
	
//	mudar a Cor do texto e se ele for clicado entra na tela correspondente
	if (InputManager::GetInstance().IsMouseInside(txMinus.box)) {
		
		txMinus.SetColor(padraoCorSelect);
		if(InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON)){
			if (Game::GetInstance().volume > 0)
				Game::GetInstance().volume -= 10;
		}
	}
	else
		txMinus.SetColor(padraoCor);
	
	//	mudar a Cor do texto e se ele for clicado entra na tela correspondente
	if (InputManager::GetInstance().IsMouseInside(txReset.box)) {

		txReset.SetColor(padraoCorSelect);
		if (InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON)){
			this->nextState = new MessageWindow(2, "Sua pontuacao e seus itens serao zerados. Tem certeza?");
			this->nextState->previousState = this;
			Game::GetInstance().Push(this->nextState);
		}
	}
	else
		txReset.SetColor(padraoCor);
	
//	mudar a Cor do texto e se ele for clicado entra na tela correspondente
	if (InputManager::GetInstance().IsMouseInside(txBack.box)) {
		
		txBack.SetColor(padraoCorSelect);
		if(InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON)){
			requestDelete = true;
		}
	}
	else
		txBack.SetColor(padraoCor);
	
}


void OptionState::Update(float dt) {
	
//	se a musica nao estiver sendo tocada ele coloca para reproduzir novamente
    if (!music.IsPlaying())
		music.Play(-1);

// verifica se era pra resetar as pontuacoes e os itens
	if (resultados.size() > 0 && strcmp(resultados.front().descricao, "escolha") == 0) {
		if (resultados.front().boolValue) {
			FILE *fp;
			fp = fopen("arquivos/save/score.txt", "w");
			fprintf(fp, "0 0\n");
			fclose(fp);
			fp = fopen("arquivos/save/storehistory.txt", "w");
			fprintf(fp, "");
			fclose(fp);
		}
		resultados.clear();
	}

	Input();
	
	txVolume.SetText(std::to_string(Game::GetInstance().volume));
	
}


void OptionState::Render(){
	
	rectRed.Render(RED);
	rectOrange.Render(ORANGE);
	txMute.Render();
	txVolume.Render();
	txPlus.Render();
	txMinus.Render();
	txReset.Render();
	txBack.Render();
	
}
