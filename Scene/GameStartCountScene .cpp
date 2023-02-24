#include "GameStartCountScene.h"
#include <DxLib.h>
#include "../game.h"
#include "../Util/Sound.h"
#include "../Util/InputState.h"
#include "SceneManager.h"
#include "TitleScene.h"


GameStartCountScene::GameStartCountScene(SceneManager& manager) : Scene(manager), m_updateFunc(&GameStartCountScene::CountDownUpdat)
{
	/*Sound::Play(Sound::Count);*/
}

GameStartCountScene::~GameStartCountScene()
{
}

void GameStartCountScene::Update(const InputState& input)
{
	(this->*m_updateFunc)(input);
}
void GameStartCountScene::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_MULA, 196);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0xffffff, false);

	if (m_isTuru)
	{
		DrawString(kStartMenuPosX, kStartMenuPosY - kLevelFontSize, L"ゲームレベル設定", 0xffffff);
		DrawFormatString(kStartMenuPosX + 200, kStartMenuPosY - kLevelFontSize, 0xffffff, L"Level %d", 1 + m_selectLevel * 4);
		//メニュー項目を描画
		SetFontSize(kLevelFontSize);
		DrawFormatString(m_SelectLevel[Easy].x, m_SelectLevel[Easy].y, 0xffffff, L"Easy");
		DrawFormatString(m_SelectLevel[Normal].x, m_SelectLevel[Normal].y, 0xffffff, L"Normal");
		DrawFormatString(m_SelectLevel[Hard].x, m_SelectLevel[Hard].y, 0xffffff, L"Hard");
		SetFontSize(0);
	}
	else
	{
		int count = static_cast<int>(m_count) / 60;
		SetFontSize(kLevelFontSize);
		if (count > 0)
		{
			DrawFormatString(Game::kScreenWidth / 2, Game::kScreenHeight / 3, 0xffffff, L"%d", count);
		}
		else
		{
			DrawString(Game::kScreenWidth / 2, Game::kScreenHeight / 3, L"スタート", 0xffffff);
		}
		SetFontSize(0);
	}
}

void GameStartCountScene::NormalUpdat(const InputState& input)
{
	bool isPress = false;//キーが押されたかどうか
	if (input.IsTriggered(InputType::down))
	{
		m_selectLevel = (m_selectLevel + 1) % Max;//選択状態を一つ下げる
		isPress = true;
	}
	else if (input.IsTriggered(InputType::up))
	{
		m_selectLevel = (m_selectLevel + (Max - 1)) % Max;//選択状態を一つ上げる
		isPress = true;
	}

	if (isPress)
	{
		for (int i = 0; i < Max; i++)
		{
			if (i == m_selectLevel)
			{
				m_SelectLevel[i].x = kMovePosX;//移動位置に移動する
			}
			else
			{
				m_SelectLevel[i].x = kStartMenuPosX;//元の位置に戻す
			}
		}
	}

	if (input.IsTriggered(InputType::next))
	{
		m_isTuru = false;
		m_updateFunc = &GameStartCountScene::CountDownUpdat;
	}
}

void GameStartCountScene::CountDownUpdat(const InputState& input)
{
	if (--m_count <= 0)
	{
		m_manager.PopScene();
		return;
	}
}
