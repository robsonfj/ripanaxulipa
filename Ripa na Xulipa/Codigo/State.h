

#ifndef __IDJ__State__
#define __IDJ__State__

#include <memory>

#include "GameObject.h"
#include "Camera.h"

#define FONTE "arquivos/font/orangejuice.ttf"

#define TIPO_INT 0
#define TIPO_FLOAT 1
#define TIPO_CHAR 2
#define TIPO_BOOL 3
#define TIPO_UNDEF -1

typedef struct resultado {
	char descricao[50];
	int tipo = TIPO_UNDEF;
	union {
		int intValue;
		float floatValue;
		char charValue[50];
		bool boolValue;
	};
} Resultado;

class State {

protected:
	bool requestDelete;
	bool requestQuit;
	SDL_Color padraoCor, padraoCorSelect, padraoCorSelect2;
	
	virtual void UpdateArray(float dt);
	virtual void RenderArray();
	std::vector<std::unique_ptr<GameObject>> objectArray;
	
public:
	State();
	virtual ~State() {};
	
	virtual void Update(float dt) = 0;
	virtual void Render() = 0;
	
	virtual void AddObject(GameObject* object) { objectArray.emplace_back(object); };
	bool RequestedDelete() { return requestDelete; };
	bool RequestedQuit() { return requestQuit; };
	State* nextState = NULL;
	State* previousState = NULL;
	std::vector<Resultado> resultados;
    static int deletedpapercount;

};

#endif /* defined(__IDJ__State__) */
