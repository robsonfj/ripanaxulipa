
#include "StageState.h"
#include "LoadState.h"


StageState::StageState() {
    int i = 0;
    
    for (i = LoadState::percent; i<10; i++) {
        LoadState::percent = i;
        SDL_Delay(20);
    }
	
    road = *new Road("arquivos/img/states/stage/roadobj/road.jpg", 250);
    
//  Inicializa variaveis
    playingB1 = true;
    playingB2 = false;

    cachorroFX = *new Sound("arquivos/audio/Efeitchos Sonoricos/k-chorro.ogg");
   
    for (i = LoadState::percent; i < 50; i++) {
        LoadState::percent = i;
        SDL_Delay(20);
    }
	
    bg = *new Sprite("arquivos/img/states/stage/fundo/fundo.jpg");
	bg2 = *new Sprite("arquivos/img/states/stage/fundo/cidadebackground.png");

    float temp[6] = {-100, 0, 500, 100, 700, 300};
	for (int i = 0; i < 5; i++) {
		nuvens.emplace_back(new Nuvem(temp[rand() % 6], 0));
	}
    for (i = LoadState::percent; i < 60; i++) {
        LoadState::percent = i;
        SDL_Delay(20);
    }
	
	LoadMusic();
    for (i = LoadState::percent; i < 70; i++) {
        LoadState::percent = i;
        SDL_Delay(20);
    }
	
	paper = *new Sprite("arquivos/img/states/stage/paper.png");
	coin = *new Sprite("arquivos/img/states/stage/coin.png");
    
    for (i = LoadState::percent; i < 80; i++) {
        LoadState::percent = i;
        SDL_Delay(20);
    }
    txpapers = *new Text(FONTE, 40, Text::TEXT_BLENDED, " 0", padraoCor, 200, 50);
	txcoins = *new Text(FONTE, 40, Text::TEXT_BLENDED, " 0 ", padraoCor, 410, 50);
	txpoints = *new Text(FONTE, 40, Text::TEXT_BLENDED, " 0 ", padraoCor, 500, 50);
	txpause = *new Text(FONTE, 40, Text::TEXT_BLENDED," || ", padraoCor, 80, 50);
	
    for (i = LoadState::percent; i < 90; i++) {
        LoadState::percent = i;
        SDL_Delay(20);
    }
	
    cachorro = new Cachorro(512, 400, 8);
	personagem = new MainCharacter(512, 400);
	AddObject(personagem);
    AddObject(cachorro);
	
    for (i = LoadState::percent; i <= 100; i++) {
        LoadState::percent = i;
        SDL_Delay(20);
    }
	
}


void StageState::LoadMusic() {
	
	mainmusic.Open("arquivos/audio/Blocos/1.ogg");
	mainmusic.Open("arquivos/audio/Blocos/2.ogg");
	mainmusic.Open("arquivos/audio/Blocos/3.ogg");
	mainmusic.Open("arquivos/audio/Blocos/4.ogg");
	mainmusic.Open("arquivos/audio/Blocos/5.ogg");
	mainmusic.Open("arquivos/audio/Blocos/6.ogg");
	mainmusic.Open("arquivos/audio/Blocos/7.ogg");
	mainmusic.Open("arquivos/audio/Blocos/8.ogg");
	mainmusic.Open("arquivos/audio/Blocos/9.ogg");
	
}


void StageState::Input() {
	
//  Se as teclas ESC ou SPACE forem pressionadas, setar a flag para pausa
	if (InputManager::GetInstance().KeyPress(ESCAPE_KEY)) {
		mainmusic.Stop();
		pause.paused = true;
	}
	
//	Se o personagem morrer
    if (MainCharacter::player == NULL) {
		FILE *fp;
		fp = fopen("arquivos/save/coins.txt", "w");
		MainCharacter::plstrpoints += Game::GetInstance().GetCoins();
		fprintf(fp, "%d\n", MainCharacter::plstrpoints);
		fclose(fp);
		Game::GetInstance().AddToRanking(MainCharacter::plpoints, MainCharacter::papers);

        requestDelete = true;
        mainmusic.Stop();
        Game::GetInstance().Push(new EndState(false));
    }
	
//	Se a condicao de saida for atendida
	if (InputManager::GetInstance().ShouldQuit()) {
		requestQuit = true;
	}
		
//	Verifica se esta selecionado e se foi clicado
	if (InputManager::GetInstance().IsMouseInside(txpause.box)) {
        txpause.selected = true;
		txpause.SetColor(padraoCorSelect);
		if (InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON)) {
			mainmusic.Stop();
			pause.paused = true;
		}
	}
    else {
        if (txpause.selected) {
            txpause.selected = false;
             txpause.SetColor(padraoCor);
        }
    }

}


void StageState::Update(float dt) {

    std::stringstream s;
	Camera::Update(dt);
	
	if (pause.paused == false) {
		Input();
		road.Update(dt);
//		Percorre o array de objetos
		UpdateArray(dt);
		
//      Seta a musica com blocos aleatorios
        PlayBlocos();
		
//      Atualiza os pontos do jogador na tela
		s << MainCharacter::papers << "/100   ";
		txpapers.SetText(s.str());
		s.str("");
		s << MainCharacter::plstrpoints;
		txcoins.SetText(s.str());
		s.str("");
        s << "Points: " << MainCharacter::plpoints;
        txpoints.SetText(s.str());
		
//		Atualiza as posicoes das nuvens
		for (int i = 0; i < 5; i++) {
			nuvens[i]->Update(dt);
		}
        
//      Se a fase acabar
        if (deletedpapercount >= 100) {
			FILE *fp;
			fp = fopen("arquivos/save/coins.txt", "w");
			MainCharacter::plstrpoints += Game::GetInstance().GetCoins();
			fprintf(fp, "%d\n", MainCharacter::plstrpoints);
			fclose(fp);
			Game::GetInstance().AddToRanking(MainCharacter::plpoints, MainCharacter::papers);
			requestDelete = true;
			block1.Stop();
			Game::GetInstance().Push(new EndState(true));
        }
        
	}
	else {
		Pause();
	}
	
}


void StageState::Render() {
	
	bg.Render();
	bg2.Render(0, 25);
	for (int i = 0; i < 5; i++) {
		nuvens[i]->Render();
	}
    
	road.Render();
//	Percorre array e renderiza os objetos
	RenderArray();

	paper.Render(130, 20);
	coin.Render(330, 20);

	txpapers.Render();
    txpoints.Render();
	txcoins.Render();
	txpause.Render();
	
//  Renderiza o pausestate
	if (pause.paused) {
		pause.Render(); 
	}
	
}


void StageState::Pause() {
    
    pause.Update(0);
    if (pause.pauseContinue) {
        pause.paused = false;
        pause.pauseContinue = false;
    }
    if (pause.pauseExit) {
        requestDelete = true;
        
    }
		
}


void StageState::PlayBlocos() {

    std::stringstream s;
    s << "arquivos/audio/Blocos/";

//    Do Bloco 1 a música pode ir pro Bloco 2 ou 3.
//    Do Bloco 2 ela pode ir para o 1 ou o 3.
//    Do Bloco 3 ela só pode ir para o Bloco 1.

    if (!block1.IsPlaying()) {
        if (playingB1) {
            playingB1 = false;
            switch (1 + rand() % 2) {
                case 1:
                    s << 4 + rand() % 3 << ".ogg";
                    block2.Open(s.str());
                    block2.Play(0);
                    playingB2 = true;
//                    cout<<"Bloco 1-2"<<std::endl;
                    break;
                case 2:
                    s << 7 + rand() % 3 << ".ogg";
                    block3.Open(s.str());
                    block3.Play(0);
//                    cout<<"Bloco 1-3"<<std::endl;
                    break;
                default:
                    break;
            }
        }
        else {
            if (playingB2) {
                playingB2 = false;
                switch (1 + rand() % 2) {
                    case 1:
                        s << 1 + rand() % 3 << ".ogg";
                        block1.Open(s.str());
                        mainmusic = block1;
                        block1.Play(0);
                        playingB1 = true;
//                        cout<<"Bloco 2-1"<<std::endl;
                        break;
                    case 2:
                        s << 7 + rand() % 3 << ".ogg";
                        block3.Open(s.str());
                        mainmusic = block3;
                        block3.Play(0);
//                        cout<<"Bloco 2-3"<<std::endl;
                        return;
                    default:
                        break;
                }
            }
            else {
                s << 1 + rand() % 3 << ".ogg";
                block1.Open(s.str());
                mainmusic = block1;
                block1.Play(0);
                playingB1 = true;
//                cout<<"Bloco 3-1"<<std::endl;
            }
        }
    }

}


StageState::~StageState() {

	objectArray.clear();
    nuvens.clear();
	Road::segmentos.clear();

}



