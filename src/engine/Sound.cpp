// Sound.cpp - efeito sonoro (Mix_Chunk) com cache.

#include "Sound.h"
#include "Game.h"
#include "Sync.h"

std::unordered_map<std::string, Mix_Chunk*> Sound::assetTable;


Sound::Sound(string file) : Sound() {
	
	Open(file);

}


void Sound::Play(int times) {

    Mix_VolumeChunk(chunk, Game::GetInstance().volume);
	if (Game::GetInstance().mute) {
		Mix_VolumeChunk(chunk, 0);
	}    
	channel = Mix_PlayChannel(channel, chunk, times);

}


void Sound::Open(string file) {
	
	std::lock_guard<std::recursive_mutex> lock(g_gfxMutex);
//	Verifica se o som ja foi carregado
	if (assetTable.find(file) != assetTable.end()) {
		chunk = assetTable.find(file)->second;
	}
    else {
        chunk = Mix_LoadWAV(file.c_str());
        if (!IsOpen()) {
            std::cout << "O Som nao foi carregado: " << file << " (" << Mix_GetError() << ")" << std::endl;
        }
		else {
			assetTable.emplace(file, chunk);
		}
    }
}


bool Sound::IsOpen() {
	
	if (chunk) {
		return true;
	}
	return false;

}


