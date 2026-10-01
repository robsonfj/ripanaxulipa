
#include "ObjPespectiva.h"
#include "MainCharacter.h"


ObjPespectiva::ObjPespectiva(string file, int segpos, float x, float y, float scale) : sp(file) {

    this->scale = scale;
    this->seg = Road::segmentos[segpos];
    jumpOver = true;
    
    SFX = new Sound("arquivos/audio/Efeitchos Sonoricos/falls.wav");
	    
    sp.SetScaleX(scale/(seg->GetZ_World1()));
    sp.SetScaleY(scale/(seg->GetZ_World1()));
    
    box.x = pos.x = x;
    box.y = pos.y = y;
    box.w = sp.GetWidth();
    box.h = sp.GetHeight();
    
//  Atribuicao da lane para os objetos
	if (x >= 0.2 && x <= 0.4) {
		lanex = 0;
	}
	if (x >= 0.6 && x <= 0.7) {
		lanex = 1;
	}
	if (x >= 0.8 && x <= 0.9) {
		lanex = 2;
	}
        
}


void ObjPespectiva::Update(float dt) {
    
    sp.SetScaleX(scale / (seg->GetZ_World1()));
    sp.SetScaleY(scale / (seg->GetZ_World1()));
    
    box.x = seg->GetScreenRect1().x - sp.GetWidth() / 2 + seg->GetScreenRect1().w * pos.x;
    box.y = seg->GetScreenRect1().y - sp.GetHeight() * pos.y;
    box.w = sp.GetWidth();
    box.h = sp.GetHeight();
    
    sp.SetScaleX(scale / (seg->GetZ_World2()));
    sp.SetScaleY(scale / (seg->GetZ_World2()));
    
    box2.x = seg->GetScreenRect2().x - sp.GetWidth() / 2 + seg->GetScreenRect2().w * pos.x;
    box2.y = seg->GetScreenRect2().y - sp.GetHeight() * pos.y;
    box2.w = sp.GetWidth();
    box2.h = sp.GetHeight();

//	Habilita ou desabilita a colisao de acordo com a distancia do objeto
	if (seg == Road::segmentos[3]) {
		actCollision = true;
	}
	else {
		actCollision = false;
	}
    
}


void ObjPespectiva::Render() {

    float escala = seg->GetScreenRect2().w / seg->GetScreenRect1().w;
    
    glBindTexture(GL_TEXTURE_2D, *sp.GetTexture());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glBegin(GL_QUADS);

//	Ponto superior esquerdo
    glTexCoord4f(0, -2, 0, escala);
    glVertex2f(box2.x, box2.y);
    
//	Ponto superior direito
    glTexCoord4f(escala, -2, 0, escala);
    glVertex2f( box2.x + box2.w, box2.y);
    
//	Ponto inferior direito
    glTexCoord4f(1, 1, 0, 1);
    glVertex2f(box.x+ box.w, box.y+box.h);
    
//	Ponto inferior esquerdo
    glTexCoord4f(0, 1, 0, 1);
    glVertex2f(box.x, box.y+box.h);
    glEnd();
    
}


void ObjPespectiva::NotifyCollision(GameObject& other){

	if (other.Is("personagem") && actCollision == true) {
		if (!MainCharacter::player->isjumping) {
			isDead = true;
			if (SFX) {
				SFX->Play(0);
			}
		}
	}
}


bool ObjPespectiva::Is(string type) {
    
	if (type == "buraco") {
		return true;
	}    
    return false;

}


bool ObjPespectiva::IsDead() {
    
	if (seg->IsDead() || isDead) {
		return true;
	}
    return false;
    
}