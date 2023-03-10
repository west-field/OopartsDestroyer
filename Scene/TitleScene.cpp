#include "TitleScene.h"
#include <DxLib.h>

#include "../game.h"

#include "../Util/Sound.h"
#include "../Util/InputState.h"
#include "../Util/DrawFunctions.h"
#include "../Util/Graph.h"

#include "SceneManager.h"
#include "GameplayingScene.h"
#include "MonologueScene.h"
#include "PauseScene.h"

#include "../Game/HpBar.h"
#include "../Game/Player.h"
#include "../Enemy/EnemyBase.h"
#include "../Enemy/EnemyMoveLeft.h"
#include "../Game/ShotFactory.h"

void TitleScene::FadeInUpdat(const InputState& input)
{
	//◇どんどん明るくなる
	m_fadeValue = 255 * static_cast<int>(m_fadeTimer) / static_cast<int>(kFadeInterval);
	ChangeVolumeSoundMem(255 - m_fadeValue,m_BgmH);
	if (--m_fadeTimer == 0)
	{
		m_updateFunc = &TitleScene::NormalUpdat;
		m_fadeValue = 0;
	}
}

void TitleScene::NormalUpdat(const InputState& input)
{
	//背景
	Background::GetInstance().Update();//m_scroll = m_scroll + 1;
	//ブロック
	(this->*m_blockMove)();
	//敵
	int i = 0;
	for (auto& enemy : m_enemy)
	{
		if (enemy->IsExist())
		{
			enemy->Update();
			if (enemy->GetRect().GetCenter().x <= -enemy->GetRect().GetSize().w)
			{
				enemy->SetPos(Position2{ static_cast<float>(Game::kScreenWidth + Game::kDrawSize * (i + 2)),
					static_cast<float>(Game::kDrawSize * (i + 2))});
			}
		}
		i++;
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
			}
			else
			{
				SelectMenu[i].x = kOriginalPosX;//元の位置に戻す
			}
		}
	}
	//「次へ」ボタンが押されたら次シーンへ移行する
	if (input.IsTriggered(InputType::next))
	{
		SoundManager::GetInstance().Play(SoundId::Determinant);
		
		if (m_selectNum == menuConfig)
		{
			m_manager.PushScene(new PauseScene(m_manager));
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
	ChangeVolumeSoundMem(255 - m_fadeValue,m_BgmH);
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

void TitleScene::SetBlock()
{
	int numY = GetRand(100) % 4;
	int numX = GetRand(100) % 10 + 1;
	m_blocks.size.w = m_blocks.size.h = Game::ChipSize * 2;
	
	int num = numX % 2;
	switch (num)
	{
	case 0:
		m_blocks.idxX = numX - 1;
		break;
	case 1:
		m_blocks.idxX = numX;
		break;
	default:
		break;
	}
	num = numY % 2;
	switch (num)
	{
	case 0:
		m_blocks.idxY = numY;
		break;
	case 1:
		m_blocks.idxY = numY - 1;
		break;
	default:
		break;
	}

	int screenSizeX = Game::kScreenWidth / Game::kDrawSize;
	int tempX = GetRand(screenSizeX);
	int screenSizeY = Game::kScreenHeight / Game::kDrawSize;
	int tempY = GetRand(screenSizeY);
	m_blocks.pos = { static_cast<float>(tempX * Game::kDrawSize - Game::kDrawSize / 2),static_cast<float>(tempY * Game::kDrawSize -Game::kDrawSize / 2 )};
}

void TitleScene::BlockIn()
{
	m_fade = 255 * m_frame / 60;
	if (m_frame++ == 60)
	{
		m_blockMove=&TitleScene::BlockOut;
	}
}

void TitleScene::BlockOut()
{
	m_fade = 255 * m_frame / 60;
	if (m_frame-- == 0)
	{
		m_fade = 0;
		m_frame = 0;
		SetBlock();
		m_blockMove = &TitleScene::BlockIn;
	}
}

TitleScene::TitleScene(SceneManager& manager) : Scene(manager),m_updateFunc(&TitleScene::FadeInUpdat), m_blockMove(&TitleScene::BlockIn)
{	
	m_titleH = my::MyLoadGraph(L"Data/Title 1.png");
	m_blockH = my::MyLoadGraph(L"Data/map/mapchip.png");
	SetBlock();
	m_enemyH = my::MyLoadGraph(L"Data/goldenSpaceShuttle.png");
	m_hp = std::make_shared<HpBar>();
	m_player = std::make_shared<Player>(Position2{ 0.0f,0.0f },m_hp);
	m_shot = std::make_shared<ShotFactory>();
	for (int i = 0; i < 3; i++)
	{
		m_enemy[i] = std::make_shared<EnemyMoveLeft>(m_player,
			Position2{ static_cast<float>(Game::kScreenWidth - Game::kDrawSize * (i + 2)),
					static_cast<float>(Game::kDrawSize * (i + 2)) }, m_enemyH, 0, m_shot);
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
	DeleteGraph(m_blockH);
	DeleteGraph(m_enemyH);
}

void
TitleScene::Update(const InputState& input)
{
	//◇メンバ関数ポインタを呼び出す　演算子　->*
	(this->*m_updateFunc)(input);
}

void TitleScene::Draw()
{
	//背景
	Background::GetInstance().Draw(0);
	//ブロック
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fade);
	my::MyDrawRectRotaGraph(static_cast<int>(m_blocks.pos.x), static_cast<int>(m_blocks.pos.y),
		m_blocks.idxX*Game::ChipSize, m_blocks.idxY*Game::ChipSize,
		m_blocks.size.w, m_blocks.size.h, Game::kScale, 0.0f, m_blockH, true, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//敵
	for (auto& enemy : m_enemy)
	{
		enemy->Draw();
	}

	//タイトルロゴ表示
	my::MyDrawRectRotaGraph((Game::kScreenWidth / 2), (Game::kScreenHeight / 3), 0, 0, 3508, 2480, 0.45f, 0.0f, m_titleH, true, false);

	//メニュー項目を描画
	m_color = 0x000000;
	MenuDraw(5, 5);
	m_color = 0xaaa0ff;
	MenuDraw(0,0);

#ifdef _DEBUG

	DrawFormatString(0, 0,  0x000000, L"X%dY%d", m_blocks.idxX,m_blocks.idxY);
	DrawFormatString(0, 20,  0x000000, L"x%3f,y%3f", m_blocks.pos.x, m_blocks.pos.y);
#endif


	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeValue);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TitleScene::MenuDraw(int X, int Y)
{
	SetFontSize(kMenuFontSize);
	DrawString(SelectMenu[menuGameStart].x + X, SelectMenu[menuGameStart].y + Y, L"ゲームスタート", m_color);
	DrawString(SelectMenu[menuConfig].x + X, SelectMenu[menuConfig].y + Y, L"せってい", m_color);
	DrawString(SelectMenu[menuGameEnd].x + X, SelectMenu[menuGameEnd].y + Y, L"おわり", m_color);
	SetFontSize(0);
}
