
#include "StageState.h"


StageState::StageState(): road("arquivos/img/road2.jpg", 200), mainMusic("arquivos/audio/El corridon com piano leve.mp3"){

    bg = *new Sprite("arquivos/img/Fundo/fundo.png");
	bg2 = *new Sprite("arquivos/img/Fundo/cidadebackground.png");

    float temp[6] = {-100, 0, 500, 100, 700, 300};
    for (int i = 0; i < 5; i++)
        nuvens.emplace_back(new Nuvem(temp[rand()% 6], 0));
        
	paper = *new Sprite("arquivos/img/paper.png");

	txpapers = *new Text(FONTE, 40, Text::TEXT_BLENDED, " 0", padraoCor, 200, 50);
    txpoints = *new Text(FONTE, 40, Text::TEXT_BLENDED," 0 ", padraoCor, 300, 50);
	txPause = *new Text(FONTE, 40, Text::TEXT_BLENDED," || ", padraoCor, 100, 50);
	
    cachorro = new Cachorro(512, 400, 5);
	personagem = new MainCharacter(512, 400);
	AddObject(personagem);
    AddObject(cachorro);

}

void StageState::Input(){
	
//  Se a tecla for ESC, setar a flag para deletar esse estado
	if((InputManager::GetInstance().KeyPress(ESCAPE_KEY))){
		mainMusic.Stop();
		pause.paused = true;
	}
	
//	se o personagem morrer
    if (MainCharacter::player == NULL){
        requestDelete = true;
        mainMusic.Stop();
        Game::GetInstance().Push(new EndState(false));
    }
	
//	se condicao de saida for atendido
	if(InputManager::GetInstance().ShouldQuit())
		requestQuit = true;
		
//	mudar a Cor do texto e se ele for clicado entra na tela correspondente
	if (InputManager::GetInstance().IsMouseInside(txPause.box)) {
		
		txPause.SetColor(padraoCorSelect);
		if(InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON)){
			mainMusic.Stop();
			pause.paused = true;
		}
	}
	else
		txPause.SetColor(padraoCor);
	
}

void StageState::Update(float dt){
    std::stringstream s;
	Camera::Update(dt);
	
	if(pause.paused == false){
		
		time.Update(dt);
		Input();
		road.Update(dt);
//		perpadraoCorre o array de objetos
		UpdateArray(dt);
		
		
		if (!mainMusic.IsPlaying()) {
			mainMusic.Play(-1);
		}
        
//      atualiza os pontos do jogador na tela
		s << MainCharacter::papers << "/60";
		txpapers.SetText(s.str());
		s.str("");
        s << "Points: " << MainCharacter::plpoints<< "    Store: " << MainCharacter::plstrpoints;;
        txpoints.SetText(s.str());
		
//		atualiza as posicoes das nuvens
        for (int i = 0; i < 5; i++)
            nuvens[i]->Update(dt);
        
//      se o tempo da fase acabar
        printf("\n%f\n", time.Get());
        if (time.Get() > 120) {
            FILE *fp;
            fp = fopen("arquivos/save/score.txt", "w");
            MainCharacter::plstrpoints += Game::GetInstance().GetCoins();
            fprintf(fp, "%d %d\n", MainCharacter::plstrpoints, MainCharacter::plpoints);
            fclose(fp);
            requestDelete = true;
            mainMusic.Stop();
            Game::GetInstance().Push(new EndState(true));
        }
        
	}
	else
		Pause();
	
}

void StageState::Render(){
	
	
	bg.Render();
	bg2.Render(0, 25);
    for (int i = 0; i < 5; i++)
        nuvens[i]->Render();
    
	road.Render();
//	percorre array e renderiza os objetos
	RenderArray();

	paper.SetScaleX(0.4);
	paper.SetScaleY(0.4);
	paper.Render(130, 20);

	txpapers.Render();
    txpoints.Render();
	txPause.Render();
	
	if (pause.paused)
		pause.Render();//rederiza o statepause
	
}

void StageState::Pause(){
    
    pause.Update(0);
    if(pause.pauseContinue){
        pause.paused = false;
        pause.pauseContinue = false;
    }
    if (pause.pauseExit) {
        requestDelete = true;
        
    }
		
}

StageState::~StageState(){
	objectArray.clear();
	Road::segmentos.clear();
}
