
#ifndef __IDJ__Music__
#define __IDJ__Music__

#include <iostream>
#include <string>
#include <unordered_map>
#include <SDL2/SDL_mixer.h>

#include "Game.h"

using std::string;

class Music {

private:
	string file;
	Mix_Music* music;
	static std::unordered_map<std::string, Mix_Music*> assetTable;

public:
	Music() {music = NULL;};
	Music(string file);
	
	void Play(int times);
	void Stop();
	void Stop(int fadeout);
	void Open(string file);
	bool IsOpen();
	bool IsPlaying();
	static void Clear();

};

#endif /* defined(__IDJ__Music__) */
