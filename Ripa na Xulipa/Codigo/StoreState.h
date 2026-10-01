
#ifndef __Avenida_Paulista__StoreState__
#define __Avenida_Paulista__StoreState__

#include <cstring>
#include <string>
#include <memory>
#include <cstdio>

#include "State.h"
#include "Sprite.h"
#include "Text.h"
#include "Timer.h"
#include "ColorRect.h"
#include "StoreItem.h"
#include "Money.h"
#include "MessageWindow.h"
#include "Sound.h"
#include "Music.h"

class StoreState : public State {

private:
    int nrtxtselected = 0;
    FILE *fp;
	Sprite bg, bg2, title;
	Text txBack;
	Money money;
    Music storeMusic;
    Sound coinFX;
	int numItens;
	std::vector<StoreItem*> itens;

	void Input();
	void BuyItem(int item, int nivel);	
	void CanBuy(int item);
	void NoMoney(int item);
	void MaxUpgrade(int item);
	void ManageItems();
	
public:
	StoreState();
	~StoreState();

	static int VerifyLevel(int item);
	void Update(float dt);
	void Render();
	Resultado result;
	
};

#endif /* defined(__Avenida_Paulista__StoreState__) */
