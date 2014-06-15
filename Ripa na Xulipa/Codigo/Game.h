

#ifndef __IDJ__Game__
#define __IDJ__Game__

#include <vector>
#include <memory>
#include <stack>
#include <ctime>
#include <string>
#include <sstream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL_opengl.h>

#include "State.h"
#include "Timer.h"

using std::cout;
using std::string;

class Game{
	Timer timer;
	int width, height;
	int frameStart = 0;
	float dt = 0;
	void CalculateDeltaTime();
	
	static Game* instance;
	State* storedState;
	
	SDL_Window* window;
	SDL_GLContext context;
	
	std::stack<std::unique_ptr<State>> stateStack;
	
public:
	int highScore;
	int coins;

	Game(string title, int width, int height);
	~Game();
	
	float GetDeltaTime(){return dt;};
	static Game& GetInstance(){return *instance;};
	static State& GetCurrentState(){return *GetInstance().stateStack.top();};
	
	void ResetView();
	int GetWindowWidth(){return width;};
	int GetWindowHeight(){return height;};
	void FPS();
	void Push(State* state){storedState = state;};
	void Run();

	int volume = 100;
	bool mute = true;
	void SetMute();
	int GetHighScore();
	int GetCoins();
	void SetCoins(int value);
	
};

#endif /* defined(__IDJ__Game__) */
