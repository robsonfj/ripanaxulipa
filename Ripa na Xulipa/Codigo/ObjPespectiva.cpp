//
//  ObjPespectiva.cpp
//  Avenida Paulista
//
//  Created by Robson Ferreira Jacomini on 14/06/14.
//
//

#include "ObjPespectiva.h"

ObjPespectiva::ObjPespectiva(string file, int segpos, float x, float y, float scale): sp(file) {
    
    this->scale = scale;
    this->seg = Road::segmentos[segpos];
    
    sp.SetScaleX(scale * seg->GetScreenRect1().w/Game::GetInstance().GetWindowHeight()/2);
    sp.SetScaleY(scale * seg->GetScreenRect1().w/Game::GetInstance().GetWindowHeight()/2);
    box.x = pos.x = x;
    box.y = pos.y = y;
    box.w = sp.GetWidth();
    box.h = sp.GetHeight();
    
}

void ObjPespectiva::Update(float dt) {
    
    sp.SetScaleX(scale * seg->GetScreenRect1().w/Game::GetInstance().GetWindowHeight()/2);
    sp.SetScaleY(scale * seg->GetScreenRect1().w/Game::GetInstance().GetWindowHeight()/2);
    
    box.x = seg->GetScreenRect1().x - sp.GetWidth() + seg->GetScreenRect1().w*pos.x;
    box.y = seg->GetScreenRect1().y - sp.GetHeight() +pos.y;
    box.w = sp.GetWidth();
    box.h = sp.GetHeight();
    
    sp.SetScaleX(scale * seg->GetScreenRect2().w/Game::GetInstance().GetWindowHeight()/2);
    sp.SetScaleY(scale * seg->GetScreenRect2().w/Game::GetInstance().GetWindowHeight()/2);
    
    box2.x = seg->GetScreenRect2().x - sp.GetWidth() + seg->GetScreenRect2().w*pos.x;
    box2.y = seg->GetScreenRect2().y - sp.GetHeight() +seg->GetScreenRect2().w*pos.y;
    box2.w = sp.GetWidth();
    box2.h = sp.GetHeight();
    
//	habilita ou desabilita a colisao de acordo com a distancia do objeto
    if(seg == Road::segmentos[1])
        actCollision = true;
    else
        actCollision = false;
    
    
}

void ObjPespectiva::Render(){
    float escala = seg->GetScreenRect2().w/seg->GetScreenRect1().w;
    
    glBindTexture(GL_TEXTURE_2D, sp.GetTexture());
    
    glBegin(GL_QUADS);
//	ponto superior esquerdo
    glTexCoord4f(0, 0, 0, escala);
    glVertex2f(box2.x, box2.y);
    
//	ponto superior direito
    glTexCoord4f(escala, 0, 0, escala);
    glVertex2f( box2.x + box2.w, box2.y);
    
//	ponto inferior direito
    glTexCoord4f(1, 1, 0, 1);
    glVertex2f(box.x+ box.w, box.y);
    
//	ponto inferior esquerdo
    glTexCoord4f(0, 1, 0, 1);
    glVertex2f(box.x, box.y);
    glEnd();
    
}

bool ObjPespectiva::Is(string type){
    
    if (type == "buraco")
        return true;
    
    return false;
}

bool ObjPespectiva::IsDead(){
    
    if (seg->IsDead() || isDead)
        return true;
    
    return false;
    
}