

#include "Objetos.h"

Objetos::Objetos(string tipo, string file, int segpos, float x, float y, float scale): spObj(file) {
	
    this->tipo = tipo;
	this->scale = scale;
	this->seg = Road::segmentos[segpos];
	
	spObj.SetScaleX(scale * seg->GetScreenRect1().w/Game::GetInstance().GetWindowHeight()/2);
	spObj.SetScaleY(scale * seg->GetScreenRect1().w/Game::GetInstance().GetWindowHeight()/2);
	box.x = pos.x = x;
	box.y = pos.y = y;
	box.w = spObj.GetWidth();
	box.h = spObj.GetHeight();
	
}

void Objetos::Update(float dt) {
	
	spObj.SetScaleX(scale * seg->GetScreenRect1().w/Game::GetInstance().GetWindowHeight()/2);
	spObj.SetScaleY(scale * seg->GetScreenRect1().w/Game::GetInstance().GetWindowHeight()/2);

	box.x = seg->GetScreenRect1().x - spObj.GetWidth()/2 + seg->GetScreenRect1().w*pos.x;
	box.y = seg->GetScreenRect1().y - spObj.GetHeight() * pos.y;
	box.w = spObj.GetWidth();
	box.h = spObj.GetHeight();
	
//	habilita ou desabilita a colisao de acordo com a distancia do objeto
    if(seg == Road::segmentos[2])
		actCollision = true;
	else
		actCollision = false;
    

}

void Objetos::Render(){
	
	spObj.Render(box.x, box.y);
}

void Objetos::NotifyCollision (GameObject& other){

    if (other.Is("personagem") && actCollision == true) {
        if (tipo == "paper" ) {
            isDead = true;
        }
    }
}

bool Objetos::Is(string type){
	
	if (type == tipo)
		return true;
	
	return false;
}

bool Objetos::IsDead(){
	
	if (seg->IsDead() || isDead)
		return true;
	
	return false;

}
