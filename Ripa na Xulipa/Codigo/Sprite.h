

#ifndef __IDJ__Sprite__
#define __IDJ__Sprite__

#include <unordered_map>
#include <SDL.h>
#include <SDL_opengl.h>

#include "Rect.h"

using std::string;

class Sprite{
	float frameCount;
	float currentFrame;
	float timeElapsed;
	float frameTime;
	float scaleX = 1;
	float scaleY= 1;
    float alpha = 1;
	GLuint *texturegl;
	Rect dimentions, clipRect;
	
	static std::unordered_map<std::string, GLuint*> assetTable;
	
public:
	Sprite();
	Sprite (string file, int frameCount = 1, float frameTime = 1);
	~Sprite(){};
	static void Clear();
	
	void  Update (float dt);
	void Open(string file);
	void Render (float x = 0, float y = 0, float angle = 0);
	
	int GetWidth (){return dimentions.w*scaleX/frameCount;};
	int GetHeight (){return dimentions.h*scaleY/frameCount;};
	GLuint GetTexture(){return *texturegl;};
	bool IsOpen();
	
	void SetClip(float x, float y, float w, float h);
	void SetFrame (int frame);
	void SetFrameCount (int frameCount){this->frameCount = frameCount;};
	void SetFrameTime (float frameTime){this->frameTime = frameTime;};
	void SetScaleX (float scale){scaleX = scale;};
	void SetScaleY (float scale){scaleY = scale;};
    void SetAlpha(float alpha){this->alpha = alpha;};
};

#endif /* defined(__IDJ__Sprite__) */
