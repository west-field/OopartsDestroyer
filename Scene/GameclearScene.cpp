#include "GameclearScene.h"
#include <DxLib.h>
#include "../game.h"
#include "../Util/Sound.h"
#include "../Util/InputState.h"
#include "../Util/DrawFunctions.h"
#include "SceneManager.h"
#include "TitleScene.h"

#include "../Game/Player.h"

void GameclearScene::FadeInUpdat(const InputState& input)
{
	//ÅûÇ«ÇÒÇ«ÇÒñæÇÈÇ≠Ç»ÇÈ
	m_fadeValue = 255 * m_fadeTimer / kFadeInterval;
	if (--m_fadeTimer == 0)
	{
		m_updateFunc = &GameclearScene::NormalUpdat;
		m_fadeValue = 0;
	}
}

void GameclearScene::NormalUpdat(const InputState& input)
{
	m_player->Update();
	//ÉvÉåÉCÉÑÅ[ÇâÊñ íÜâõÇ…à⁄ìÆÇ≥ÇπÇÈ
	auto vel = Vector2{ static_cast<float>(Game::kScreenWidth / 2),static_cast<float>(Game::kScreenHeight / 2) } - m_player->GetRect().GetCenter();

	float Num = vel.SQLength();
	if (Num <= 0.2f)
	{
		m_player->Action(ActionType::grah_jump);
		vel = { 0.0f,0.0f };
	}
	else
	{
		vel.Normalize();
		vel *= 1.0f;
		m_player->ScaleEnlarge(0.03f);
	}

	m_player->Movement(vel);

	if (input.IsTriggered(InputType::next))
	{
		m_updateFunc = &GameclearScene::FadeOutUpdat;
		m_fadeColor = 0x000000;
	}
}

void GameclearScene::FadeOutUpdat(const InputState& input)
{
	m_fadeValue = 255 * m_fadeTimer / kFadeInterval;
	if (++m_fadeTimer == kFadeInterval)
	{
		m_manager.ChangeScene(new TitleScene(m_manager));
		return;
	}
}

GameclearScene::GameclearScene(SceneManager& manager, std::shared_ptr<Player>player) :
	Scene(manager) ,m_player(player), m_updateFunc(&GameclearScene::FadeInUpdat) {
	//m_gameoverH = my::MyLoadGraph(L"Data/img/gameover.png");
	Sound::Play(Sound::Gameclear);
}

GameclearScene::~GameclearScene()
{
	//DeleteGraph(m_gameoverH);
	Sound::StopBgm(Sound::Gameclear);
}

void GameclearScene::Update(const InputState& input)
{
	(this->*m_updateFunc)(input);
}

void GameclearScene::Draw()
{
	m_player->Draw();
	//DrawRotaGraph(Game::kScreenWidth / 2, Game::kScreenHeight / 2, 1.0f, 0.0f, m_gameoverH, true);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeValue);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, m_fadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	SetFontSize(50);
	DrawString(Game::kScreenWidth / 3, Game::kScreenHeight / 3, L"Gameclear", 0xffffff);
	SetFontSize(0);
}