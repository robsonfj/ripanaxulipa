// Sprite.cpp - textura OpenGL com cache global e animacao por frames.

#include "Sprite.h"
#include "Game.h"
#include "Sync.h"

#include <cstring> // memcpy (reempacotamento de pitch)

// Compatibilidade OpenGL no Windows (gl.h do MSVC e OpenGL 1.1, sem esses enums)
#ifdef _WIN32
#ifndef GL_BGR
#define GL_BGR 0x80E0
#endif
#ifndef GL_BGRA
#define GL_BGRA 0x80E1
#endif
#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif
#ifndef GL_GENERATE_MIPMAP
#define GL_GENERATE_MIPMAP 0x8191
#endif
#ifndef GL_TEXTURE_BASE_LEVEL
#define GL_TEXTURE_BASE_LEVEL 0x813C
#endif
#ifndef GL_TEXTURE_MAX_LEVEL
#define GL_TEXTURE_MAX_LEVEL 0x813D
#endif
#ifndef GL_CLAMP
#define GL_CLAMP 0x2900
#endif
#endif

std::unordered_map<std::string, GLuint*> Sprite::assetTable;


Sprite::Sprite() {
    semaphore = SDL_CreateSemaphore(1);
	currentFrame = 1;

}


Sprite::Sprite(string file, int frameCount, float frameTime) : Sprite() {

	this->frameCount = frameCount;
	this->frameTime = frameTime;
	Open(file);

}


void Sprite::Update(float dt) {

//	Converte dt para milissegundos
	timeElapsed.Update(dt * 1000);

	if (timeElapsed.Get() > frameTime) {
        timeElapsed.Restart();
		currentFrame += 1;
		if (currentFrame > frameCount) {
			currentFrame = 1;
		}
		SetClip((currentFrame - 1) / frameCount, 1, dimensions.w / frameCount, dimensions.h);
	}
	
}


void Sprite::Open(string file) {

    std::lock_guard<std::recursive_mutex> lock(g_gfxMutex);
    glFlush();
    SDL_SemWait(semaphore);
    SDL_Surface *surface = IMG_Load(file.c_str());

	if (surface) {
//	    Procura uma imagem ja carregada
		if (assetTable.find(file) != assetTable.end()) {
			texturegl = assetTable.find(file)->second;
		}		
		else {           
            texturegl = new GLuint;
            glGenTextures(1, texturegl);
            glBindTexture(GL_TEXTURE_2D, *texturegl);

            // NOTA (Windows/vcpkg SDL_image 2.8, verificado empiricamente):
            // - PNG  -> ABGR8888, bytes R,G,B,A, mascaras corretas.
            // - JPG  -> rotulado ARGB8888 mas bytes em ordem R,G,B,A
            //          (mascaras R/B trocadas no loader). Nao confiar nas
            //          mascaras: converte-las via SDL_ConvertSurfaceFormat
            //          QUEBRA o JPG. Sobe os bytes direto como RGB/RGBA.
            int modo = GL_RGB;
            int internalFormat = GL_RGB;
            if (surface->format->BytesPerPixel == 4) {
                modo = GL_RGBA;
                internalFormat = GL_RGBA;
            } else if (surface->format->BytesPerPixel == 3) {
                modo = GL_RGB;
                internalFormat = GL_RGB;
            } else {
                // Paletizado (8-bit) etc: ai sim converte, via paleta (seguro).
                SDL_Surface* conv = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_ABGR8888, 0);
                if (conv) {
                    SDL_FreeSurface(surface);
                    surface = conv;
                    modo = GL_RGBA;
                    internalFormat = GL_RGBA;
                }
            }
            
//          Parametros de filtragem da textura
#ifdef _WIN32
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#else
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
#endif
			
//			Constroi a textura.
//			Reempacota linhas justas se houver padding no pitch
//			(ex: 2.jpg 510x3: pitch 1532). Sem isso a imagem sai
//			"esticada/entrelaçada". Nao troca canais, so remove padding.
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

//			Textura incompleta renderiza em branco: registra para diagnostico.
			GLenum texErr = glGetError();
			if (texErr != GL_NO_ERROR) {
				cout << "Falha ao subir textura (" << texErr << "): " << file << std::endl;
			}
			
//			Se texture ficar NULL, imprime mensagem de erro
			if (!IsOpen()) {
				cout << glGetError() << std::endl;
				exit(1);
			}
			assetTable.emplace(file, texturegl);
		} // else
		
		dimensions.w = surface->w;
		dimensions.h = surface->h;
		
		SetClip(0, 1, dimensions.w/frameCount, dimensions.h);
	}
	else { // Se surface ficar NULL, imprime mensagem de erro
		cout << "Nao foi possivel encontrar imagem em: " << file << " (" << IMG_GetError() << ")" << std::endl;
		SDL_Delay(5000);
		exit(1);
	}
    
    if (surface) {
        SDL_FreeSurface(surface);
    }
    glFinish();
    SDL_SemPost(semaphore);
}



void Sprite::SetClip(float x, float y, float w, float h) {

	clipRect.x = x;
	clipRect.y = y;
	clipRect.w = w;
	clipRect.h = h;

}


void Sprite::Render(float x, float y) {
    
    glFlush();
	glBindTexture(GL_TEXTURE_2D, *GetTexture());
#ifdef _WIN32
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#else
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
#endif
    
    glColor4f(1, 1, 1, alpha);
//	Tipo de primitiva com quatro vertices
	glPushMatrix();
	glBegin(GL_QUADS);

//	Ponto superior esquerdo
	glTexCoord2f(clipRect.x, 0);
	glVertex2f(x, y);

//	Ponto superior direito
	glTexCoord2f(currentFrame / frameCount, 0);
	glVertex2f(x + (clipRect.w * scaleX), y);

//	Ponto inferior direito
	glTexCoord2f(currentFrame/frameCount, 1);
	glVertex2f(x + (clipRect.w * scaleX), y + (dimensions.h * scaleY));

//	Ponto inferior esquerdo
	glTexCoord2f(clipRect.x, 1);
	glVertex2f(x, y +(dimensions.h * scaleY));

	glEnd();
	glPopMatrix();
    glColor4f(1, 1, 1, 1);
    glFinish();
}


bool Sprite::IsOpen() {

	if (texturegl) {
		return true;
	}
	else {
		return false;
	}
}


bool Sprite::IsTransparent() {
    
	if (alpha < 1) {
		return true;
	}
	else {
		return false;
	}
    
}