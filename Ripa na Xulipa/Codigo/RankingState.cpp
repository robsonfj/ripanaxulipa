
#include "RankingState.h"
#include "Game.h"


RankingState::RankingState() : bg("arquivos/img/states/Pattern.jpg"), bg2("arquivos/img/states/ranking/rankingbg.png"), music("arquivos/audio/titlemusic.mp3") {
	FILE *fp;

	txBack = *new Text(FONTE, 50, Text::TEXT_BLENDED, "BACK", padraoCor, 80, 50);
	txTitle = *new Text(FONTE, 70, Text::TEXT_BLENDED, "RANKING", padraoCor, 512, 54);
	// Inicializa as posicoes dos textos do ranking
	for (int i = 0; i < 10; i++) {
		txRanking[i] = *new Text(FONTE, 35, Text::TEXT_BLENDED, "0", padraoCor, 322, 120 + 50 * i);
	}

	//	Le o arquivo com o ranking
	fp = fopen("arquivos/save/highscores.txt", "r");
	while (fscanf(fp, "%d", &scores[tam].score) > 0) {
		fscanf(fp, "%d", &scores[tam].papers); // pega papers
		getc(fp); // pega "\n"
		tam++;
	}
	fclose(fp);
    
}


void RankingState::Input() {

    std::stringstream s;
    
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
		if (InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON) || InputManager::GetInstance().KeyPress(ENTER_KEY)) {
			requestDelete = true;
		}
	}
    else {
        if (txBack.selected) {
            txBack.selected = false;
            txBack.SetColor(padraoCor);
        }
    }
    
    for (int i = 0; i < tam; i++) {
        s << i + 1 << ". " << scores[i].score << " - " << scores[i].papers << "/100" << " - " << Game::GetInstance().CalculateScore(scores[i].papers);
        txRanking[i].SetText(s.str());
        s.str("");
    }

}


void RankingState::Update(float dt) {

	//	Se a musica nao estiver sendo tocada, coloca para reproduzir novamente
	if (!music.IsPlaying()) {
		music.Play(-1);
	}
	Input();

}


void RankingState::Render() {
	
	bg.Render();
	bg2.Render(512 - bg2.GetWidth() / 2);
	txBack.Render();
	txTitle.Render();
	for (int i = 0; i < tam; i++) {
		txRanking[i].Render();
	}

}
