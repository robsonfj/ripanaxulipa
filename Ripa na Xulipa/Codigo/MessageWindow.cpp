

#include "MessageWindow.h"

MessageWindow::MessageWindow(int type, std::string message) {
	background = Sprite::Sprite("arquivos/img/messagewindow/messagebg.png");
	this->type = type;
	if (type == 1) {
		buttonOk = Sprite::Sprite("arquivos/img/messagewindow/buttonok.png");
	}
	else {
		buttonYes = Sprite::Sprite("arquivos/img/messagewindow/buttonyes.png");
		buttonNo = Sprite::Sprite("arquivos/img/messagewindow/buttonno.png");
	}
	// Verifica e lida com o tamanho da mensagem
	if (message.size() > 23) {
		Text auxText;
		std::string auxMessage;
		int i = 0;
		int max;
		int submessages;
		submessages = (message.size() / 23) + 1;
		while (i < submessages) {
			max = 22;
			// Verifica onde tem um espaco em branco mais proximo
			if (message.size() >= 23) {
				while (message.c_str()[max] != ' ') {
					max--;
				}
			}
			auxMessage = message.substr(0, max);
			auxText = *new Text(FONTE, 50, Text::TEXT_BLENDED, auxMessage, padraoCor);
			messages.emplace_back(auxText);
			if (message.size() > max) {
				message = message.substr(max + 1, message.size());
			}
			i++;
		}
	}
	else {
		this->message = *new Text(FONTE, 50, Text::TEXT_BLENDED, message, padraoCor);
	}
}

MessageWindow::~MessageWindow() {
	previousState->resultados.clear();
	previousState->resultados.emplace_back(result);
	previousState->resultados.emplace_back(auxResult);
}

void MessageWindow::Render() {
	background.Render(512 - background.GetWidth() / 2, 300 - background.GetHeight() / 2);
	if (type == 1) {
		boxOk.x = 512 - buttonOk.GetWidth() / 2;
		boxOk.y = 300 + background.GetHeight() / 2 - buttonOk.GetHeight() - 20;
		boxOk.w = buttonOk.GetWidth();
		boxOk.h = buttonOk.GetHeight();
		buttonOk.Render(boxOk.x, boxOk.y);
	}
	else {
		boxYes.x = 512 - background.GetWidth() / 2 + 20;
		boxYes.y = 300 + background.GetHeight() / 2 - buttonYes.GetHeight() - 20;
		boxYes.w = buttonYes.GetWidth();
		boxYes.h = buttonYes.GetHeight();
		buttonYes.Render(boxYes.x, boxYes.y);
		boxNo.x = 512 - background.GetWidth() / 2 + background.GetWidth() - buttonNo.GetWidth() - 20;
		boxNo.y = 300 + background.GetHeight() / 2 - buttonNo.GetHeight() - 20;
		boxNo.w = buttonNo.GetWidth();
		boxNo.h = buttonNo.GetHeight();
		buttonNo.Render(boxNo.x, boxNo.y);
	}

	if (messages.size() > 0) {
		int y = 300 - background.GetHeight() / 2 + 50;
		for (int i = 0; i < messages.size(); i++) {
			messages[i].SetPos(512 - background.GetWidth() / 2 + 15, y);
			messages[i].Render();
			y = y + 50;
		}
	}
	else {
		message.SetPos(512 - background.GetWidth() / 2 + 15, 300 - background.GetHeight() / 2 + 50);
		message.Render();
	}
}

void MessageWindow::Update(float dt) {
	// Recebe o resultado enviado pelo ultimo state (StoreState)
	if (cont == 0) {
		if (previousState->resultados.size() > 0 && strcmp(previousState->resultados.front().descricao, "item") == 0 && previousState->resultados.front().tipo == TIPO_INT) {
			auxResult = previousState->resultados.front();
			printf("\n");
		}
		cont++;
	}

	if (type == 1) {
		if (InputManager::GetInstance().IsMouseInside(boxOk)) {
			buttonOk.Open("arquivos/img/messagewindow/buttonokselected.png");
			if (InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON)){
				strcpy(result.descricao, "ok");
				result.tipo = TIPO_BOOL;
				result.boolValue = false;
				requestDelete = true;
			}
		}
		else {
			buttonOk.Open("arquivos/img/messagewindow/buttonok.png");
		}
	}
	else {
		if (InputManager::GetInstance().IsMouseInside(boxYes)) {
			buttonYes.Open("arquivos/img/messagewindow/buttonyesselected.png");
			if (InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON)){
				strcpy(result.descricao, "escolha");
				result.tipo = TIPO_BOOL;
				result.boolValue = true;
				requestDelete = true;
			}
		}
		else {
			buttonYes.Open("arquivos/img/messagewindow/buttonyes.png");
		}
		if (InputManager::GetInstance().IsMouseInside(boxNo)) {
			buttonNo.Open("arquivos/img/messagewindow/buttonnoselected.png");
			if (InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON)){
				strcpy(result.descricao, "escolha");
				result.tipo = TIPO_BOOL;
				result.boolValue = false;
				requestDelete = true;
			}
		}
		else {
			buttonNo.Open("arquivos/img/messagewindow/buttonno.png");
		}
	}
}

void MessageWindow::SetMessage(std::string message) {
	this->message = *new Text(FONTE, 50, Text::TEXT_BLENDED, message, padraoCor, 80, 50);
}
