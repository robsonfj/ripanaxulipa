// InputManager.h - teclado/mouse centralizado com borda de pressionamento.

#ifndef __IDJ__InputManager__
#define __IDJ__InputManager__

#include <sstream>
#include <string>
#include <SDL.h>
#include <unordered_map>

#include "Rect.h"

#define N_MOUSEKEYS 5
#define LEFT_ARROW_KEY SDLK_LEFT
#define RIGHT_ARROW_KEY SDLK_RIGHT
#define UP_ARROW_KEY SDLK_UP
#define DOWN_ARROW_KEY SDLK_DOWN
#define ESCAPE_KEY SDLK_ESCAPE
#define ENTER_KEY SDLK_RETURN
#define SPACE_KEY SDLK_SPACE
#define LEFT_MOUSE_BUTTON SDL_BUTTON_LEFT
#define RIGHT_MOUSE_BUTTON SDL_BUTTON_RIGHT
#define W_KEY SDLK_w
#define S_KEY SDLK_s
#define A_KEY SDLK_a
#define D_KEY SDLK_d

class InputManager {

private:
	enum InputState{
		RELEASED, JUST_RELEASED,
		PRESSED, JUST_PRESSED
	};
	bool quitGame = false;
	int mouseX, mouseY;
//	Ultima posicao que contou como movimento (debounce): jitter de
//	1-2px nao desabilita a navegacao por teclado dos menus.
	int lastMoveX = -10000, lastMoveY = -10000;
	InputState mouseState[N_MOUSEKEYS];
	std::unordered_map<int, InputState> keyState;

	InputManager();
	~InputManager() {};
	
public:
	std::stringstream inputTexto;
	
	void Update();
	bool KeyPress(int key);
	bool KeyRelease(int key); // borda de subida da soltura (usado em testes e menus)
	bool IsKeyDown(int key);
	bool MousePress(int button);
	bool IsMouseInside(Rect rect);
	bool ShouldQuit() { return quitGame; };
	bool mouseMoving = false;
	std::string Text();
	static InputManager& GetInstance();
	
};

#endif /* defined(__IDJ__InputManager__) */
