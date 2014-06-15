

#include "State.h"
#include "Game.h"

int State::deletedpapercount;

State::State(){
	requestDelete = false;
	requestQuit = false;
	padraoCorSelect.r = padraoCorSelect.g = padraoCorSelect.b = 150;
	padraoCor.r = padraoCor.g = padraoCor.b = 50;
    deletedpapercount = 0;
}

void State::UpdateArray(float dt){
	
	for (int i = 0; i < objectArray.size(); i++) {
		
//		somente verifica colisao quando estiver ativa
		if(objectArray[i]->actCollision){
			for (auto &obj:objectArray) {
				if(!objectArray[i]->Is("poste")){
					if(Collision::IsColliding(objectArray[i]->box + Camera::pos, obj->box + Camera::pos, objectArray[i]->rotation, obj->rotation)){
						objectArray[i]->NotifyCollision(*obj);
						obj->NotifyCollision(*objectArray[i]);
					}
				}
			}
		}
//		chama update do objeto
		objectArray[i]->Update(dt);
		
//  	se retorna true ele deleta o objeto do array
        if (objectArray[i]->IsDead()){
            if (objectArray[i]->Is("paper"))
                deletedpapercount += 1;
            
            objectArray.erase(objectArray.begin() + i);
            i--;
        }
    }
}

void State::RenderArray(){

	for (int i = objectArray.size()-1; i >= 0; i--)
		objectArray[i]->Render();
	
}



