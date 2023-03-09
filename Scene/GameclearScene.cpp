#include "GameclearScene.h"
#include <DxLib.h>
#include "../game.h"
#include "../Util/Sound.h"
#include "../Util/Graph.h"
#include "../Util/InputState.h"
#include "../Util/DrawFunctions.h"
#include "SceneManager.h"
#include "TitleScene.h"

#include "../Game/Player.h"

namespace
{
	const TCHAR* const kMoji[GameclearScene::kMojiNum] =
	{
		L"ゲ",
		L"ー",
		L"ム",
		L"ク",
		L"リ",
		L"ア",
	};

	constexpr float kMojiNum = 30.0f;
	constexpr float kMoveNum = 2.0f;
}

GameclearScene::GameclearScene(SceneManager& manager, std::shared_ptr<Player>player) :
	Scene(manager), m_player(player), m_updateFunc(&GameclearScene::FadeInUpdat),
	m_drawFunc(&GameclearScene::NormalDraw) {
	Background::GetInstance().Init();
	float posX = (Game::kScreenWidth - kMojiNum * kMojiSize) / 2;
	for (int i = 0; i < kMojiNum; i++)
	{
		m_moji[i].pos = { static_cast<float>(posX + i * kMojiSize) ,Game::kScreenHeight / 3 };
		m_moji[i].moveY = i * -1.0f;
		m_moji[i].add = 0.5f;
	}
	m_BgmH = LoadSoundMem(L"Sound/BGM/emerald.mp3");
}

GameclearScene::~GameclearScene()
{
	DeleteSoundMem(m_BgmH);
	SoundManager::GetInstance().StopBgm(SoundId::EnemyShot);
}

void GameclearScene::Update(const InputState& input)
{
	Background::GetInstance().Update();
	(this->*m_updateFunc)(input);
}

void GameclearScene::Draw()
{
	Background::GetInstance().Draw(0);
	m_player->Draw();
	(this->*m_drawFunc)();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeValue);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, m_fadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameclearScene::FadeInUpdat(const InputState& input)
{
	m_fadeValue = 255 * m_fadeTimer / kFadeInterval;
	if (--m_fadeTimer == 0)
	{
		m_updateFunc = &GameclearScene::NormalUpdat;
		m_fadeValue = 0;
		SoundManager::GetInstance().Play(SoundId::Gameclear);
	}
}
void GameclearScene::FadeOutUpdat(const InputState& input)
{
	m_fadeValue = 255 * m_fadeTimer / kFadeInterval;
	ChangeVolumeSoundMem(255 - m_fadeValue, m_BgmH);
	if (++m_fadeTimer == kFadeInterval)
	{
		m_manager.ChangeScene(new TitleScene(m_manager));
		return;
	}
}

void GameclearScene::NormalUpdat(const InputState& input)
{
	m_player->Update();
	//プレイヤーを画面中央に移動させる
	auto vel = Vector2{ static_cast<float>(Game::kScreenWidth / 2),static_cast<float>(Game::kScreenHeight / 2) } - m_player->GetRect().GetCenter();

	float Num = vel.SQLength();
	if (Num <= kMoveNum)
	{
		vel = { 0.0f,0.0f };
		m_player->Action(ActionType::grah_jump);
		ChangeVolumeSoundMem(0, m_BgmH);
		PlaySoundMem(m_BgmH, DX_PLAYTYPE_LOOP, true);
		m_updateFunc = &GameclearScene::MojiUpdate;
		m_drawFunc = &GameclearScene::MojiDraw;
		return;
	}
	else
	{
		vel.Normalize();
		vel *= kMoveNum;
		m_player->ScaleEnlarge(0.05f);
	}

	m_player->Movement(vel);
}

void GameclearScene::MojiUpdate(const InputState& input)
{
	if (m_soundVolume++ >= 255)
	{
		m_soundVolume = 255;
	}
	ChangeVolumeSoundMem(m_soundVolume, m_BgmH);

	m_player->Update();
	m_player->Action(ActionType::grah_jump);

	for (auto& moji : m_moji)
	{
		if (moji.moveY > kMojiNum)
		{
			moji.add *= -1.0f;
		}
		else if (moji.moveY < -kMojiNum)
		{
			moji.add *= -1.0f;
		}
		moji.moveY += moji.add;
	}

	if (input.IsTriggered(InputType::next))
	{
		m_updateFunc = &GameclearScene::FadeOutUpdat;
		m_fadeColor = 0x000000;
	}
}

void GameclearScene::NormalDraw()
{
}

void GameclearScene::MojiDraw()
{
	SetFontSize(kMojiSize);
	for (int i = 0; i < kMojiNum; i++)
	{
		DrawStringF(m_moji[i].pos.x, m_moji[i].pos.y + m_moji[i].moveY, kMoji[i], 0xffff00);
	}
	SetFontSize(0);
}



