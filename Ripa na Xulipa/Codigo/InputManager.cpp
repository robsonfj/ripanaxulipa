
#include "InputManager.h"
#include "Camera.h"
#include "Game.h"


InputManager::InputManager() {

	for (int i = 0; i < N_MOUSEKEYS; i++) {
		mouseState[i] = RELEASED;
	}
	SDL_StopTextInput();

}


InputManager& InputManager::GetInstance() {

	static InputManager inputManager;
	return inputManager;

}


void InputManager::Update() {

	SDL_Event event;
	
	SDL_GetMouseState(&mouseX, &mouseY);
	mouseMoving = false;
	quitGame = false;
	
	for (int i = 0; i < N_MOUSEKEYS; i++) {
		if (mouseState[i] == JUST_PRESSED) {
			mouseState[i] = PRESSED;
		}
		if (mouseState[i] == JUST_RELEASED) {
			mouseState[i] = RELEASED;
		}
	}
	
//	Percorre automaticamente o unordered map keystate
	for (auto &tecla:keyState) {
		if (tecla.second == JUST_PRESSED) {
			tecla.second = PRESSED;
		}
		if (tecla.second == JUST_RELEASED) {
			tecla.second = RELEASED;
		}
	}
	
	while (SDL_PollEvent(&event)) {
		switch (event.type) {				
			case SDL_QUIT:
				quitGame = true;
				break;				
			case SDL_KEYDOWN:
//				Se a tecla nao estiver em keyState, adiciona
//				Se estiver, verifica qual foi apertada
				if (keyState.find(event.key.keysym.sym) == keyState.end()) {
					keyState.emplace(event.key.keysym.sym, JUST_PRESSED);
				}
				else {
					if (keyState.find(event.key.keysym.sym)->second != PRESSED) {
						keyState.find(event.key.keysym.sym)->second = JUST_PRESSED;
					}
				}
				break;				
			case SDL_KEYUP:				
				keyState.find(event.key.keysym.sym)->second = JUST_RELEASED;
				break;				
			case SDL_MOUSEBUTTONDOWN:				
				mouseState[event.button.button] = JUST_PRESSED;
				break;				
			case SDL_MOUSEBUTTONUP:				
				mouseState[event.button.button] = JUST_RELEASED;
				break;                
            case SDL_MOUSEMOTION:
//      	    Se o mouse estiver em movimento, seta para true
                mouseMoving = true;
                break;				
			case SDL_TEXTINPUT:
                inputTexto<<event.text.text;
                break;
		}		      
//      Evento de redimensionamento da tela
        if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
            SDL_Log("\nWindow resized to %dx%d\n",event.window.data1, event.window.data2);
            Game::GetInstance().SetWindowWidth(event.window.data1);
            Game::GetInstance().SetWindowHeight(event.window.data2);
        }
    }

}


std::string InputManager::Text() {

	std::string temp;
	temp = inputTexto.str();
	inputTexto.str("");
	return temp;

}


bool InputManager::KeyPress(int key) {
	
	if (keyState.find(key) != keyState.end()) {
		if (keyState.find(key)->second == JUST_PRESSED) {
			return true;
		}
	}
	return false;

}


bool InputManager::KeyRelease(int key) {
	
	if (keyState.find(key) != keyState.end()) {
		if (keyState.find(key)->second == JUST_RELEASED) {
			return true;
		}
	}
	return false;

}


bool InputManager::IsKeyDown(int key) {
	
	if (keyState.find(key) != keyState.end()) {
		if (keyState.find(key)->second == PRESSED) {
			return true;
		}
	}	
	return false;

}


bool InputManager::MousePress(int button) {
	
	if (mouseState[button] == JUST_PRESSED) {
		return true;
	}	
	return false;

}


bool InputManager::MouseRelease(int button) {
	
	if (mouseState[button] == JUST_RELEASED) {
		return true;
	}	
	return false;

}


bool InputManager::IsMouseDown(int button) {
	
	if (mouseState[button] == PRESSED) {
		return true;
	}
	return false;

}


// Verifica se o mouse esta dentro do rect considerando a posicao da camera
bool InputManager::IsMouseInside(Rect rect) {
	
	return rect.IsInside(mouseX, mouseY);

}

