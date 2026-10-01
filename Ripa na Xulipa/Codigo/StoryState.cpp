
#include "StoryState.h"
#include "Game.h"
#include "LoadState.h"


StoryState::StoryState(): storyMusic("arquivos/audio/initmusic.mp3") {

	scenes.emplace_back("arquivos/img/states/init/1.jpg");
	scenes.emplace_back("arquivos/img/states/init/2.jpg");
	slotB = Sprite("arquivos/img/states/init/3.jpg");
	scenes.emplace_back("arquivos/img/states/init/4.jpg");
	scenes.emplace_back("arquivos/img/states/init/5.jpg");
	scenes.emplace_back("arquivos/img/states/init/6.jpg");
	scenes.emplace_back("arquivos/img/states/init/7.jpg");
	scenes.emplace_back("arquivos/img/states/init/8.jpg");
	scenes.emplace_back("arquivos/img/states/init/9.jpg");
	scenes.emplace_back("arquivos/img/states/init/10.jpg");
	scenes.emplace_back("arquivos/img/states/init/11.jpg");
	scenes.emplace_back("arquivos/img/states/init/12.jpg");

	ApplyAlpha();
	storyMusic.Play(-1);

//	Começa o load da fase em paralelo (sem tela de progresso).
	loader = new LoadState();

}


StoryState::~StoryState() {

	DropLoader();

}


// Libera o loader sem entrar no jogo (ao voltar ao menu, por exemplo).
// Se a thread ainda estiver carregando, espera terminar com seguranca.
void StoryState::DropLoader() {

	if (loader) {
		loader->DiscardLoad();
		delete loader;
		loader = nullptr;
	}

}


// Tempo de transicao (fade de saida) do slot atual para o proximo.
// Slots 6 ("8") e 7 ("9") tem transicao rapida de 0.5s.
float StoryState::TransFrom(int slot) {

	if (slot == 6 || slot == 7) {
		return FAST_TIME;
	}
	return FADE_TIME;

}


void StoryState::ApplyAlpha() {

	scenes[current].SetAlpha(alpha);
	if (current == 1) {
		slotB.SetAlpha(alpha);
	}

}


void StoryState::Input() {

//	Se a condicao de saida for atendida
	if (InputManager::GetInstance().ShouldQuit()) {
		requestQuit = true;
	}

//	ESC volta ao menu
	if (InputManager::GetInstance().KeyPress(ESCAPE_KEY)) {
		GoToTitle();
		return;
	}

//	ENTER pula a historia e vai direto ao jogo
	if (InputManager::GetInstance().KeyPress(ENTER_KEY)) {
		GoToGame();
	}

}


void StoryState::GoToGame() {

	storyMusic.Stop();
	requestDelete = true;
	if (loader && loader->sair) {
//		Load terminou durante a historia: entra direto na fase.
		StageState* stage = loader->ClaimStage();
		delete loader;
		loader = nullptr;
		stage->cachorroFX.Play(0);
		Game::GetInstance().Push(stage);
	}
	else if (loader) {
//		Load ainda rodando (ex: pulou a historia): o LoadState
//		assume com a tela de progresso de onde parou.
		LoadState* pending = loader;
		loader = nullptr;
		Game::GetInstance().Push(pending);
	}

}


void StoryState::GoToTitle() {

	storyMusic.Stop();
	DropLoader();
	requestDelete = true;

}


void StoryState::Update(float dt) {

	timer.Update(dt);
	Input();
	if (RequestedDelete() || RequestedQuit()) {
		return;
	}

	if (!storyMusic.IsPlaying()) {
		storyMusic.Play(-1);
	}

//	Load da fase roda em paralelo; a thread comeca uma vez.
	if (loader && !loader->loaded) {
		loader->ThreadLoad();
	}

	slotTime += dt;
	float t = TransFrom(current);
	float inT = (t < 1.0f) ? t : 1.0f;

	if (slotTime < inT) {
		alpha = slotTime / inT;
	}
	else if (slotTime < SLOT_TIME) {
		alpha = 1.0f;
	}
	else {
		alpha = 1.0f - (slotTime - SLOT_TIME) / t;
		if (alpha < 0.0f) {
			alpha = 0.0f;
		}
	}
	ApplyAlpha();

	if (slotTime >= SLOT_TIME + t) {
		current++;
		slotTime = 0.0f;
		alpha = 0.0f;
		if (current >= (int)scenes.size()) {
			GoToGame();
		}
		else {
			ApplyAlpha();
		}
	}

}


void StoryState::Render() {

	if (current < 0 || current >= (int)scenes.size()) {
		return;
	}
	if (current == 1) {
		scenes[1].Render(2, 0);
		slotB.Render(512, 0);
	}
	else {
		scenes[current].Render();
	}

}
