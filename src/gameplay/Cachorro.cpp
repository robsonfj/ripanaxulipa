// Cachorro.cpp - cachorro que acompanha a pista em perspectiva.

#include "Cachorro.h"
#include "Road.h"


Cachorro::Cachorro(float x, float y, float scale) : sp("arquivos/img/Personagens/spritecachorro.png", 6, 40) {
    
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
//	Evita ler fora do vetor quando segmentos sao removidos da frente:
//	i cresce sem teto e segmentos encolhe.
    int n = (int)Road::segmentos.size();
    if (n <= 1) {
        return;
    }
    if (i >= n) {
        i = n - 1;
    }
    if (i < 1) {
        i = 1;
    }
    RoadSegment* ref = Road::SegmentAt(i);
    if (!ref) {
        return;
    }
    sp.SetScaleX(scale / i);
    sp.SetScaleY(scale / i);
    box.x = ref->GetScreenRect1().x - sp.GetWidth() + ref->GetScreenRect1().w * 0.8;
    box.y = ref->GetScreenRect1().y;
    
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