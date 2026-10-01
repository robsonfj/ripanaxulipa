// Credits.cpp - tela de creditos da equipe.

#include "Credits.h"
#include "Game.h"

Credits::Credits() : bg("arquivos/img/states/Pattern.jpg"), bg2("arquivos/img/states/credits/creditsbg.png"), credMusic("arquivos/audio/creditmusic.ogg") {

	txBack = *new Text(FONTE, 50, Text::TEXT_BLENDED, "BACK", padraoCor, 80, 50);
	txTitle = *new Text(FONTE, 70, Text::TEXT_BLENDED, "CREDITS", padraoCor, 515, 55);
	txArt = *new Text(FONTE, 50, Text::TEXT_BLENDED, "ARTISTAS", padraoCor, 265, 140);
	txArt1 = *new Text(FONTE, 50, Text::TEXT_BLENDED, "Inara Régia Cardoso", padraoCor, 245, 200);
	txArt2 = *new Text(FONTE, 50, Text::TEXT_BLENDED, "Renata Rinaldi", padraoCor, 190, 250);
	txDev = *new Text(FONTE, 50, Text::TEXT_BLENDED, "DESENVOLVEDORES", padraoCor, 725, 330);
	txDev1 = *new Text(FONTE, 50, Text::TEXT_BLENDED, "Renata Cristina M. Nunes", padraoCor, 718, 390);
	txDev2 = *new Text(FONTE, 50, Text::TEXT_BLENDED, "Robson Ferreira Jacomini", padraoCor, 715, 440);
	txMus = *new Text(FONTE, 50, Text::TEXT_BLENDED, "MÚSICO", padraoCor, 275, 505);
	txMus1 = *new Text(FONTE, 50, Text::TEXT_BLENDED, "Lucian Lorens", padraoCor, 270, 560);

}


void Credits::Input() {

	//	Se o cursor estiver se movendo, desabilita selecao pelo teclado
	if (InputManager::GetInstance().mouseMoving) {
		nrtxtselected = -1;
	}

	//	Habilita a selecao das opcoes pelo teclado
	if (InputManager::GetInstance().KeyPress(UP_ARROW_KEY)) {
		nrtxtselected -= 1;
		if (nrtxtselected < 0) {
			nrtxtselected = 1;
		}
	}
	if (InputManager::GetInstance().KeyPress(DOWN_ARROW_KEY)) {
		nrtxtselected += 1;
		if (nrtxtselected > 1) {
			nrtxtselected = 0;
		}
	}

	//  Se a tecla ESC for pressionada, setar a flag para deletar esse estado
	if ((InputManager::GetInstance().KeyPress(ESCAPE_KEY))) {
		credMusic.Stop();
		requestDelete = true;
	}

	//	Se a condicao de saida for atendida
	if (InputManager::GetInstance().ShouldQuit()) {
		requestQuit = true;
	}


	//	Verifica se esta selecionado e se foi clicado
	if ((InputManager::GetInstance().IsMouseInside(txBack.box)) || (nrtxtselected == 0)) {
		txBack.selected = true;
		txBack.SetColor(padraoCorSelect2);
//		Clique ativa o item sob o mouse; ENTER ativa o item do teclado.
		if ((InputManager::GetInstance().IsMouseInside(txBack.box) && InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON)) || (nrtxtselected == 0 && InputManager::GetInstance().KeyPress(ENTER_KEY))) {
			requestDelete = true;
            credMusic.Stop();
		}
	}
	else {
		if (txBack.selected) {
			txBack.selected = false;
			txBack.SetColor(padraoCor);
		}
	}

}


void Credits::Update(float dt) {

	Input();    
    if (!credMusic.IsPlaying()) {
        credMusic.Play(-1);
    }
}


void Credits::Render() {

	bg.Render();
	bg2.Render();
	txBack.Render();
	txTitle.Render();
	txArt.Render();
	txArt1.Render();
	txArt2.Render();
	txDev.Render();
	txDev1.Render();
	txDev2.Render();
	txMus.Render();
	txMus1.Render();

}
