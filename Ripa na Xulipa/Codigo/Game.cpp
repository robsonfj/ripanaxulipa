
#include "Game.h"
#include "Text.h"

Game* Game::instance = 0;

Game::Game(string title, int width, int height){
	
	if (!instance) {
	
		instance = this;
		storedState = NULL;
		
		this->width = width;
		this->height = height;
		
		if(SDL_Init(SDL_INIT_EVERYTHING)){
			cout << SDL_GetError() << std::endl;
			exit(1);
		}
		
		IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG|IMG_INIT_TIF);
		
		Mix_Init(MIX_INIT_FLAC|MIX_INIT_MP3|MIX_INIT_OGG|MIX_INIT_MOD|MIX_INIT_FLUIDSYNTH|MIX_INIT_MODPLUG);
		Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT,
					  MIX_DEFAULT_CHANNELS, 1024);
		
		TTF_Init();

//		faz filtragem anistropica(diminui o serrilhamento da imagem)
//		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
//		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
		
		window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
		context = SDL_GL_CreateContext(window);
		
//		inicializa itens nessessarios do OpenGL
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glEnable(GL_DOUBLEBUFFER);
		glDisable(GL_DEPTH_TEST);
		
//		limpa a tela
		ResetView();
		
//		seed para o rand()
        srand(time(0));
	}

// inicializa coins e highscore
	FILE* fp;
	fp = fopen("arquivos/save/score.txt", "r");
	if (fp == NULL) {
		coins = 0;
		highScore = 0;
	}
	else {
		fscanf(fp, "%d %d", &coins, &highScore);
	}
	fclose(fp);
	
}

Game::~Game(){
	IMG_Quit();
	Mix_CloseAudio();
	Mix_Quit();
	TTF_Quit();
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::ResetView (){
//	limpa os buffers de renderizacao

    glClearColor(0, 0, 0, 0);
   
	glClear(GL_COLOR_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glViewport(0, 0, width, height);
	glOrtho(0, 1024, 600, 0, -1, 1);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//Usado para o calculo e impressao de frames por segundo
void Game::FPS(){
	std::stringstream s;
	int fps = 1/dt;//fps = 1 segundo / diferenca de tempo(dt)
	SDL_Color cor;
	cor.r = cor.g = cor.b = 0;
	
	static Text txFPS(FONTE, 50, Text::TEXT_BLENDED,"0", cor, 860, 30);
	timer.Update(dt*1000);
	if (timer.Get() >= 200){
		s << fps<<" FPS";
		txFPS.SetText(s.str());
		
		timer.Restart();
	}
	txFPS.Render();
}

void Game::Run(){
	
	if (storedState) {
		stateStack.emplace(storedState);
		storedState = NULL;
	}
	
	while (!GetCurrentState().RequestedQuit()) {
//      reseta a cena
        ResetView();
        
		CalculateDeltaTime();
		InputManager::GetInstance().Update();
		
		if (GetCurrentState().RequestedDelete())
			stateStack.pop();
		
		if (storedState) {
			stateStack.emplace(storedState);
			storedState = NULL;
		}
		
//		se nao existir nenhum elemento na pilha ele sai do jogo
		if (stateStack.size() == 0)
			break;
		
		GetCurrentState().Update(dt);
		GetCurrentState().Render();
		FPS();
		
        SDL_GL_SwapWindow(window);
		SDL_Delay(33);
	}
}

void Game::CalculateDeltaTime(){
	dt = SDL_GetTicks() - frameStart;
//	converte para segundos
	dt = dt/1000;
	frameStart = SDL_GetTicks();
}

int Game::GetHighScore() {
    FILE* fp;
    fp = fopen("arquivos/save/score.txt", "r");
    fscanf(fp, "%d %d",&coins, &highScore);
    fclose(fp);
	return highScore;
}

int Game::GetCoins() {
    FILE* fp;
    fp = fopen("arquivos/save/score.txt", "r");
    fscanf(fp, "%d", &coins);
    fclose(fp);
	return coins;
}

void Game::SetCoins(int value) {
	coins = value;
}

void Game::SetMute(){
	if (mute)
		mute = false;
	else
		mute = true;
}
