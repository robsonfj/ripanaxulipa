// TextureUpload.h - upload de texturas compativel ate com OpenGL 1.1.
//
// Problema: drivers basicos (ex. Microsoft GDI Generic em VMs de CI)
// implementam OpenGL 1.1 puro: rejeitam (GL_INVALID_VALUE) texturas
// NPOT (nao potencia de 2) e enums de versoes novas. O jogo usa muitas
// texturas NPOT (900x169, 510x600, textos de largura livre...).
//
// Solucao centralizada aqui:
//  - detecta em runtime (glGetString) se o driver exige POT;
//  - se sim, reempacota os pixels num buffer POT (linhas justas,
//    sem trocar canais) e informa a area util via uScale/vScale para
//    os Render()s ajustarem as coordenadas (1.0 vira uScale/vScale);
//  - internalFormat numerico 3/4 (o simbolico GL_RGBA nem sempre
//    existe no 1.1) e wrap GL_CLAMP (TO_EDGE e 1.2+).
// Em driver moderno uScale=vScale=1 e o comportamento e identico ao
// upload direto (bit a bit nas coordenadas).
#pragma once

#include <SDL.h>
#include <SDL_opengl.h>

#include <cstdlib>
#include <cstddef>
#include <cstring>

// Menor potencia de 2 >= v.
inline int RipaNextPOT(int v) {
    int p = 1;
    while (p < v) {
        p *= 2;
    }
    return p;
}

// Diz se o driver atual exige texturas power-of-two (cacheado).
// Default seguro: em caso de duvida (sem contexto), assume que sim.
inline bool RipaNeedsPOT() {
    static int cached = -1;
    if (cached >= 0) {
        return cached != 0;
    }
    cached = 1;
    const char* ver = (const char*)glGetString(GL_VERSION);
    int major = ver ? std::atoi(ver) : 0;
    if (major >= 2) {
        cached = 0;  // 2.0+ tem NPOT nativo
    }
    else {
        const char* ext = (const char*)glGetString(GL_EXTENSIONS);
        if (ext && std::strstr(ext, "GL_ARB_texture_non_power_of_two")) {
            cached = 0;
        }
    }
    return cached != 0;
}

// Prepara pixels para glTexImage2D a partir de um SDL_Surface.
// Retorna nullptr quando pode subir direto (surface->pixels); senao um
// buffer novo (caller da delete[]) com linhas justas + padding POT.
// outW/outH = dimensoes reais do upload; uScale/vScale = fracao util.
inline unsigned char* RipaPreparePixels(SDL_Surface* s, int& outW, int& outH,
                                        float& uScale, float& vScale) {
    const int bpp = s->format->BytesPerPixel;
    const int w = s->w;
    const int h = s->h;
    int pw = w;
    int ph = h;
    if (RipaNeedsPOT()) {
        pw = RipaNextPOT(w);
        ph = RipaNextPOT(h);
    }
    uScale = (float)w / (float)pw;
    vScale = (float)h / (float)ph;
    outW = pw;
    outH = ph;
    if (pw == w && ph == h && s->pitch == w * bpp) {
        return nullptr;  // ja justo: sobe direto, sem copia
    }
    unsigned char* buf = new unsigned char[(size_t)pw * ph * bpp]();
    for (int y = 0; y < h; y++) {
        std::memcpy(buf + (size_t)y * pw * bpp,
                    (unsigned char*)s->pixels + (size_t)y * s->pitch,
                    (size_t)w * bpp);
    }
    return buf;
}

// internalFormat legado 1/2/3/4 a partir do BytesPerPixel (vale no 1.1,
// onde os simbolicos GL_RGB/GL_RGBA como internalFormat nem sempre
// existem; nos drivers novos equivale ao mesmo).
inline int RipaInternalFormat(SDL_Surface* s) {
    switch (s->format->BytesPerPixel) {
        case 1: return 1;
        case 2: return 2;
        case 4: return 4;
        default: return 3;
    }
}
