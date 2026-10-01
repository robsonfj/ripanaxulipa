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
	TTF_Font* font;
	GLuint* texturegl = NULL;
	SDL_Surface *surface = NULL;
	string fontFile, text;
	TextStyle style;
	int fontSize;
	SDL_Color color;
	static std::unordered_map<std::string, TTF_Font*> assetTable;

};

#endif /* defined(__IDJ__Text__) */
