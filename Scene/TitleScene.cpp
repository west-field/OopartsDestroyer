#include "TitleScene.h"
#include <DxLib.h>

#include "../game.h"

#include "../Util/Sound.h"
#include "../InputState.h"
#include "../Util/DrawFunctions.h"
#include "../Util/Graph.h"

#include "SceneManager.h"
#include "GameplayingScene.h"
#include "MonologueScene.h"
#include "PauseScene.h"

#include "../Game/HpBar.h"
#include "../Game/Player.h"
#include "../Game/ShotFactory.h"
#include "../Game/ItemFactory.h"
#include "../Enemy/EnemyBase.h"
#include "../Enemy/EnemyMoveLeft.h"

namespace
{
	constexpr int kGearSize = 160;//画像サイズ
	constexpr float kGearScale = 2.0f;//表示拡大率
	constexpr int kGearNum = 2;//アニメーション枚数
	constexpr int kGearSpeed = 10;//アニメーションスピード
}

void TitleScene::FadeInUpdat(const InputState& input)
{
	//◇どんどん明るくなる
	m_fadeValue = 255 * static_cast<int>(m_fadeTimer) / static_cast<int>(kFadeInterval);
	ChangeVolumeSoundMem(SoundManager::GetInstance().GetBGMVolume() - m_fadeValue, m_BgmH);
	if (--m_fadeTimer == 0)
	{
		m_updateFunc = &TitleScene::NormalUpdat;
		m_fadeValue = 0;
	}
}

void TitleScene::NormalUpdat(const InputState& input)
{
	//背景
	Background::GetInstance().Update();
	//敵
	int i = 0;
	for (int i = 0;i < 3;i++)
	{
		if (m_enemy[i]->IsExist())
		{
			m_enemy[i]->Update();
			if (m_enemy[i]->GetRect().GetCenter().x <= -m_enemy[i]->GetRect().GetSize().w)
			{
				m_enemy[i]->SetPos(Position2{ static_cast<float>(Game::kScreenWidth + Game::kDrawSize * (i + 2)),
					static_cast<float>(Game::kDrawSize * (i + GetRand(50)/3))});
			}
		}
	}
	
	//メニュー
	bool isPress = false;//キーが押されたかどうか
	if (input.IsTriggered(InputType::down))
	{
		m_selectNum = (m_selectNum + 1) % menuNum;//選択状態を一つ下げる
		isPress = true;
	}
	else if (input.IsTriggered(InputType::up))
	{
		m_selectNum = (m_selectNum + (menuNum - 1)) % menuNum;//選択状態を一つ上げる
		isPress = true;
	}

	if (isPress)
	{
		SoundManager::GetInstance().Play(SoundId::Cursor);
		for (int i = 0; i < menuNum; i++)
		{
			if (i == m_selectNum)
			{
				SelectMenu[i].x = kMovedPosX;//移動位置に移動する
				SelectMenu[i].color = 0xffa0aa;//色を変える
			}
			else
			{
				SelectMenu[i].x = kOriginalPosX;//元の位置に戻す
				SelectMenu[i].color = 0xaaa0ff;//元の色に戻す
			}
		}
	}
	//「次へ」ボタンが押されたら次シーンへ移行する
	if (input.IsTriggered(InputType::next))
	{
		SoundManager::GetInstance().Play(SoundId::Determinant);
		
		if (m_selectNum == menuConfig)
		{
			m_manager.PushScene(new PauseScene(m_manager,m_BgmH));
			return;
		}
		else
		{
			m_updateFunc = &TitleScene::FadeOutUpdat;
		}
	}
}

void TitleScene::FadeOutUpdat(const InputState& input)
{
	m_fadeValue = 255 * static_cast<int>(m_fadeTimer) / static_cast<int>(kFadeInterval);
	ChangeVolumeSoundMem(SoundManager::GetInstance().GetBGMVolume() - m_fadeValue,m_BgmH);
	if (++m_fadeTimer == kFadeInterval)
	{
		//現在選択中の状態によって処理を分岐
		switch (m_selectNum)
		{
		case menuGameStart:
			//m_manager.ChangeScene(new GameplayingScene(m_manager));
			m_manager.ChangeScene(new MonologueScene(m_manager));
			return;
		case menuGameEnd:
			m_manager.SetIsEnd();
			return;
		}
	}
}

TitleScene::TitleScene(SceneManager& manager) : Scene(manager),m_updateFunc(&TitleScene::FadeInUpdat)
{	
	m_titleH = my::MyLoadGraph(L"Data/title.png");
	m_gearH = my::MyLoadGraph(L"Data/gear.png");
	m_enemyH = my::MyLoadGraph(L"Data/goldenSpaceShuttle.png");
	m_hp = std::make_shared<HpBar>(Position2{0.0f,0.0f});
	m_player = std::make_shared<Player>(Position2{ 0.0f,0.0f },m_hp);
	m_shot = std::make_shared<ShotFactory>();
	m_item = std::make_shared<ItemFactory>();
	for (int i = 0; i < 3; i++)
	{
		m_enemy[i] = std::make_shared<EnemyMoveLeft>(m_player,
			Position2{ static_cast<float>(Game::kScreenWidth + Game::kDrawSize * (i + 2)),
					static_cast<float>(Game::kDrawSize * (i + GetRand(50) / 3)) }, m_enemyH, 0, m_shot,m_item);
	}
	Background::GetInstance().Init();
	m_BgmH = LoadSoundMem(L"Sound/BGM/noranekonokuchibue.mp3");
	ChangeVolumeSoundMem( 0, m_BgmH);
	PlaySoundMem(m_BgmH, DX_PLAYTYPE_LOOP,true);
}

TitleScene::~TitleScene()
{
	DeleteSoundMem(m_BgmH);
	DeleteGraph(m_titleH);
	DeleteGraph(m_gearH);
	DeleteGraph(m_enemyH);
}

void
TitleScene::Update(const InputState& input)
{
	//◇メンバ関数ポインタを呼び出す　演算子　->*
	(this->*m_updateFunc)(input);
	m_gearIdx++;
	if (m_gearIdx == kGearNum * kGearSpeed)
	{
		m_gearIdx = 0;
	}
}

void TitleScene::Draw()
{
	//背景
	Background::GetInstance().Draw();
	//敵
	for (auto& enemy : m_enemy)
	{
		enemy->Draw();
	}

	//歯車
	int img = m_gearIdx / kGearSpeed * kGearSize;
	my::MyDrawRectRotaGraph((Game::kScreenWidth / 2), (Game::kScreenHeight / 3), img, 0, kGearSize, kGearSize, kGearScale, 0.0f, m_gearH, true, false);
	SetDrawBlendMode(DX_BLENDMODE_ADD, 50);//加算合成
	my::MyDrawRectRotaGraph((Game::kScreenWidth / 2), (Game::kScreenHeight / 3), img, 0, kGearSize, kGearSize, kGearScale, 0.0f, m_gearH, true, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//タイトルロゴ表示
	my::MyDrawRectRotaGraph((Game::kScreenWidth / 2), (Game::kScreenHeight / 3), 0, 0, 3508, 2480, 0.45f, 0.0f, m_titleH, true, false);

	//メニュー項目を描画
	SetFontSize(kMenuFontSize);
	for (auto& menu : SelectMenu)
	{
		DrawString(menu.x+5, menu.y+5, menu.name, 0x000000);
		DrawString(menu.x, menu.y, menu.name, menu.color);
	}
	SetFontSize(0);

#ifdef _DEBUG
	DrawFormatString(0, 40,  0x000000, L"m_gearIdx%d", m_gearIdx);
#endif


	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeValue);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
