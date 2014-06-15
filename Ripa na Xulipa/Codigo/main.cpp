

#include "Game.h"
#include "TitleState.h"

int main (int argc, char* argv[]){
	Game jogo("RIPA NA XULIPA", 1024, 600);
	jogo.Push(new TitleState);
	jogo.Run();
	
	return 0;
}