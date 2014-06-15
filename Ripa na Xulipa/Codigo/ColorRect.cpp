//
//  ColorRect.cpp
//  Avenida Paulista
//
//  Created by Robson Ferreira Jacomini on 12/06/14.
//
//

#include "ColorRect.h"

ColorRect::ColorRect(float x, float y , float w, float h){
    
    box.x = x;
    box.y = y;
    box.w = w;
    box.h = h;
    
}


void ColorRect::Render(int cor){
    
    glBindTexture(GL_TEXTURE_2D, NULL);
    
    switch (cor) {
        case WHITE:
            glColor4f(1, 1, 1, 0.8);
            break;
        case RED:
            glColor3f(0.8, 0.2, 0.2);
            break;
            
        case ORANGE:
            glColor3f(1, 0.4, 0.1);
            break;
            
        default:
            break;
    }
    
//	tipo de primitiva com quatro vertices
    glBegin(GL_QUADS);
   
//	ponto superior esquerdo
    glVertex2f(box.x, box.y);
    
//	ponto superior direito
    glVertex2f(box.x + box.w , box.y);
    
//	ponto inferior direito
    glVertex2f(box.x + box.w, box.y + box.h);
    
//	ponto inferior esquerdo
    glVertex2f(box.x , box.y + box.h);
    glColor3f(1, 1, 1);
    glEnd();
  
}