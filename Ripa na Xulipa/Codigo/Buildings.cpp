
#include "Buildings.h"
#include "Road.h"


Buildings::Buildings(string pred1, string pred2, int segpos1, int segpos2, bool isMirror, float scale) : sp1(pred1), sp2(pred2) {
	
	this->seg1 = Road::segmentos[segpos1];
	this->seg2 = Road::segmentos[segpos2];
	this->isMirror = isMirror;
    this->scale = scale;
	
}


void Buildings::Update(float dt) {

    if (seg1) {
        sp1.SetScaleX(scale * seg1->GetScreenRect1().w / Game::GetInstance().GetWindowHeight() / 2);
        sp1.SetScaleY(scale * seg1->GetScreenRect1().w / Game::GetInstance().GetWindowHeight() / 2);
    }
	sp2.SetScaleX(scale * seg2->GetScreenRect1().w / Game::GetInstance().GetWindowHeight() / 2);
	sp2.SetScaleY(scale * seg2->GetScreenRect1().w / Game::GetInstance().GetWindowHeight() / 2);
    
	if (isMirror) {
        if (seg1) {
            box1.x = seg1->GetScreenRect1().x - sp1.GetWidth();
            box1.y = seg1->GetScreenRect1().y;
            box1.w = -seg1->GetScreenRect1().w;
        }
        box.w = sp1.GetWidth(); // O box.w esta sendo usado para armazenar um valor temporario
        
        box2.x = seg2->GetScreenRect1().x;
        box2.y = seg2->GetScreenRect1().y;
        box2.w = -seg2->GetScreenRect1().w;
        
    }
    else {
        if (seg1) {
            box1.x = seg1->GetScreenRect1().x;
            box1.y = seg1->GetScreenRect1().y;
            box1.w = seg1->GetScreenRect1().w;
        }
        box.w = 0;
        
        box2.x = seg2->GetScreenRect1().x;
        box2.y = seg2->GetScreenRect1().y;
        box2.w = seg2->GetScreenRect1().w;
    }
	
}


void Buildings::Render() {

	float escala = box1.w / box2.w;
    glFlush();
	glBindTexture(GL_TEXTURE_2D, *sp1.GetTexture());
    glPushMatrix();
	glBegin(GL_QUADS);

//	Ponto superior esquerdo
	glTexCoord4f(0, 0, 0, 1);
	glVertex2f(box1.x + box1.w, box1.y - sp1.GetHeight());
	
//	Ponto superior direito
	glTexCoord4f(1, 0, 0, 1);
	glVertex2f( box1.x + sp1.GetWidth()  + box1.w, box1.y - sp1.GetHeight());
	
//	Ponto inferior direito
	glTexCoord4f(1, 1, 0, 1);
	glVertex2f( box1.x + sp1.GetWidth() + box1.w, box1.y);
	
//	Ponto inferior esquerdo
	glTexCoord4f(0, 1, 0, 1);
	glVertex2f(box1.x + box1.w, box1.y);
	glEnd();
	
	
	glBindTexture(GL_TEXTURE_2D, *sp2.GetTexture());
	glBegin(GL_QUADS);
//	Ponto superior esquerdo
	glTexCoord4f(0, 0, 0, 1);
	glVertex2f( box2.x + box2.w, box2.y - sp2.GetHeight());
	
//	Ponto superior direito
	glTexCoord4f(escala, 0, 0, escala);
	glVertex2f( box1.x + box1.w + box.w, box1.y - sp1.GetHeight());
	
//	Ponto inferior direito
	glTexCoord4f(escala, escala, 0, escala);
	glVertex2f( box1.x + box1.w + box.w, box1.y );
	
//	Ponto inferior esquerdo
	glTexCoord4f(0, 1, 0, 1);
	glVertex2f( box2.x + box2.w, box2.y );
	glEnd();
    glPopMatrix();
    glFinish();

}


bool Buildings::IsDead() {
	
	if (seg2->IsDead()) {
		return true;
	}	
	return false;

}

bool Buildings::Is(string type) {
	
	if (type == "building") {
		return true;
	}	
	return false;

}

