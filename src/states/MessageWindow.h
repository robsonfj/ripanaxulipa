// MessageWindow.h - modal OK / Sim-Nao com retorno via resultados do estado.

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

    MessageWindow(int type, std::string message);
	~MessageWindow();
//	Quebra a mensagem em linhas de ate 23 chars (corte no espaco).
//	Pura (sem SDL/GL): coberta por testes. Comportamento identico ao
//	loop original do construtor, incluindo casos-limite.
	static std::vector<std::string> SplitLines(std::string message);
	void Render();
	void Update(float dt);

};

#endif /* defined(__IDJ__MessageWindow__) */
