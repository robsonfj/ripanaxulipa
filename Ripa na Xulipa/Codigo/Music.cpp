
#include "Music.h"

std::unordered_map<std::string, Mix_Music*> Music::assetTable;


Music::Music(string file): Music() {
	
	Open(file);

}


void Music::Play(int times) {

	Mix_VolumeMusic(Game::GetInstance().volume);
	if (Game::GetInstance().mute) {
		Mix_VolumeMusic(0);
	}	
    Mix_PlayMusic(music, times);

}


void Music::Stop() {

	Mix_FadeOutMusic(500);

}


void Music::Stop(int fadeout) {

	Mix_FadeOutMusic(fadeout);

}


void Music::Open(string file) {
	
//	Verifica se a musica ja foi carregada
	if (assetTable.find(file) != assetTable.end()) {
		music = assetTable.find(file)->second;
	}	
	else {
		music = Mix_LoadMUS(file.c_str());
		// Fallback Windows: vcpkg sem mpg123 nao toca MP3.
		// Convertemos os MP3 para OGG (mesmo nome) — tenta .ogg.
		if (!IsOpen() && file.size() > 4 &&
			file.compare(file.size() - 4, 4, ".mp3") == 0) {
			string oggFile = file.substr(0, file.size() - 4) + ".ogg";
			music = Mix_LoadMUS(oggFile.c_str());
			if (IsOpen()) {
				assetTable.emplace(file, music);
				return;
			}
		}
		if (!IsOpen()) {
			std::cout << "A musica nao foi carregada: " << file << " (" << Mix_GetError() << ")" << std::endl;
		}
		else {
			assetTable.emplace(file, music);
		}
	}

}


bool Music::IsOpen() {
	
	if (music) {
		return true;
	}	
	return false;

}


bool Music::IsPlaying() {

	Mix_VolumeMusic(Game::GetInstance().volume);
	if (Game::GetInstance().mute) {
		Mix_VolumeMusic(0);
	}    
	if (Mix_PlayingMusic()) {
		return true;
	}
	return false;

}


void Music::Clear() {
	
	while (assetTable.size() > 0) {
		Mix_FreeMusic(assetTable.begin()->second);
		assetTable.erase(assetTable.begin());
	}

}