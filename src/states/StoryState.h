// StoryState.h - historia de abertura com load da fase em paralelo.

#ifndef __Ripa_na_Xulipa__StoryState__
#define __Ripa_na_Xulipa__StoryState__

#include <vector>

#include "State.h"
#include "Sprite.h"
#include "Music.h"
#include "Timer.h"

class LoadState;

// Historia de abertura, exibida ao clicar em NEW GAME
// (a menos que a introducao esteja desativada nas opcoes).
// Cada cena fica 5s na tela com fade. As cenas 2 e 3 aparecem
// juntas, lado a lado.
class StoryState : public State {

private:
    static constexpr float SLOT_TIME = 5.0f;
    static constexpr float FADE_TIME = 1.0f;
    static constexpr float FAST_TIME = 0.5f;

    std::vector<Sprite> scenes;
    Sprite slotB; // segunda imagem do slot 2+3
    Music storyMusic;
    Timer timer;
    int current = 0;
    float slotTime = 0;
    float alpha = 0;

//  Load da fase em paralelo com a historia (para entrar no jogo sem
//  tela de espera). Se a historia acabar antes do load, o LoadState
//  assume com a tela de progresso.
    LoadState* loader = nullptr;

    float TransFrom(int slot);
    void ApplyAlpha();
    void Input();
    void DropLoader();
    void GoToGame();
    void GoToTitle();

public:
    StoryState();
    ~StoryState();
    void Update(float dt);
    void Render();

};

#endif /* defined(__Ripa_na_Xulipa__StoryState__) */
