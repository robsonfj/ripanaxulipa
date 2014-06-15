//
//  Nuvem.cpp
//  Avenida Paulista
//
//  Created by Robson Ferreira Jacomini on 14/06/14.
//
//

#include "Nuvem.h"


Nuvem::Nuvem(float x, float y){
    
    s<<"arquivos/img/Fundo/Nuvem_"<<(1+rand()%5)<<".png";
    sp = *new Sprite(s.str());
    
    scale = (3+rand()% 8);
    sp.SetScaleX(scale/10);
    sp.SetScaleY(scale/10);
    sp.SetAlpha((float)(7+rand()% 3)/10);
    
    speed = scale*(3+rand()% 10)/10;
    pos.x = x;
    pos.y = y;
}

void Nuvem::Update(float dt){
    pos.x+=speed*dt;
    if (pos.x > 1024) {
        s.str("");
        s<<"arquivos/img/Fundo/Nuvem_"<<(1+rand()% 5)<<".png";
        sp.Open(s.str());
        pos.x = -sp.GetWidth();
    }
    
}

void Nuvem::Render(){
    sp.Render(pos.x, pos.y);
    
}