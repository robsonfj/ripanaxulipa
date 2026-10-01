
#include <SDL2/SDL_thread.h>

#include "Game.h"
#include "TitleState.h"
#include "InitState.h"


int main (int argc, char* argv[]) {

	Game jogo("RIPA NA XULIPA", 1024, 600);
	jogo.Push(new InitState);
	
	jogo.Run();
	
	return 0;

}
