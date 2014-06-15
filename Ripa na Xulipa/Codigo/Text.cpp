

#include "Text.h"

std::unordered_map<std::string, TTF_Font*> Text::assetTable;

Text::Text(string fontFile, int fontSize, TextStyle style,string text, SDL_Color color, int x, int y){

	this->fontFile = fontFile;
	this->fontSize = fontSize;
	this->style = style;
	this->text = text;
	this->color = color;
	texturegl = new GLuint;
	glGenTextures(1, texturegl);
	RemakeTexture();
	
	box.x = x - box.w/2;
	box.y = y - box.h/2;
}

Text::~Text(){
	if (texturegl)
		glDeleteTextures(1, texturegl);

}

void Text::Render(int cameraX, int cameraY){
	
	glBindTexture(GL_TEXTURE_2D, *texturegl);
//	tipo de primitiva com quatro vertices
	glBegin(GL_QUADS);
//  ponto superior esquerdo
	glTexCoord2f(0, 0);
	glVertex2f(box.x, box.y);

//  ponto superior direito
	glTexCoord2f(1, 0);
	glVertex2f(box.x + box.w, box.y);

//  ponto inferior direito
	glTexCoord2f(1, 1);
	glVertex2f(box.x + box.w, box.y + box.h);

//  ponto inferior esquerdo
	glTexCoord2f(0, 1);
	glVertex2f(box.x, box.y + box.h);

	glEnd();

	
}

void Text::SetPos(int x, int y, bool centerX, bool centerY) {
	
	if (centerX)
		box.x = x - box.w/2;
	else
		box.x = x;
	
	if (centerY)
		box.y = y - box.h/2;
	else
		box.y = y;
	
	RemakeTexture();
}

void Text::SetText(string text){
	this->text = text;
	RemakeTexture();
}

void Text::SetColor(SDL_Color color){
	this->color = color;
	RemakeTexture();
}

void Text::SetStyle(TextStyle style){
	this->style = style;
	RemakeTexture();
}

void Text::SetFontSize(int fontSize){
	this->fontSize = fontSize;
	RemakeTexture();
}

void Text::Clear(){
	
	while (assetTable.size() > 0){
		TTF_CloseFont(assetTable.begin()->second);
		assetTable.erase(assetTable.begin());
	}
}

void Text::RemakeTexture(){
	std::stringstream path_size;
//	cria uma variavel com o caminho e tamanho da letra
	path_size<<fontFile<<fontSize;
	
	glBindTexture(GL_TEXTURE_2D, *texturegl);
//  parametros de filtragem da textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	
	
	if(assetTable.find(path_size.str()) != assetTable.end())
		font = assetTable.find(path_size.str())->second;
	
	else{
		
		font = TTF_OpenFont(fontFile.c_str(), fontSize);
		
//		se texture ficar com NULL imprime mensagem de erro
		if (!font){
			cout<<"Não foi possivel encontrar imagem em: "<< fontFile<<std::endl;
			exit(1);
		}
		
		assetTable.emplace(path_size.str(), font);
	}
	
	switch (style) {
		case TEXT_SOLID:
			surface = TTF_RenderText_Solid(font, text.c_str(), color);
			break;
		case TEXT_SHADED:
			surface = TTF_RenderText_Shaded(font, text.c_str(), color, color);
			break;
		case TEXT_BLENDED:
			surface = TTF_RenderText_Blended(font, text.c_str(), color);
			break;
	}
	
	int modo = GL_RGB;
//  verifica se a imagem tem transparencia ou nao
	if (surface->format->BytesPerPixel == 4)
		modo = GL_RGBA;
	
#ifdef __APPLE__ //se for um apple as cores ficam diferentes
	modo = GL_BGR;
//  verifica se a imagem tem transparencia ou nao
	if (surface->format->BytesPerPixel == 4)
		modo = GL_BGRA;
#endif
	
//  constroi a textura
	glTexImage2D(GL_TEXTURE_2D, 0, surface->format->BytesPerPixel, surface->w, surface->h, 0, modo, GL_UNSIGNED_BYTE, surface->pixels);
	
	box.w = surface->w;
	box.h = surface->h;
	SDL_FreeSurface(surface);
}

