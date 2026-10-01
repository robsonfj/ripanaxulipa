// Text.h - texto via SDL_ttf com cache de fontes, renderizado como textura.

#ifndef __IDJ__Text__
#define __IDJ__Text__

#include "Game.h"

class Text {

public:	
	enum TextStyle{
		TEXT_SOLID,
		TEXT_SHADED,
		TEXT_BLENDED
	};
	Rect box;
	bool selected = false;

	Text() {};
	Text(string fontFile, int fontSize, TextStyle style,string text, SDL_Color color, int x = 0, int y = 0);
	~Text();
	
	void Render(int cameraX = 0, int cameraY = 0);
	void SetPos(int x, int y, bool centerX = false, bool centerY = false);
	void SetText(string text);
	void SetColor(SDL_Color color);
	void RemakeTexture();

private:
    string tempText = "";
	TTF_Font* font = nullptr;
	GLuint* texturegl = NULL;
	SDL_Surface *surface = NULL;
	string fontFile, text;
	TextStyle style = TEXT_BLENDED;
	int fontSize = 0;
	SDL_Color color = {0, 0, 0, 0};
//	Fracao util da textura (POT em drivers 1.1: ver TextureUpload.h).
	float texU = 1.0f, texV = 1.0f;
	static std::unordered_map<std::string, TTF_Font*> assetTable;

};

#endif /* defined(__IDJ__Text__) */
