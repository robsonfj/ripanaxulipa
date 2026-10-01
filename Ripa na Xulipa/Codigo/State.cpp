
#include "State.h"
#include "Game.h"

int State::deletedpapercount;


State::State() {

	requestDelete = false;
	requestQuit = false;
	// Seta cor amarela
	padraoCorSelect.r = 236;
	padraoCorSelect.g = 165;
	padraoCorSelect.b = 42;
	// Seta cor vermelha
	padraoCorSelect2.r = 223;
	padraoCorSelect2.g = 55;
	padraoCorSelect2.b = 38;
	// Seta cor preta
	padraoCor.r = padraoCor.g = padraoCor.b = 0;
    deletedpapercount = 0;

}


void State::UpdateArray(float dt) {
	
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

	for (int i = objectArray.size() - 1; i >= 0; i--) {
		if (objectArray[i]) {
			objectArray[i]->Render();
		}
	}

}