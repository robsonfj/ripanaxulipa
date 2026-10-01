// Sprite.h - textura OpenGL com cache global e animacao por frames.

#ifndef __IDJ__Sprite__
#define __IDJ__Sprite__

#include <unordered_map>
#include <string>
#include <SDL.h>
#include <SDL_opengl.h>

#include "Rect.h"
#include "Timer.h"

using std::string;

class Sprite {

private:
	float frameCount;
	float currentFrame;
	Timer timeElapsed;
	float frameTime;
	float scaleX = 1;
	float scaleY= 1;
    float alpha = 1;
	GLuint *texturegl;
    SDL_sem *semaphore;
	Rect dimensions, clipRect;
	static std::unordered_map<std::string, GLuint*> assetTable;
	
public:
	Sprite();
	Sprite(string file, int frameCount = 1, float frameTime = 1);
	~Sprite() {};
	
	void  Update(float dt);
	void Open(string file);
	void Render(float x = 0, float y = 0);
	
	int GetWidth() { return dimensions.w * scaleX / frameCount; };
	int GetHeight() { return dimensions.h * scaleY / frameCount; };
	GLuint *GetTexture() { return texturegl; };
	
	void SetClip(float x, float y, float w, float h);
	void SetFrame(int frame) { currentFrame = frame; SetClip((currentFrame - 1) / frameCount, 1, dimensions.w / frameCount, dimensions.h); };
	void SetScaleX (float scale) {scaleX = scale;};
	void SetScaleY (float scale) {scaleY = scale;};
    void SetAlpha(float alpha) {this->alpha = alpha;};

	bool IsOpen();
    bool IsTransparent();

};

#endif /* defined(__IDJ__Sprite__) */
