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
    
    sp.SetScaleX(scale/(seg->GetZ_World1()));
    sp.SetScaleY(scale/(seg->GetZ_World1()));
    box.x = pos.x = x;
    box.y = pos.y = y;
    box.w = sp.GetWidth();
    box.h = sp.GetHeight();
    
}

void ObjPespectiva::Update(float dt) {
    
    sp.SetScaleX(scale/(seg->GetZ_World1()));
    sp.SetScaleY(scale/(seg->GetZ_World1()));
    
    box.x = seg->GetScreenRect1().x - sp.GetWidth()/2 + seg->GetScreenRect1().w*pos.x;
    box.y = seg->GetScreenRect1().y - sp.GetHeight()*pos.y;
    box.w = sp.GetWidth();
    box.h = sp.GetHeight();
    
    sp.SetScaleX(scale/(seg->GetZ_World2()));
    sp.SetScaleY(scale/(seg->GetZ_World2()));
    
    box2.x = seg->GetScreenRect2().x - sp.GetWidth()/2 + seg->GetScreenRect2().w*pos.x;
    box2.y = seg->GetScreenRect2().y - sp.GetHeight()*pos.y;
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glBegin(GL_QUADS);
//	ponto superior esquerdo
    glTexCoord4f(0, -1.5, 0, escala);
    glVertex2f(box2.x, box2.y);
    
//	ponto superior direito
    glTexCoord4f(escala, -1.5, 0, escala);
    glVertex2f( box2.x + box2.w, box2.y);
    
//	ponto inferior direito
    glTexCoord4f(1, 1, 0, 1);
    glVertex2f(box.x+ box.w, box.y+box.h);
    
//	ponto inferior esquerdo
    glTexCoord4f(0, 1, 0, 1);
    glVertex2f(box.x, box.y+box.h);
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