
#ifndef __IDJ__Sound__
#define __IDJ__Sound__

#include <iostream>
#include <string>
#include <unordered_map>
#include <SDL2/SDL_mixer.h>

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
    
    bool IsPlaying();
	void Play (int times);
	void Stop ();
	void Open (string file);
	bool IsOpen ();
	static void Clear();

};

#endif /* defined(__IDJ__Sound__) */
