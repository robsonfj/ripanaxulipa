
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
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_opengl.h>
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
	int highScore = 0, coins = 0, volume = 100;
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
	void Push(State* state) { storedState = state; };
	void Run();
	void SetMute();
	void SetSkipIntro(bool value);
	int GetHighScore();
	int GetCoins();
	void SetCoins(int value);
	void AddToRanking(int score, int papers);
	std::string CalculateScore(int papers);
	
};

#endif /* defined(__IDJ__Game__) */
