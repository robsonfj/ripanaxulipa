// Text.cpp - texto via SDL_ttf com cache de fontes, renderizado como textura.

#include "Text.h"

#include <cstring> // memcpy (reempacotamento de pitch)
#include "Sync.h"

#ifdef _WIN32
#ifndef GL_BGR
#define GL_BGR 0x80E0
#endif
#ifndef GL_BGRA
#define GL_BGRA 0x80E1
#endif
#ifndef GL_GENERATE_MIPMAP
#define GL_GENERATE_MIPMAP 0x8191
#endif
#endif

std::unordered_map<std::string, TTF_Font*> Text::assetTable;


Text::Text(string fontFile, int fontSize, TextStyle style,string text, SDL_Color color, int x, int y) {

	this->fontFile = fontFile;
	this->fontSize = fontSize;
	this->style = style;
	this->text = text;
	this->color = color;
	texturegl = new GLuint;
	glGenTextures(1, texturegl);
    RemakeTexture();
    
	box.x = x - box.w / 2;
	box.y = y - box.h / 2;

}


Text::~Text() {

	if (texturegl) {
		glDeleteTextures(1, texturegl);
	}

}


void Text::Render(int cameraX, int cameraY) {

    glFlush();
	glBindTexture(GL_TEXTURE_2D, *texturegl);
	
	glPushMatrix();

//	Tipo de primitiva com quatro vertices
	glBegin(GL_QUADS);

//  Ponto superior esquerdo
	glTexCoord2f(0, 0);
	glVertex2f(box.x, box.y);

//  Ponto superior direito
	glTexCoord2f(1, 0);
	glVertex2f(box.x + box.w, box.y);

//  Ponto inferior direito
	glTexCoord2f(1, 1);
	glVertex2f(box.x + box.w, box.y + box.h);
	
//  Ponto inferior esquerdo
	glTexCoord2f(0, 1);
	glVertex2f(box.x, box.y + box.h);
	glEnd();
	glPopMatrix();
    glFinish();
	
}

void Text::SetPos(int x, int y, bool centerX, bool centerY) {
	
	if (centerX) {
		box.x = x - box.w / 2;
	}
	else {
		box.x = x;
	}
	if (centerY) {
		box.y = y - box.h / 2;
	}
	else {
		box.y = y;
	}	
	RemakeTexture();

}


void Text::SetText(string text) {
    
    this->text = text;
    if (tempText != text) {
        RemakeTexture();
        tempText = text;
    }
	

}


void Text::SetColor(SDL_Color color) {

	this->color = color;
	RemakeTexture();

}





void Text::RemakeTexture() {

    std::lock_guard<std::recursive_mutex> lock(g_gfxMutex);
    glFlush();
    
	std::stringstream path_size;

//	Cria uma variavel com o caminho e tamanho da letra
	path_size << fontFile << fontSize;
    
	if (assetTable.find(path_size.str()) != assetTable.end()) {
		font = assetTable.find(path_size.str())->second;
	}	
    else {  
        font = TTF_OpenFont(fontFile.c_str(), fontSize);
//		Se texture ficar com NULL, imprime mensagem de erro
		if (!font) {
			cout << "Não foi possivel encontrar imagem em: " << fontFile << std::endl;
			exit(1);
		}
		assetTable.emplace(path_size.str(), font);
	}
	
	switch (style) {
		case TEXT_SOLID:
			surface = TTF_RenderUTF8_Solid(font, text.c_str(), color);
			break;
		case TEXT_SHADED:
			surface = TTF_RenderUTF8_Shaded(font, text.c_str(), color, color);
			break;
		case TEXT_BLENDED:
			surface = TTF_RenderUTF8_Blended(font, text.c_str(), color);
			break;
	}
	if (surface == NULL) {
		// Texto vazio: evita crash em surface->format
		return;
	}
	
    
    glBindTexture(GL_TEXTURE_2D, *texturegl);
//  Parametros de filtragem da textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#ifndef _WIN32
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
#endif
    
    // TTF_Render* retorna formatos variados (8-bit paletizado no SOLID,
    // 32-bit ARGB no BLENDED). Converte para ABGR8888 (memoria R,G,B,A)
    // e sobe como GL_RGBA — corrige "xiado" no Windows.
    int modo = GL_RGBA;
    int internalFormat = GL_RGBA;
    SDL_Surface* conv = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_ABGR8888, 0);
    if (conv) {
        SDL_FreeSurface(surface);
        surface = conv;
    }
   
//  Constroi a textura (reempacota se houver padding no pitch)
 	int bpp = surface->format->BytesPerPixel;
 	unsigned char* tightBuf = nullptr;
 	unsigned char* pixels = (unsigned char*)surface->pixels;
 	if (surface->pitch != surface->w * bpp) {
 		tightBuf = new unsigned char[(size_t)surface->w * surface->h * bpp];
 		for (int y = 0; y < surface->h; y++) {
 			memcpy(tightBuf + (size_t)y * surface->w * bpp,
 				   (unsigned char*)surface->pixels + (size_t)y * surface->pitch,
 				   (size_t)surface->w * bpp);
 		}
 		pixels = tightBuf;
 	}
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, surface->w, surface->h, 0, modo, GL_UNSIGNED_BYTE, pixels);
 	delete[] tightBuf;
    
	box.w = surface->w;
	box.h = surface->h;
    
    if (surface) {
        SDL_FreeSurface(surface);
    }
	
    glFinish();

}

