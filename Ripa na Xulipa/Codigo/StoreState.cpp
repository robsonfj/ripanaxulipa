
#include "StoreState.h"
#include "Game.h"

StoreState::StoreState(){

	// Inicializa textos 
	txBack = *new Text(FONTE, 50, Text::TEXT_BLENDED, "BACK", padraoCor, 80, 50);
//    inicializa um quadrado
    rectRed = *new ColorRect(0, 0, 1024, 600);
	// Abre as sprites
	bg = Sprite::Sprite("arquivos/img/store/storebg.png");
	// Seta as escalas dos sprites
	// Adiciona os itens
	numItens = 3;
	ManageItems();
	// Recupera a quantidade de moedas e escreve na tela
	money = Money::Money(Game::GetInstance().GetCoins());
}

StoreState::~StoreState() {
	itens.clear();
}

void StoreState::Input(){
//  Se a tecla for ESC, setar a flag para deletar esse estado
	if((InputManager::GetInstance().KeyPress(ESCAPE_KEY))){
		requestDelete = true;
	}
	
//	se condicao de saida for atendido
	if(InputManager::GetInstance().ShouldQuit())
		requestQuit = true;
	
//	mudar a Cor do texto e se ele for clicado entra na tela correspondente
	if (InputManager::GetInstance().IsMouseInside(txBack.box)) {
		
		txBack.SetColor(padraoCorSelect);
		if(InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON)){
			requestDelete = true;
		}
	}
	else
		txBack.SetColor(padraoCor);

	// Verifica se algum item foi clicado
	for (int i = 0; i < numItens; i++) {
		if (InputManager::GetInstance().IsMouseInside(itens[i]->box)) {
			itens[i]->Selected();
			if (InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON)){
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
}


void StoreState::Render(){

//  renderiza o quadrado vermelho
    rectRed.Render(RED);
//  Renderiza as sprites
	bg.Render();
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
	fprintf(fp, "%d %d\n", item, nivel);
	fclose(fp);
	// Recupera a quantidade de moedas e escreve na tela e no arquivo
	money.SetValue(Game::GetInstance().GetCoins());
	fp = fopen( "arquivos/save/score.txt", "w");
	fprintf(fp, "%d %d\n", Game::GetInstance().GetCoins(), Game::GetInstance().GetHighScore());
	fclose(fp);
	resultados.clear();
}

void StoreState::NoMoney(int item) {
	nextState = new MessageWindow(1, "Voce nao tem dinheiro suficiente!");
	nextState->previousState = this;
	strcpy(result.descricao, "item");
	result.tipo = TIPO_INT;
	result.intValue = item;
	resultados.clear();
	resultados.emplace_back(result);
	Game::GetInstance().Push(nextState);
}

void StoreState::MaxUpgrade(int item) {
	nextState = new MessageWindow(1, "Este item ja esta atualizado ao maximo!");
	nextState->previousState = this;
	strcpy(result.descricao, "item");
	result.tipo = TIPO_INT;
	result.intValue = item;
	resultados.clear();
	resultados.emplace_back(result);
	Game::GetInstance().Push(nextState);
}

int StoreState::VerifyLevel(int item) {
	int idItemFp = -1, nivelItemFp, nivel = 0;

	fp = fopen("arquivos/save/storehistory.txt", "r");
	while (fscanf(fp, "%d %d", &idItemFp, &nivelItemFp) > 0) {
		if (idItemFp == item) {
			nivel = nivelItemFp;
		}
		getc(fp); // pega "\n"
	}
	fclose(fp);
	return nivel;
}

void StoreState::ManageItems() {
    std::string nomePowerUp, level;
    int value = 0;
    for (int i = 0; i < numItens; i++) {
		// Determina qual o power up
        switch (i) {
            case 0: // super pulo
                nomePowerUp = "arquivos/img/store/superpulo";
                break;
            case 1: // intangibilidade
                nomePowerUp = "arquivos/img/store/intangibilidade";
                break;
            case 2: // revive
                nomePowerUp = "arquivos/img/store/revive";
                break;
        }
		// Verifica o nivel do power up
        switch (VerifyLevel(i)) {
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
                value = 200;
                break;
            case 3:
                level = "-3.png";
                value = 200;
                break;
        }
        nomePowerUp.append(level);
        if (itens.size() == numItens) {
            itens[i] = new StoreItem(nomePowerUp, value, 150 + i * 200 + 50, 220);
        }
        else {
            itens.emplace_back(new StoreItem(nomePowerUp, value, 150 + i * 200 + 50, 220));
        }
    }
}