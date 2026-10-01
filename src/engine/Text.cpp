// Text.cpp - texto via SDL_ttf com cache de fontes, renderizado como textura.

#include "Text.h"
#include "TextureUpload.h"

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

//	Tipo de primitiva com quatro vertices (texU/texV = 1 em driver
//	normal; <1 quando houve padding POT — ver TextureUpload.h).
	glBegin(GL_QUADS);

//  Ponto superior esquerdo
	glTexCoord2f(0, 0);
	glVertex2f(box.x, box.y);

//  Ponto superior direito
	glTexCoord2f(texU, 0);
	glVertex2f(box.x + box.w, box.y);

//  Ponto inferior direito
	glTexCoord2f(texU, texV);
	glVertex2f(box.x + box.w, box.y + box.h);
	
//  Ponto inferior esquerdo
	glTexCoord2f(0, texV);
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
//  Parametros de filtragem da textura (GL 1.x em todo lugar).
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // TTF_Render* retorna formatos variados (8-bit paletizado no SOLID,
    // 32-bit ARGB no BLENDED). Converte para ABGR8888 (memoria R,G,B,A)
    // e sobe como GL_RGBA.
    int modo = GL_RGBA;
    SDL_Surface* conv = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_ABGR8888, 0);
    if (conv) {
        SDL_FreeSurface(surface);
        surface = conv;
    }

//  Upload via helper central (POT para drivers 1.1 + reempacota pitch).
	int upW = 0, upH = 0;
	float uScale = 1.0f, vScale = 1.0f;
	unsigned char* pixels = RipaPreparePixels(surface, upW, upH, uScale, vScale);
	bool owned = (pixels != nullptr);
	if (!owned) {
		pixels = (unsigned char*)surface->pixels;
	}
	texU = uScale;
	texV = vScale;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, RipaInternalFormat(surface), upW, upH, 0, modo, GL_UNSIGNED_BYTE, pixels);
	if (owned) {
		delete[] pixels;
	}
    
	box.w = surface->w;
	box.h = surface->h;
    
    if (surface) {
        SDL_FreeSurface(surface);
    }
	
    glFinish();

}

