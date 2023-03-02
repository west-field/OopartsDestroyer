#include "GameoverScene.h"
#include <DxLib.h>
#include "../game.h"
#include "../Util/Sound.h"
#include "../Util/Font.h"
#include "../Util/InputState.h"
#include "../Util/DrawFunctions.h"
#include "SceneManager.h"
#include "TitleScene.h"

#include "../Game/Player.h"

void GameoverScene::FadeInUpdat(const InputState& input)
{
	//‚Ç‚ñ‚Ç‚ñ–¾‚é‚­‚È‚é
	m_fadeValue = 255 * m_fadeTimer / kFadeInterval;
	if (--m_fadeTimer == 0)
	{
		m_updateFunc = &GameoverScene::NormalUpdat;
		m_fadeValue = 0;
		Sound::Play(Sound::Gameover);
	}
}

void GameoverScene::NormalUpdat(const InputState& input)
{
	m_player->Update();

	//ƒvƒŒƒCƒ„[‚ğ‰æ–Ê’†‰›‚ÉˆÚ“®‚³‚¹‚é
	auto vel = Vector2{static_cast<float>(Game::kScreenWidth / 2),static_cast<float>(Game::kScreenHeight / 2)} - m_player->GetRect().GetCenter();

	float Num = vel.SQLength();
	if (Num <= 0.2f)
	{
		m_player->Action(ActionType::grah_death);
		vel = { 0.0f,0.0f };
	}
	else
	{
		vel.Normalize();
		vel *= 1.0f;
		m_player->ScaleEnlarge(0.03f);
	}

	m_player->Movement(vel);

	if (!m_player->IsExist() && input.IsTriggered(InputType::next))
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

GameoverScene::GameoverScene(SceneManager& manager, std::shared_ptr<Player>player) : 
	Scene(manager) , m_player(player), m_updateFunc(&GameoverScene::FadeInUpdat) {
	//m_gameoverH = my::MyLoadGraph(L"Data/img/gameover.png");
}

GameoverScene::~GameoverScene()
{
	//DeleteGraph(m_gameoverH);
	Sound::StopBgm(Sound::Gameover);
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

	m_player->Draw();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeValue);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, m_fadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	Font::ChangeFontSize(50);
	DrawString(Game::kScreenWidth / 3, Game::kScreenHeight / 3, L"Gameover", 0xffffff);
	Font::ChangeFontSize(0);
}