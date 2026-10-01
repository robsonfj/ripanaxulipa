// ColorRect.h - retangulo colorido OpenGL para overlays e caixas de dialogo.

#ifndef __Avenida_Paulista__ColorRect__
#define __Avenida_Paulista__ColorRect__

#include <SDL.h>
#include <SDL_opengl.h>
#include "Rect.h"

#define RED 0
#define ORANGE 1
#define BLACK 2
#define WHITE 3

class ColorRect {

private:
    Rect box;
    
public:
    ColorRect() {};
    ColorRect(float x, float y , float w, float h);

    void Render(int cor = 0, float alpha = 1);

};

#endif