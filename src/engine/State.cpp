// State.cpp - estado base: update/render do array e deteccao de colisao simples.

#include "State.h"
#include "Game.h"
#include "Sync.h"

int State::deletedpapercount;


State::State() {

	requestDelete = false;
	requestQuit = false;
	// Seta cor branca (hover com contraste)
	padraoCorSelect.r = padraoCorSelect.g = padraoCorSelect.b = 255;
	// Seta cor branca (hover com contraste)
	padraoCorSelect2.r = padraoCorSelect2.g = padraoCorSelect2.b = 255;
	// Seta cor preta
	padraoCor.r = padraoCor.g = padraoCor.b = 0;
    deletedpapercount = 0;

}


void State::UpdateArray(float dt) {
	
	std::lock_guard<std::recursive_mutex> lock(g_gfxMutex);
	for (int i = 0; i < objectArray.size(); i++) {
        if (objectArray[i]->actCollision) {
            for (auto &obj:objectArray) {
                if (objectArray[i]->lanex == obj->lanex || objectArray[i]->laney == obj->laney) {
                    objectArray[i]->NotifyCollision(*obj);
                    obj->NotifyCollision(*objectArray[i]);
                }
            }
        }
//		Chama update do objeto
		objectArray[i]->Update(dt);
//  	Se retorna true ele deleta o objeto do array
        if (objectArray[i]->IsDead()) {
			if (objectArray[i]->Is("paper")) {
				deletedpapercount += 1;
			}
            objectArray.erase(objectArray.begin() + i);
            i--;
        }
    }

}


void State::RenderArray() {
	
	std::lock_guard<std::recursive_mutex> lock(g_gfxMutex);
	for (int i = objectArray.size() - 1; i >= 0; i--) {
		if (objectArray[i]) {
			objectArray[i]->Render();
		}
	}

}