

#ifndef __Avenida_Paulista__StoreState__
#define __Avenida_Paulista__StoreState__
#include <string.h>
#include <memory>

#include "State.h"
#include "Sprite.h"
#include "Text.h"
#include "Timer.h"
#include "ColorRect.h"
#include "StoreItem.h"
#include "Money.h"
#include "MessageWindow.h"


class StoreState: public State{
	void Input();
	Sprite bg;
	Text txBack;
    ColorRect rectRed;
	Money money;
	int numItens;
	std::vector<StoreItem*> itens;
	void BuyItem(int item, int nivel);	
	void CanBuy(int item);
	void NoMoney(int item);
	void MaxUpgrade(int item);
	int VerifyLevel(int item);
	void ManageItems();
	FILE *fp;
public:
	StoreState();
	~StoreState();
	void Update(float dt);
	void Render();
	Resultado result;
	
};

#endif /* defined(__Avenida_Paulista__StoreState__) */
