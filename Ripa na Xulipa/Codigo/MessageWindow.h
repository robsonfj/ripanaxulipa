

#ifndef __IDJ__MessageWindow__
#define __IDJ__MessageWindow__

#include "Sprite.h"
#include "Text.h"

class MessageWindow : public State {
	Sprite background;
	Sprite buttonYes;
	Sprite buttonNo;
	Sprite buttonOk;
	Text message;
	std::vector<Text> messages;
	int type;
	int cont = 0;
	Rect boxOk;
	Rect boxYes;
	Rect boxNo;
	Resultado auxResult;
public:
    MessageWindow(){};
	MessageWindow(int type, std::string message);
	~MessageWindow();
	void SetMessage(std::string message);
	void Render();
	void Update(float dt);
	Resultado result;
};

#endif /* defined(__IDJ__MessageWindow__) */
