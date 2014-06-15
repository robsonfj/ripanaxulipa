
#include "Music.h"


std::unordered_map<std::string, Mix_Music*> Music::assetTable;


Music::Music(string file): Music(){
	
	Open(file);
}

void Music::Play(int times) {
	Mix_VolumeMusic(Game::GetInstance().volume);
	if (Game::GetInstance().mute)
		Mix_VolumeMusic(0);
	
	Mix_PlayMusic(music, times);
	
}

void Music::Stop(){
	Mix_FadeOutMusic(500);
}

void Music::Open(string file) {
	
//	procura se a musica ja foi carregada
	if(assetTable.find(file) != assetTable.end())
		music = assetTable.find(file)->second;
	
	else{
		
		music = Mix_LoadMUS(file.c_str());
		
		if (!IsOpen()){
			std::cout<<"A musica nao foi carregada: "<< file<<std::endl;
		}
		else
			assetTable.emplace(file, music);
	}
}


bool Music::IsOpen(){
	
	if (music)
		return true;
	
	return false;
}

bool Music::IsPlaying(){
	Mix_VolumeMusic(Game::GetInstance().volume);
	if (Game::GetInstance().mute)
		Mix_VolumeMusic(0);

	if (Mix_PlayingMusic())
		return true;
	
	return false;
}

void Music::Clear(){
	
	while (assetTable.size() > 0){
		Mix_FreeMusic(assetTable.begin()->second);
		assetTable.erase(assetTable.begin());
	}
}