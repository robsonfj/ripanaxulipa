// Game.h - janela SDL+OpenGL, pilha de estados e saves (moedas/ranking/config).

#ifndef __IDJ__Game__
#define __IDJ__Game__

#include <vector>
#include <memory>
#include <stack>
#include <ctime>
#include <string>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL_opengl.h>
#ifdef _WIN32
#include <windows.h>
#endif

#include "State.h"
#include "Timer.h"

using std::cout;
using std::string;

typedef struct ranking {
	int score;
	int papers;
}ranking;

class Game {

private:
	Timer timer;
	int width, height, frameStart = 0;
	float dt = 0;
	static Game* instance;
	State* storedState;
	std::stack<std::unique_ptr<State>> stateStack;

	void CalculateDeltaTime();
    	
public:
    SDL_Window* window;
    SDL_GLContext context, threadctx;
	int highScore = 0, volume = 100;
	bool showFPS = false, mute = false, skipIntro = false;

    Game(string title, int width, int height);
	~Game();
	
	float GetDeltaTime() { return dt; };
	static Game& GetInstance() { return *instance; };
	static State& GetCurrentState() { return *GetInstance().stateStack.top(); };
	void ResetView();
	int GetWindowWidth() { return width; };
	int GetWindowHeight() { return height; };
    void SetWindowWidth(float width) { this->width = width; };
    void SetWindowHeight(float height) { this->height =height; };
	void FPS();
	void Push(State* state) {
		if (storedState) {
			delete storedState; // segundo Push no mesmo frame: mantem o primeiro
		}
		storedState = state;
	};
//	Apenas para testes locais: move o estado pendente (Push) para o
//	topo da pilha sem rodar o loop. Nao usado pelo jogo.
	void FlushForTest() {
		if (storedState) {
			stateStack.emplace(storedState);
			storedState = nullptr;
		}
	};
	void Run();
	void SetMute();
	void SetSkipIntro(bool value);
	int GetHighScore();
//	Puros de arquivo (static de proposito: testaveis sem janela/SDL).
	static int GetCoins();
	static void SetCoins(int value);
	static void AddToRanking(int score, int papers);
	static std::string CalculateScore(int papers);
	
};

#endif /* defined(__IDJ__Game__) */
