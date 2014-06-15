
#include "Sound.h"

std::unordered_map<std::string, Mix_Chunk*> Sound::assetTable;

Sound::Sound(string file): Sound(){
	
	Open(file);
}

void Sound::Play(int times) {
	
	channel = Mix_PlayChannel(channel, chunk, times);
}

void Sound::Stop(){
	Mix_HaltChannel(channel);
}

void Sound::Open(string file) {
	
	chunk = Mix_LoadWAV(file.c_str());
}

bool Sound::IsOpen(){
	
	if (chunk)
		return true;
	
	return false;
}

void Sound::Clear(){
	
	while (assetTable.size() > 0){
		Mix_FreeChunk(assetTable.begin()->second);
		assetTable.erase(assetTable.begin());
	}
}