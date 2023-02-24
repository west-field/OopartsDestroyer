#include "TitleScene.h"
#include <DxLib.h>

#include "../game.h"

#include "../Util/Sound.h"
#include "../Util/InputState.h"
#include "../Util/DrawFunctions.h"

#include "SceneManager.h"
#include "GameplayingScene.h"
#include "OptionScene.h"

namespace
{
	constexpr float bg_scale = 4.0f;//背景拡大率
}

void TitleScene::FadeInUpdat(const InputState& input)
{
	//◇どんどん明るくなる
	m_fadeValue = 255 * static_cast<int>(m_fadeTimer) / static_cast<int>(kFadeInterval);
	if (--m_fadeTimer == 0)
	{
		m_updateFunc = &TitleScene::NormalUpdat;
		m_fadeValue = 0;
	}
}

void TitleScene::NormalUpdat(const InputState& input)
{
	m_scroll = m_scroll + 1;

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
		Sound::Play(Sound::Cursor);
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
		Sound::Play(Sound::Determinant);
		m_updateFunc = &TitleScene::FadeOutUpdat;
	}
}

void TitleScene::FadeOutUpdat(const InputState& input)
{
	m_fadeValue = 255 * static_cast<int>(m_fadeTimer) / static_cast<int>(kFadeInterval);
	if (++m_fadeTimer == kFadeInterval)
	{
		//現在選択中の状態によって処理を分岐
		switch (m_selectNum)
		{
		case menuGameStart:
			m_manager.ChangeScene(new GameplayingScene(m_manager));
			return;
		case menuConfig:
			m_manager.ChangeScene(new OptionScene(m_manager));
			return;
		case menuGameEnd:
			m_manager.SetIsEnd();
			return;
		}
	}
}

TitleScene::TitleScene(SceneManager& manager) : Scene(manager),m_updateFunc(&TitleScene::FadeInUpdat)
{
	/*m_titleH = my::MyLoadGraph(L"Data/title.png");
	m_bgH = my::MyLoadGraph(L"Data/bg.png");*/
	
	m_bgImgs[0].handle = my::MyLoadGraph(L"Data/Background.png");//bg.jpg
	//m_bgImgs[1].handle = my::MyLoadGraph(L"Data/Background.png");
	//m_bgImgs[2].handle = my::MyLoadGraph(L"Data/Background.png");

	m_bgImgs[0].scrollSpeed = 0.5f;	//一番遠い
	m_bgImgs[1].scrollSpeed = 0.75f;//中間
	m_bgImgs[2].scrollSpeed = 1.0f;	//一番手前

	for (auto& bg : m_bgImgs)
	{
		GetGraphSize(bg.handle, &bg.imgSize.w, &bg.imgSize.h);
	}
	
	Sound::StartBgm(Sound::BgmTitle);
}

TitleScene::~TitleScene()
{
	/*DeleteGraph(m_titleH);
	DeleteGraph(m_bgH);*/
	Sound::StopBgm(Sound::BgmTitle);
}

void
TitleScene::Update(const InputState& input)
{
	//◇メンバ関数ポインタを呼び出す　演算子　->*
	(this->*m_updateFunc)(input);
}

void
TitleScene::Draw()
{
	//DrawExtendGraph(0, 0, Game::kScreenWidth - 1, Game::kScreenHeight - 1, m_bgH, true);
	/*DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x87CEEB, true);
	DrawRotaGraph(Game::kScreenWidth / 2, Game::kScreenHeight / 3, 6.0, 0.0, m_titleH, true, false);*/
	
	//背景
	for (auto& bg : m_bgImgs)
	{
		auto bgWidth = bg.imgSize.w * bg_scale;
		int scroll = static_cast<int>(m_scroll * bg.scrollSpeed) % static_cast<int>(bgWidth);

		DrawRotaGraph(bgWidth / 2 - scroll,
			bg.imgSize.h / 2 * bg_scale,//表示中心Y座標
			bg_scale,
		  0.0f,
		  bg.handle, true, false, false);
		DrawRotaGraph(bgWidth * (1 + 0.5) - scroll,
			bg.imgSize.h / 2 * bg_scale,
			bg_scale, 
			0.0f, 
			bg.handle, true, false, false);
		break;
	}

	//メニュー項目を描画
	SetFontSize(kMenuFontSize);
	DrawFormatString(SelectMenu[menuGameStart].x, SelectMenu[menuGameStart].y, 0xffffff, L"ゲームスタート");
	DrawFormatString(SelectMenu[menuConfig].x, SelectMenu[menuConfig].y, 0xffffff, L"設定");
	DrawFormatString(SelectMenu[menuGameEnd].x, SelectMenu[menuGameEnd].y, 0xffffff, L"ゲーム終了");
	SetFontSize(0);


	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeValue);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}