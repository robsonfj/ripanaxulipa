
#include "Cachorro.h"
#include "Road.h"


Cachorro::Cachorro(float x, float y, float scale) : sp("arquivos/img/personagens/spritecachorro.png", 6, 40) {
    
    this->scale = scale;
    sp.SetScaleX(scale / i);
    sp.SetScaleY(scale / i);
    
    box.x = x - sp.GetWidth() / 2;
    box.y = y - sp.GetHeight() / 2;

}


void Cachorro::Update(float dt) {

    sp.Update(dt);
    timer.Update(dt*1000);
    if (timer.Get() > 80) {
        i++;
        timer.Restart();
    }
    sp.SetScaleX(scale / i);
    sp.SetScaleY(scale / i);
    box.x = Road::segmentos[i]->GetScreenRect1().x - sp.GetWidth() + Road::segmentos[i]->GetScreenRect1().w * 0.8;
    box.y = Road::segmentos[i]->GetScreenRect1().y;
    
}


void Cachorro::Render() {

    sp.Render(box.x, box.y);
    
}


bool Cachorro::IsDead() {
    
	if (box.y < 365) {
		return true;
	}
	else {
		return false;
	}

}


bool Cachorro::Is(string type) {

    return false;

}