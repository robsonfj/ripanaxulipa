
#include "LoadState.h"
#include "Game.h"

int LoadState::percent = 0;
static int LoadStageState(void* ptr);

LoadState::LoadState() : bg("arquivos/img/states/Pattern.jpg"), loadMusic("arquivos/audio/loadmusic.mp3"), bg2("arquivos/img/states/load/playbg.png")  {
	
	percent = 0;
	loadsp = *new Sprite("arquivos/img/states/load/Loading.png", 13, 35);
	txPercent = *new Text(FONTE, 70, Text::TEXT_BLENDED,"0%", padraoCor, 512, 500);
	txPlay = *new Text(FONTE, 70, Text::TEXT_BLENDED,"PLAY", padraoCor, 512, 500);
	txBack = *new Text(FONTE, 50, Text::TEXT_BLENDED, "BACK", padraoCor, 80, 50);
    
}


void LoadState::ThreadLoad() {
    
    thread = SDL_CreateThread(LoadStageState, "StageStateLoad", this);
    if (thread) {
        loaded = true;
    }
	else {
		std::cout << "Thread Not Loaded" << std::endl;
	}
}


StageState* LoadState::ClaimStage() {

	if (thread) {
		SDL_DetachThread(thread);
		thread = nullptr;
	}
	StageState* st = stageState;
	stageState = nullptr;
	return st;

}


void LoadState::DiscardLoad() {

	if (thread) {
		SDL_WaitThread(thread, nullptr);
		thread = nullptr;
	}
	delete stageState;
	stageState = nullptr;

}


void LoadState::Input() {
    
//	Se a condicao de saida for atendida
	if (InputManager::GetInstance().ShouldQuit())
		requestQuit = true;
    
    if (percent == 100) {
//      Se a tecla ESC for pressionada, setar a flag para deletar esse estado
        if ((InputManager::GetInstance().KeyPress(ESCAPE_KEY))) {
            requestDelete = true;
            SDL_DetachThread(thread);
            loadMusic.Stop();
        }
        
//      Se a tecla ESPACO for pressionada, iniciar o jogo
        if (InputManager::GetInstance().KeyPress(ENTER_KEY)) {
            loadMusic.Stop();
            requestDelete = true;
            SDL_DetachThread(thread);
            Game::GetInstance().Push(stageState);
            stageState->cachorroFX.Play(0);
        }

//	Verifica se esta selecionado e se foi clicado
        if (InputManager::GetInstance().IsMouseInside(txPlay.box)&& sair) {
            txPlay.selected = true;
            txPlay.SetColor(padraoCorSelect);
            if (InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON)) {
                loadMusic.Stop();
                requestDelete = true;
                SDL_DetachThread(thread);
                Game::GetInstance().Push(stageState);
                stageState->cachorroFX.Play(0);
            }
        }
        else {
            if (txPlay.selected) {
                txPlay.selected = false;
                txPlay.SetColor(padraoCor);
            }
        }
    }
    
    if (InputManager::GetInstance().IsMouseInside(txBack.box)) {
		txBack.selected = true;
		txBack.SetColor(padraoCorSelect2);
		if (InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON) || InputManager::GetInstance().KeyPress(ENTER_KEY)) {
			requestDelete = true;
            loadMusic.Stop();
		}
	}
	else {
		if (txBack.selected) {
			txBack.selected = false;
			txBack.SetColor(padraoCor);
		}
	}

}


void LoadState::Update(float dt) {

	loadsp.Update(dt);
    bg2.SetClip(0, 1, 295*percent/100, 430);
    
	if (!loaded) {
		ThreadLoad();
	}

//	Se a musica nao estiver sendo tocada, coloca para reproduzir novamente
	if (!loadMusic.IsPlaying()) {
		loadMusic.Play(-1);
	}

	Input();

	std::stringstream s;
	s << percent << "%";
	txPercent.SetText(s.str());

}


void LoadState::Render() {

	bg.Render();
	bg2.Render(512 - bg2.GetWidth() / 2, 430);
	loadsp.Render(512-loadsp.GetWidth()/2, 150);
	if (percent < 100) {
		txPercent.Render();
	}
    else {
		txPlay.Render();
        txBack.Render();
    }
		
}


int LoadStageState(void* ptr) {
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        cout << SDL_GetError() << std::endl;
        exit(1);
    }
    
    IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG|IMG_INIT_TIF);
    
    Mix_Init(MIX_INIT_FLAC|MIX_INIT_MP3|MIX_INIT_OGG|MIX_INIT_MOD);
    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024);
    
    TTF_Init();
    
    SDL_GL_MakeCurrent(Game::GetInstance().window, Game::GetInstance().threadctx);
    LoadState *load = (LoadState*) ptr;
	SDL_Delay(200);
	load->stageState = new StageState;
    load->sair = true;
    glFinish();
    SDL_GL_MakeCurrent(Game::GetInstance().window, NULL);
    return 0;

}


