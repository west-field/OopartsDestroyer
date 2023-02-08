#include "GameoverScene.h"
#include <DxLib.h>
#include "../game.h"
#include "../Util/Sound.h"
#include "../Util/InputState.h"
#include "../Util/DrawFunctions.h"
#include "SceneManager.h"
#include "TitleScene.h"

void GameoverScene::FadeInUpdat(const InputState& input)
{
	//ž‚Ç‚ñ‚Ç‚ñ–¾‚é‚­‚È‚é
	m_fadeValue = 255 * m_fadeTimer / kFadeInterval;
	if (--m_fadeTimer == 0)
	{
		m_updateFunc = &GameoverScene::NormalUpdat;
		m_fadeValue = 0;
	}
}

void GameoverScene::NormalUpdat(const InputState& input)
{
	if (input.IsTriggered(InputType::next))
	{
		m_updateFunc = &GameoverScene::FadeOutUpdat;
		m_fadeColor = 0x000000;
	}
}

void GameoverScene::FadeOutUpdat(const InputState& input)
{
	m_fadeValue = 255 * m_fadeTimer / kFadeInterval;
	if (++m_fadeTimer == kFadeInterval)
	{
		m_manager.ChangeScene(new TitleScene(m_manager));
		return;
	}
}

GameoverScene::GameoverScene(SceneManager& manager) : Scene(manager) , m_updateFunc(&GameoverScene::FadeInUpdat) {
	//m_gameoverH = my::MyLoadGraph(L"Data/img/gameover.png");
	Sound::StartBgm(Sound::BgmGameover);
}

GameoverScene::~GameoverScene()
{
	//DeleteGraph(m_gameoverH);
	Sound::StopBgm(Sound::BgmGameover);
}

void
GameoverScene::Update(const InputState& input)
{
	(this->*m_updateFunc)(input);
}

void
GameoverScene::Draw()
{
	//DrawRotaGraph(Game::kScreenWidth / 2, Game::kScreenHeight / 2, 1.0f, 0.0f, m_gameoverH, true);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeValue);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, m_fadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	SetFontSize(50);
	DrawString(Game::kScreenWidth / 3, Game::kScreenHeight / 3, L"Gameover", 0xffffff);
	SetFontSize(0);
}