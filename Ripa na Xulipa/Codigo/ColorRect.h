//
//  ColorRect.h
//  Avenida Paulista
//
//  Created by Robson Ferreira Jacomini on 12/06/14.
//
//
#ifndef __Avenida_Paulista__ColorRect__
#define __Avenida_Paulista__ColorRect__

#include <SDL.h>
#include <SDL_opengl.h>
#include "Rect.h"

#define WHITE 0
#define RED 1
#define ORANGE 2

class ColorRect{
    Rect box;
    
public:
    ColorRect(){};
    ColorRect(float x, float y , float w, float h);
    
    void Render(int cor = 0);
    
};

#endif