// Sound.h - efeito sonoro (Mix_Chunk) com cache.

#ifndef __IDJ__Sound__
#define __IDJ__Sound__

#include <iostream>
#include <string>
#include <unordered_map>
#include <SDL_mixer.h>

using std::string;

class Sound {

private:
	string file;
	Mix_Chunk* chunk;
	int channel = 0;
	static std::unordered_map<std::string, Mix_Chunk*> assetTable;
	
public:
	Sound() {chunk = NULL;};
	Sound(string file);
    
	void Play (int times);
	void Open (string file);
	bool IsOpen ();

};

#endif /* defined(__IDJ__Sound__) */
