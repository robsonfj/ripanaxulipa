
#ifndef __IDJ__MessageWindow__
#define __IDJ__MessageWindow__

#include "Sprite.h"
#include "Text.h"

class MessageWindow : public State {

private:
	Sprite bg, bg2, buttonOk, buttonYes, buttonNo;
	Text message, txyes, txno, txok;
	std::vector<Text> messages;
	int type, cont = 0;
	Resultado auxResult;

public:
	Resultado result;

    MessageWindow() {};
	MessageWindow(int type, std::string message);
	~MessageWindow();
	void SetMessage(std::string message);
	void Render();
	void Update(float dt);

};

#endif /* defined(__IDJ__MessageWindow__) */
