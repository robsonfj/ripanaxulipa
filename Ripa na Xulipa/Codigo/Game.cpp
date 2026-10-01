
#include "Game.h"
#include "Text.h"

Game* Game::instance = 0;


Game::Game(string title, int width, int height) {
	
	if (!instance) {	
		instance = this;
		storedState = NULL;
		
		this->width = width;
		this->height = height;
		
		if (SDL_Init(SDL_INIT_EVERYTHING)) {
			cout << SDL_GetError() << std::endl;
			exit(1);
		}
		
		IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG|IMG_INIT_TIF);
		
		Mix_Init(MIX_INIT_FLAC|MIX_INIT_MP3|MIX_INIT_OGG|MIX_INIT_MOD);
		Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024);
		
		TTF_Init();

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
        
        context = SDL_GL_CreateContext(window);
		SDL_GL_MakeCurrent(window, context);

//		O contexto da thread de load deve COMPARTILHAR texturas com o
//		contexto principal (senao a fase carrega em outra memoria de GPU
//		e renderiza em branco). O atributo tem que ser setado ANTES de
//		criar o segundo contexto, com o primeiro corrente na thread.
		SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
        threadctx = SDL_GL_CreateContext(window);
        SDL_GL_MakeCurrent(window, context);
        
//		Inicializa itens nessessarios do OpenGL
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		
//		Limpa a tela
		ResetView();
		
//		Seed para o rand()
        srand(time(0));
	}

//  Inicializa coins
	FILE* fp;
	fp = fopen("arquivos/save/coins.txt", "r");
	if (fp == NULL) {
		fp = fopen("arquivos/save/coins.txt", "w");
		if (fp != NULL) {
			fprintf(fp, "%d", coins);
			fclose(fp);
		}
	}
	else {
		fscanf(fp, "%d", &coins);
		fclose(fp);
	}

//  Inicializa highscore
	fp = fopen("arquivos/save/highscores.txt", "r");
	if (fp == NULL) {
		highScore = 0;
		fp = fopen("arquivos/save/highscores.txt", "w");
		if (fp != NULL) {
			fclose(fp);
		}
	}
	else {
		fscanf(fp, "%d", &highScore);
		fclose(fp);
	}

//  Carrega config (pular introducao da historia)
	skipIntro = false;
	fp = fopen("arquivos/save/config.txt", "r");
	if (fp != NULL) {
		int v = 0;
		if (fscanf(fp, "%d", &v) > 0) {
			skipIntro = (v != 0);
		}
		fclose(fp);
	}
	
}


Game::~Game() {

	IMG_Quit();
	Mix_CloseAudio();
	Mix_Quit();
	TTF_Quit();
	SDL_DestroyWindow(window);
	SDL_Quit();

}


void Game::ResetView () {

//	Limpa os buffers de renderizacao
    glClearColor(0, 0, 0, 1);
   	glClear(GL_COLOR_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glViewport(0, 0, width, height);
	glOrtho(0, width, height, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}


// Usado para o calculo e impressao de frames por segundo
void Game::FPS() {

    std::stringstream s;
 
	if (showFPS) {
        int fps = 1 / dt;//fps = 1 segundo / diferenca de tempo(dt)
        SDL_Color cor;
        cor.a = 1;
        cor.r = cor.g = cor.b = 0;
        
        static Text txFPS(FONTE, 50, Text::TEXT_BLENDED, "0", cor, 860, 30);
        timer.Update(dt * 1000);
        if (timer.Get() >= 200) {
            s << fps << " FPS";
            txFPS.SetText(s.str());
            timer.Restart();
        }
        txFPS.Render();
    }

}


void Game::Run() {
	
	if (storedState) {
		stateStack.emplace(storedState);
		storedState = NULL;
	}
	
	while (!GetCurrentState().RequestedQuit()) {
//      Reseta a cena
        ResetView();
        
		CalculateDeltaTime();
		InputManager::GetInstance().Update();
		
		if (GetCurrentState().RequestedDelete()) {
			stateStack.pop();
		}

		if (storedState) {
			stateStack.emplace(storedState);
			storedState = NULL;
		}
		
//		Se nao existir nenhum elemento na pilha ele sai do jogo
		if (stateStack.size() == 0) {
			break;
		}

        glFlush();
		GetCurrentState().Update(dt);
		GetCurrentState().Render();
        glFinish();
		FPS();
        
        SDL_GL_SwapWindow(window);
		SDL_Delay(33);
	}

}


void Game::CalculateDeltaTime() {

	dt = SDL_GetTicks() - frameStart;
//	Converte para segundos
	dt = dt / 1000;
	frameStart = SDL_GetTicks();

}


int Game::GetHighScore() {

    FILE* fp;
    fp = fopen("arquivos/save/highscores.txt", "r");
	if (fp != NULL) {
		fscanf(fp, "%d", &highScore); // Le o primeiro valor (o maior, porque o arquivo esta ordenado)
		fclose(fp);
	}
	return highScore;

}


int Game::GetCoins() {

    FILE* fp;
    fp = fopen("arquivos/save/coins.txt", "r");
	if (fp != NULL) {
		fscanf(fp, "%d", &coins);
		fclose(fp);
	}
	return coins;

}


void Game::SetCoins(int value) {

	FILE* fp;
	coins = value;
	fp = fopen("arquivos/save/coins.txt", "w");
	if (fp != NULL) {
		fprintf(fp, "%d\n", coins);
		fclose(fp);
	}

}


void Game::SetMute() {

	if (mute) {
		mute = false;
	}
	else {
		mute = true;
	}

}


void Game::SetSkipIntro(bool value) {

	skipIntro = value;
	FILE* fp = fopen("arquivos/save/config.txt", "w");
	if (fp != NULL) {
		fprintf(fp, "%d\n", skipIntro ? 1 : 0);
		fclose(fp);
	}

}


void Game::AddToRanking(int score, int papers) {

	struct ranking scores[10];
	int tam = 0, i = 0, position = 0;
	FILE *fp;

	// Inicializa vetor de scores
	for (i = 0; i < 10; i++) {
		scores[i].papers = -1;
		scores[i].score = -1;
	}

	// Le scores que ja estao no arquivo
	fp = fopen("arquivos/save/highscores.txt", "r");
	if (fp != NULL) {
		while (fscanf(fp, "%d", &scores[tam].score) > 0) {
			fscanf(fp, "%d", &scores[tam].papers); // pega papers
			getc(fp); // pega "\n"
			tam++;
		}
		fclose(fp);
	}

	i = 0;
	if (tam > 0) {
		// Insere o valor na posicao correta
		while (i < tam) {
			if (score > scores[i].score) {
				position = i;
				if (tam < 10) {
					i = tam;
					while (i > position) {
						scores[i] = scores[i - 1];
						i = i - 1;
					}
				}
				break;
			}
			else if (score <= scores[i].score) {
				position = i + 1;
			}
			i++;
		}
		if (position <= tam) {
			if (tam < 10) {
				tam++;
			}
			scores[position].score = score;
			scores[position].papers = papers;
		}
		// Grava vetor de scores no arquivo
		fp = fopen("arquivos/save/highscores.txt", "w");
		if (fp != NULL) {
			for (i = 0; i < tam; i++) {
				fprintf(fp, "%d %d\n", scores[i].score, scores[i].papers);
			}
			fclose(fp);
		}
	}
	else {
		fp = fopen("arquivos/save/highscores.txt", "w");
		if (fp != NULL) {
			fprintf(fp, "%d %d\n", score, papers);
			fclose(fp);
		}
	}
	
}


std::string Game::CalculateScore(int papers) {

	std::stringstream s;

	if (papers == 0) {
		s << "Nota: SR";
	}
	else {
		if (papers > 0 && papers < 29) {
			s << "Nota: II";
		}
		else {
			if (papers >= 30 && papers < 49) {
				s << "Nota: MI";
			}
			else {
				if (papers >= 50 && papers < 69) {
					s << "Nota: MM";
				}
				else {
					if (papers >= 70 && papers < 89) {
						s << "Nota: MS";
					}
					else {
						if (papers >= 89) {
							s << "Nota: SS";
						}
					}
				}
			}
		}
	}
	return s.str();

}
