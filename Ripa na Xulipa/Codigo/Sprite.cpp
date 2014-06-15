

#include "Sprite.h"
#include "Game.h"

std::unordered_map<std::string, GLuint*> Sprite::assetTable;

Sprite::Sprite(){
	currentFrame = 1;
	timeElapsed = 0;
	texturegl = NULL;
}

Sprite::Sprite(string file, int frameCount, float frameTime) : Sprite(){
	this->frameCount = frameCount;
	this->frameTime = frameTime;
	Open(file);
}

void Sprite::Update(float dt){
//	transforma dt para milisegundos
	timeElapsed += dt*1000;
	if (timeElapsed > frameTime) {
		timeElapsed = 0;
		currentFrame += 1;
		if (currentFrame > frameCount)
			currentFrame = 1;
		
		SetClip((currentFrame-1)/frameCount, 1, dimentions.w/frameCount, dimentions.h);
	}
	
}

void Sprite::Open(string file) {
	SDL_Surface *surface = IMG_Load(file.c_str());

	if (surface) {
//	    procura uma imagem ja carregada
		if(assetTable.find(file) != assetTable.end())
			texturegl = assetTable.find(file)->second;
		
		else{
			
			texturegl = new GLuint;
			
			glGenTextures(1, texturegl);
			glBindTexture(GL_TEXTURE_2D, *texturegl);
			
			int modo = GL_RGB;
//          verifica se a imagem tem transparencia ou nao
			if (surface->format->BytesPerPixel == 4)
				modo = GL_RGBA;
			
#ifdef __APPLE__ //se for um computador apple as cores ficam diferentes
			modo = GL_RGB;
//          verifica se a imagem tem transparencia ou nao
			if (surface->format->BytesPerPixel == 4)
				modo = GL_BGRA;
#endif
			
//          parametros de filtragem da textura
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, -1);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
			
//			constroi a textura
			glTexImage2D(GL_TEXTURE_2D, 0, surface->format->BytesPerPixel, surface->w, surface->h, 0, modo, GL_UNSIGNED_BYTE, surface->pixels);
			
//			se texture ficar com NULL imprime mensagem de erro
			if (!IsOpen()){
				cout<<glGetError()<<std::endl;
				exit(1);
			}
			
			assetTable.emplace(file, texturegl);
		}
		
		dimentions.w = surface->w;
		dimentions.h = surface->h;
		
		SetClip(0, 0, dimentions.w, dimentions.h);
		
	}
	else{//se surface ficar com NULL imprime mensagem de erro
		cout<<"Não foi possivel encontrar imagem em: "<< file<<std::endl;
		SDL_Delay(5000);
		exit(1);
	}
	SDL_FreeSurface(surface);
}

void Sprite::Clear(){

	while (assetTable.size() > 0){
		glDeleteTextures(1, assetTable.begin()->second);
		assetTable.erase(assetTable.begin());
	}
}

void Sprite::SetClip(float x, float y, float w, float h) {
	clipRect.x = x;
	clipRect.y = y;
	clipRect.w = w;
	clipRect.h = h;
}

void Sprite::Render(float x, float y, float angle){

	glBindTexture(GL_TEXTURE_2D, GetTexture());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    
    glColor4f(1, 1, 1, alpha);
//	tipo de primitiva com quatro vertices
	glBegin(GL_QUADS);

//	ponto superior esquerdo
	glTexCoord2f(clipRect.x, 0);
	glVertex2f(x, y);

//	ponto superior direito
	glTexCoord2f(currentFrame/frameCount, 0);
	glVertex2f(x + (clipRect.w * scaleX), y);

//	ponto inferior direito
	glTexCoord2f(currentFrame/frameCount, 1);
	glVertex2f(x + (clipRect.w * scaleX), y + (dimentions.h * scaleY));

//	ponto inferior esquerdo
	glTexCoord2f(clipRect.x, 1);
	glVertex2f(x, y +(dimentions.h * scaleY));

	glEnd();
    glColor4f(1, 1, 1, 1);
}

bool Sprite::IsOpen(){
	if (texturegl)
		return true;
	
	return false;
}