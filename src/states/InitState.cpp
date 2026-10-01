// InitState.cpp - splash rapido do logo antes do menu.

#include "InitState.h"


// Tela de abertura rapida: mostra o logo e segue para o menu.
// A historia (cenas 1-13) passou para o StoryState, exibido no NEW GAME.
#define SPLASH_TIME 4.0f


InitState::InitState(): logoMusic("arquivos/audio/logosong.ogg") {
	
	logo = *new Sprite("arquivos/img/states/init/patotinha.jpg");
	
	if (!logoMusic.IsPlaying()) {
        logoMusic.Play(1);
    }
}


void InitState::Input() {

//	Se a condicao de saida for atendida
	if ((InputManager::GetInstance().KeyPress(ESCAPE_KEY)) || (InputManager::GetInstance().ShouldQuit())) {
		requestQuit = true;
	}

//	ENTER pula o logo imediatamente
	if (InputManager::GetInstance().KeyPress(ENTER_KEY)) {
		GoToMenu();
	}

}


void InitState::GoToMenu() {

	logoMusic.Stop();
	requestDelete = true;
	Game::GetInstance().Push(new TitleState);

}


void InitState::Update(float dt) {

	timer.Update(dt);
    Input();

   

//	Avanca sozinho para o menu apos o tempo do splash
	if (timer.Get() > SPLASH_TIME) {
		GoToMenu();
	}

}


void InitState::Render() {

	logo.Render();

}
