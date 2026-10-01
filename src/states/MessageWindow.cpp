// MessageWindow.cpp - modal OK / Sim-Nao com retorno via resultados do estado.

#include "MessageWindow.h"


MessageWindow::MessageWindow(int type, std::string message) {

	// Carrega backgrounds
	bg = Sprite("arquivos/img/states/Pattern.jpg");
	bg2 = Sprite("arquivos/img/states/messagewindow/messagebg.png");

	// Inicializa as sprites e os textos
	this->type = type;
	if (type == 1) {
		buttonOk = Sprite("arquivos/img/states/messagewindow/buttonbg.png");
		txok = *new Text(FONTE, 50, Text::TEXT_BLENDED, "OK", padraoCor, 508, 410);
	}
	else {
		buttonYes = Sprite("arquivos/img/states/messagewindow/buttonbg.png");
		txyes = *new Text(FONTE, 50, Text::TEXT_BLENDED, "YES", padraoCor, 360, 410);
		buttonNo = Sprite("arquivos/img/states/messagewindow/buttonbg.png");
		txno = *new Text(FONTE, 50, Text::TEXT_BLENDED, "NO", padraoCor, 658, 410);
	}

	// Verifica e lida com o tamanho da mensagem
	if (message.size() > 23) {
		for (const std::string& auxMessage : SplitLines(message)) {
			Text auxText;
			auxText = *new Text(FONTE, 50, Text::TEXT_BLENDED, auxMessage, padraoCor);
			messages.emplace_back(auxText);
		}
	}
	else {
		this->message = *new Text(FONTE, 50, Text::TEXT_BLENDED, message, padraoCor);
	}

}


std::vector<std::string> MessageWindow::SplitLines(std::string message) {

	std::vector<std::string> lines;
	if (message.size() > 23) {
		int i = 0;
		int max;
		int submessages;
		submessages = (message.size() / 23) + 1;
		while (i < submessages) {
			max = 22;
			// Verifica onde tem um espaco em branco mais proximo
			if (message.size() >= 23) {
				while (max > 0 && (size_t)max < message.size() && message.c_str()[max] != ' ') {
					max--;
				}
			}
			lines.emplace_back(message.substr(0, max));
			if (message.size() > (size_t)max) {
				message = message.substr(max + 1, message.size());
			}
			i++;
		}
	}
	else {
		lines.emplace_back(message);
	}
	return lines;

}


MessageWindow::~MessageWindow() {

	previousState->resultados.clear();
	previousState->resultados.emplace_back(result);
	previousState->resultados.emplace_back(auxResult);

}


void MessageWindow::Render() {

	bg.Render();
	bg2.Render(512 - bg2.GetWidth() / 2, 300 - bg2.GetHeight() / 2);

	if (type == 1) {
		buttonOk.Render(512 - buttonOk.GetWidth() / 2, 280 + bg2.GetHeight() / 2 - buttonOk.GetHeight() - 20);
		txok.Render();
	}
	else {
		buttonYes.Render(512 - bg2.GetWidth() / 2 + 40, 280 + bg2.GetHeight() / 2 - buttonYes.GetHeight() - 20);
		buttonNo.Render(512 - bg2.GetWidth() / 2 + bg2.GetWidth() - buttonNo.GetWidth() - 40, 280 + bg2.GetHeight() / 2 - buttonNo.GetHeight() - 20);
		txyes.Render();
		txno.Render();
	}

	if (messages.size() > 0) {
		int y = 300 - bg2.GetHeight() / 2 + 50;
		for (int i = 0; i < messages.size(); i++) {
			messages[i].SetPos(512 - bg2.GetWidth() / 2 + 25, y);
			messages[i].Render();
			y = y + 50;
		}
	}
	else {
		message.SetPos(512 - bg2.GetWidth() / 2 + 25, 300 - bg2.GetHeight() / 2 + 50);
		message.Render();
	}

}


void MessageWindow::Update(float dt) {

	//  Se a tecla ESC for pressionada, setar a flag para deletar esse estado
	if ((InputManager::GetInstance().KeyPress(ESCAPE_KEY))) {
		requestDelete = true;
	}

	//	Se a condicao de saida for atendida
	if (InputManager::GetInstance().ShouldQuit()) {
		requestQuit = true;
	}

//  Recebe o resultado enviado pelo ultimo state (StoreState)
	if (cont == 0) {
		if (previousState->resultados.size() > 0 && strcmp(previousState->resultados.front().descricao, "item") == 0 && previousState->resultados.front().tipo == TIPO_INT) {
			auxResult = previousState->resultados.front();
			printf("\n");
		}
		cont++;
	}

	if (type == 1) {
		if (InputManager::GetInstance().IsMouseInside(txok.box)) {
			txok.SetColor(padraoCorSelect);
			if (InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON)) {
				snprintf(result.descricao, sizeof(result.descricao), "ok");
				result.tipo = TIPO_BOOL;
				result.boolValue = false;
				requestDelete = true;
			}
		}
		else {
			txok.SetColor(padraoCor);
		}
	}
	else {
		if (InputManager::GetInstance().IsMouseInside(txyes.box)) {
			txyes.SetColor(padraoCorSelect);
			if (InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON)) {
				snprintf(result.descricao, sizeof(result.descricao), "escolha");
				result.tipo = TIPO_BOOL;
				result.boolValue = true;
				requestDelete = true;
			}
		}
		else {
			txyes.SetColor(padraoCor);
		}
		if (InputManager::GetInstance().IsMouseInside(txno.box)) {
			txno.SetColor(padraoCorSelect);
			if (InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON)) {
				snprintf(result.descricao, sizeof(result.descricao), "escolha");
				result.tipo = TIPO_BOOL;
				result.boolValue = false;
				requestDelete = true;
			}
		}
		else {
			txno.SetColor(padraoCor);
		}
	}

}