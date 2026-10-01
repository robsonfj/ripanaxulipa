
#include "Sound.h"
#include "Game.h"

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


void Sound::Stop() {

	Mix_HaltChannel(channel);

}


void Sound::Open(string file) {
	
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


bool Sound::IsPlaying() {

    Mix_VolumeChunk(chunk, Game::GetInstance().volume);
	if (Game::GetInstance().mute) {
		Mix_VolumeChunk(chunk, 0);
	}
	if (Mix_Playing(channel)) {
		return true;
	}    
    return false;

}


void Sound::Clear() {
	
	while (assetTable.size() > 0) {
		Mix_FreeChunk(assetTable.begin()->second);
		assetTable.erase(assetTable.begin());
	}

}