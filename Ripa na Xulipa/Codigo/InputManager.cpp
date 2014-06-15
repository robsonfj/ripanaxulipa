

#include "InputManager.h"
#include "Camera.h"

InputManager::InputManager(){
	for (int i = 0; i < N_MOUSEKEYS; i++)
		mouseState[i] = RELEASED;
}

InputManager& InputManager::GetInstance(){
	static InputManager inputManager;
	
	return inputManager;
}

void InputManager::Update(){
	SDL_Event event;
//	SDL_MouseMotionEvent *mousemotion = NULL;
	
	SDL_GetMouseState(&mouseX, &mouseY);
	mouseMoving = false;
	quitGame = false;
	
	for (int i = 0; i < N_MOUSEKEYS; i++){
		if (mouseState[i] == JUST_PRESSED)
			mouseState[i] = PRESSED;
		
		if (mouseState[i] == JUST_RELEASED)
			mouseState[i] = RELEASED;
	}
	
//	percorre automaticamente o unordered map keystate
	for (auto &tecla:keyState) {
		if (tecla.second == JUST_PRESSED)
			tecla.second = PRESSED;
		
		if (tecla.second == JUST_RELEASED)
			tecla.second = RELEASED;
	}
	
//	se o mouse estiver em movimento seta para true
//	if (mousemotion->which == SDL_MOUSEMOTION)
//		mouseMoving = true;
	
	
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
				
			case SDL_QUIT:
				quitGame = true;
				break;
				
			case SDL_KEYDOWN:
//				se a tecla nao estiver em keyState adiciona
//				se estiver verifica qual foi apertada
				if (keyState.find(event.key.keysym.sym) == keyState.end())
					keyState.emplace(event.key.keysym.sym, JUST_PRESSED);
				
				else{
					if (keyState.find(event.key.keysym.sym)->second != PRESSED)
						keyState.find(event.key.keysym.sym)->second = JUST_PRESSED;
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
		}
	}
}

bool InputManager::KeyPress (int key){
	
	if (keyState.find(key) != keyState.end())
		if (keyState.find(key)->second == JUST_PRESSED )
			return true;
	
	return false;
}

bool InputManager::KeyRelease (int key){
	
	if (keyState.find(key) != keyState.end())
		if (keyState.find(key)->second == JUST_RELEASED )
			return true;
	
	return false;
}

bool InputManager::IsKeyDown (int key){
	
	if (keyState.find(key) != keyState.end())
		if (keyState.find(key)->second == PRESSED)
			return true;
	
	return false;
}

bool InputManager::MousePress (int button){
	
	if (mouseState[button] == JUST_PRESSED)
		return true;
	
	return false;
}

bool InputManager::MouseRelease (int button){
	
	if (mouseState[button] == JUST_RELEASED)
		return true;
	
	return false;
}

bool InputManager::IsMouseDown (int button){
	
	if (mouseState[button] == PRESSED)
		return true;
	
	return false;
}

//verifica se o mouse esta dentro do rect ajustando com a posicao da camera
bool InputManager::IsMouseInside(Rect rect){
	
	return rect.IsInside(mouseX + Camera::pos.x, mouseY + Camera::pos.y);
}

