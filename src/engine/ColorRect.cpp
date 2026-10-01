// ColorRect.cpp - retangulo colorido OpenGL para overlays e caixas de dialogo.

#include "ColorRect.h"


ColorRect::ColorRect(float x, float y , float w, float h) {

    box.x = x - w / 2;
    box.y = y - h / 2;
    box.w = w;
    box.h = h;

}



void ColorRect::Render(int cor, float alpha) {
    
    glBindTexture(GL_TEXTURE_2D, NULL);
    
    switch (cor) {
        case RED:
            glColor4f(0.8, 0.2, 0.2, alpha);
            break;
        case ORANGE:
            glColor4f(1, 0.4, 0.1, alpha);
            break;
        case BLACK:
            glColor4f(0, 0, 0, alpha);
            break;
		case WHITE:
			glColor4f(1, 1, 1, alpha);
            break;
        default:
            break;
    }
    
//	Tipo de primitiva com quatro vertices
    glBegin(GL_QUADS);
   
//	Ponto superior esquerdo
    glVertex2f(box.x, box.y);
    
//	Ponto superior direito
    glVertex2f(box.x + box.w , box.y);
    
//	Ponto inferior direito
    glVertex2f(box.x + box.w, box.y + box.h);
    
//	Ponto inferior esquerdo
    glVertex2f(box.x , box.y + box.h);
    glColor4f(1, 1, 1, 1);
    glEnd();
  
}