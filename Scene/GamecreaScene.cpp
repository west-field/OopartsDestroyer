#include "GamecreaScene.h"
#include <DxLib.h>
#include "../game.h"
#include "../Util/Sound.h"
#include "../Util/InputState.h"
#include "../Util/DrawFunctions.h"
#include "SceneManager.h"
#include "TitleScene.h"

void GamecreaScene::FadeInUpdat(const InputState& input)
{
	//ž‚Ç‚ñ‚Ç‚ñ–¾‚é‚­‚È‚é
	m_fadeValue = 255 * m_fadeTimer / kFadeInterval;
	if (--m_fadeTimer == 0)
	{
		m_updateFunc = &GamecreaScene::NormalUpdat;
		m_fadeValue = 0;
	}
}

void GamecreaScene::NormalUpdat(const InputState& input)
{
	if (input.IsTriggered(InputType::next))
	{
		m_updateFunc = &GamecreaScene::FadeOutUpdat;
		m_fadeColor = 0x000000;
	}
}

void GamecreaScene::FadeOutUpdat(const InputState& input)
{
	m_fadeValue = 255 * m_fadeTimer / kFadeInterval;
	if (++m_fadeTimer == kFadeInterval)
	{
		m_manager.ChangeScene(new TitleScene(m_manager));
		return;
	}
}

GamecreaScene::GamecreaScene(SceneManager& manager) : Scene(manager) , m_updateFunc(&GamecreaScene::FadeInUpdat) {
	//m_gameoverH = my::MyLoadGraph(L"Data/img/gameover.png");
	Sound::StartBgm(Sound::BgmGameover);
}

GamecreaScene::~GamecreaScene()
{
	//DeleteGraph(m_gameoverH);
	Sound::StopBgm(Sound::BgmGameover);
}

void
GamecreaScene::Update(const InputState& input)
{
	(this->*m_updateFunc)(input);
}

void
GamecreaScene::Draw()
{
	//DrawRotaGraph(Game::kScreenWidth / 2, Game::kScreenHeight / 2, 1.0f, 0.0f, m_gameoverH, true);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeValue);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, m_fadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	SetFontSize(50);
	DrawString(Game::kScreenWidth / 3, Game::kScreenHeight / 3, L"Gamecrea", 0xffffff);
	SetFontSize(0);
}