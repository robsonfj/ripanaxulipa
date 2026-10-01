// main.cpp - ponto de entrada: cria o Game e empilha o estado inicial.

#include <SDL_thread.h>

#include "Game.h"
#include "TitleState.h"
#include "InitState.h"


int main (int argc, char* argv[]) {

	Game jogo("RIPA NA XULIPA", 1024, 600);
	jogo.Push(new InitState);
	
	jogo.Run();
	
	return 0;

}
