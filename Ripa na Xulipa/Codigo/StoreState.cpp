
#include "StoreState.h"
#include "Game.h"

StoreState::StoreState() : bg2("arquivos/img/states/Pattern.jpg"), title("arquivos/img/states/store/storetitle.png"), storeMusic("arquivos/audio/storemusic.mp3") {

	// Inicializa textos 
	txBack = *new Text(FONTE, 50, Text::TEXT_BLENDED, "BACK", padraoCor, 80, 50);
	// Adiciona os itens
	numItens = 3;
	ManageItems();
	// Recupera a quantidade de moedas e escreve na tela
	money = *new Money(Game::GetInstance().GetCoins());
//    inicializa efeito de som
    coinFX = *new Sound("arquivos/audio/Efeitchos Sonoricos/k-ching.ogg");
    storeMusic.Play(-1);

}


StoreState::~StoreState() {

	itens.clear();

}


void StoreState::Input() {
    
//	Se o cursor estiver se movendo, desabilita selecao pelo teclado
	if (InputManager::GetInstance().mouseMoving) {
		nrtxtselected = -1;
	}
    
//	Habilita a selecao das opcoes pelo teclado
    if (InputManager::GetInstance().KeyPress(UP_ARROW_KEY)) {
        nrtxtselected -= 1;
		if (nrtxtselected < 0) {
			nrtxtselected = numItens;
		}
    }
    if (InputManager::GetInstance().KeyPress(DOWN_ARROW_KEY)) {
        nrtxtselected += 1;
		if (nrtxtselected > numItens) {
			nrtxtselected = 0;
		}
    }
    
//  Se a tecla for ESC, setar a flag para deletar esse estado
	if ((InputManager::GetInstance().KeyPress(ESCAPE_KEY))) {
		requestDelete = true;
		storeMusic.Stop();
	}
	
//	Se a condicao de saida for atendida
	if (InputManager::GetInstance().ShouldQuit()) {
		requestQuit = true;
	}
	
//	Verifica se esta selecionado e se foi clicado
	if (InputManager::GetInstance().IsMouseInside(txBack.box) || nrtxtselected == 0) {
        txBack.selected = true;
        txBack.SetColor(padraoCorSelect2);
		if (InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON) || InputManager::GetInstance().KeyPress(ENTER_KEY)) {
			requestDelete = true;
			storeMusic.Stop();
		}
	}
    else {
        if (txBack.selected) {
            txBack.selected = false;
            txBack.SetColor(padraoCor);
        }
    }

//  Verifica se algum item foi clicado
	for (int i = 0; i < numItens; i++) {
		if (InputManager::GetInstance().IsMouseInside(itens[i]->box) || nrtxtselected == i + 1) {
			if (!itens[i]->IsSelected()) {
				itens[i]->Selected();
			}
			if (InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON) || InputManager::GetInstance().KeyPress(ENTER_KEY)) {
				nextState = new MessageWindow(2, "Tem certeza?");
				nextState->previousState = this;
//				Cria o "resultado" para mandar para MessageWindow
				strcpy(result.descricao, "item");
				result.tipo = TIPO_INT;
				result.intValue = i;
				resultados.clear();
				resultados.emplace_back(result);
				Game::GetInstance().Push(nextState);
			}
        }
        else {
            if (itens[i]->IsSelected()) {
                itens[i]->Unselected();
            }
        }
    }

}


void StoreState::Update(float dt) {
    
	if (resultados.size() > 1 && strcmp(resultados.front().descricao, "escolha") == 0 && resultados.front().tipo == TIPO_BOOL && resultados.front().boolValue) {
		if (strcmp(resultados[1].descricao, "item") == 0 && resultados[1].tipo == TIPO_INT) {
			CanBuy(resultados[1].intValue);
			ManageItems();
			printf("\n");
		}
	}
	Input();
    
    if (!storeMusic.IsPlaying()) {
        storeMusic.Play(-1);
    }

}


void StoreState::Render() {

	bg2.Render();
	title.Render(512 - title.GetWidth() / 2, 15);
//  Renderiza o dinheiro
 	money.Render(50, 500);
//  Renderiza os textos
	txBack.Render();
//  Renderiza os itens
	for (int i = 0; i < numItens; i++) {
		itens[i]->Render();
	}

}


void StoreState::CanBuy(int item) {

	// Verifica se o item ainda pode ser comprado (nao esta bloqueado)
	if (itens[item]->locked) {
		MaxUpgrade(item);
		printf("\n");
	}
	else {
		// Verifica se o item ja foi comprado e em qual nivel de upgrade esta
		int nivel = VerifyLevel(item);
		// Verifica no arquivo se o item nao esta no ultimo nivel de upgrade
		if (nivel < 3) {
			nivel++;
			// Verifica se ha dinheiro suficiente
            if (Game::GetInstance().GetCoins() >= itens[item]->GetValue()) {
                BuyItem(item, nivel);
            }
            else {
                NoMoney(item);
            }
        }
        else {
            itens[item]->locked = true;
            MaxUpgrade(item);
        }
    }

}


void StoreState::BuyItem(int item, int nivel) {
	// Efetua a compra
	Game::GetInstance().SetCoins(Game::GetInstance().GetCoins() - itens[item]->GetValue());
	// Salva no arquivo o item que foi comprado
	fp = fopen("arquivos/save/storehistory.txt", "a");
	if (fp != NULL) {
		fprintf(fp, "%d %d\n", item, nivel);
		fclose(fp);
	}
	// Recupera a quantidade de moedas e escreve na tela e no arquivo
	money.SetValue(Game::GetInstance().GetCoins());
	fp = fopen("arquivos/save/coins.txt", "w");
	if (fp != NULL) {
		fprintf(fp, "%d\n", Game::GetInstance().GetCoins());
		fclose(fp);
	}
	resultados.clear();
    coinFX.Play(0);

}


void StoreState::NoMoney(int item) {

	nextState = new MessageWindow(1, "Você não tem dinheiro suficiente!");
	nextState->previousState = this;
	strcpy(result.descricao, "item");
	result.tipo = TIPO_INT;
	result.intValue = item;
	resultados.clear();
	resultados.emplace_back(result);
	Game::GetInstance().Push(nextState);

}


void StoreState::MaxUpgrade(int item) {

	nextState = new MessageWindow(1, "Este item já está atualizado!");
	nextState->previousState = this;
	strcpy(result.descricao, "item");
	result.tipo = TIPO_INT;
	result.intValue = item;
	resultados.clear();
	resultados.emplace_back(result);
	Game::GetInstance().Push(nextState);

}


int StoreState::VerifyLevel(int item) {

	FILE *fp;
	int idItemFp = -1, nivelItemFp, nivel = 0;

	fp = fopen("arquivos/save/storehistory.txt", "r");
	if (fp != NULL) {
		while (fscanf(fp, "%d %d", &idItemFp, &nivelItemFp) > 0) {
			if (idItemFp == item) {
				nivel = nivelItemFp;
			}
			getc(fp); // pega "\n"
		}
		fclose(fp);
	}
	else {
		fp = fopen("arquivos/save/storehistory.txt", "w");
		if (fp != NULL) {
			fclose(fp);
		}
	}
	return nivel;

}


void StoreState::ManageItems() {

	std::string nomePowerUp, level, description;
    int value = 0, id = 0, nivel = 0;
    for (int i = 0; i < numItens; i++) {
		// Determina qual o power up
        switch (i) {
            case 0: // super pulo
				id = 0;
                nomePowerUp = "arquivos/img/states/store/superpulo";
				description = "arquivos/img/states/store/descriptions/description-superpulo";
                break;
            case 1: // intangibilidade
				id = 1;
                nomePowerUp = "arquivos/img/states/store/intangibilidade";
				description = "arquivos/img/states/store/descriptions/description-intangibilidade";
                break;
            case 2: // revive
				id = 2;
                nomePowerUp = "arquivos/img/states/store/revive";
				description = "arquivos/img/states/store/descriptions/description-revive";
                break;
        }
		// Verifica o nivel do power up
		nivel = VerifyLevel(i);
        switch (nivel) {
            case 0:
                level = "-locked.png";
                value = 50;
                break;
            case 1:
                level = "-1.png";
                value = 100;
                break;
            case 2:
                level = "-2.png";
                value = 150;
                break;
            case 3:
                level = "-3.png";
                value = 200;
                break;
        }
        nomePowerUp.append(level);
		description.append(level);
        if (itens.size() == numItens) {
            itens[i] = new StoreItem(nomePowerUp, description, value, id, nivel, 150 + i * 230 + 50, 200);
        }
        else {
            itens.emplace_back(new StoreItem(nomePowerUp, description, value, id, nivel, 150 + i * 230 + 50, 200));
        }
    }

}
