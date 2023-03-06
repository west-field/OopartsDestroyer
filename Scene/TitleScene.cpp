#include "TitleScene.h"
#include <DxLib.h>

#include "../game.h"

#include "../Util/Sound.h"
#include "../Util/Font.h"
#include "../Util/InputState.h"
#include "../Util/DrawFunctions.h"
#include "../Util/Graph.h"

#include "SceneManager.h"
#include "GameplayingScene.h"
#include "OptionScene.h"

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
	Graph::BgUpdate();//m_scroll = m_scroll + 1;

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
	m_titleH = my::MyLoadGraph(L"Data/title.png");
	Graph::Init();
	Sound::StartBgm(Sound::BgmTitle);
}

TitleScene::~TitleScene()
{
	DeleteGraph(m_titleH);
	Sound::StopBgm(Sound::BgmTitle);
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
	Graph::BgDraw(0);

	//DrawString((Game::kScreenWidth - kTitleFontSize * 12) / 2, (Game::kScreenHeight / 2 - kTitleFontSize), L"オーパーツディフェンダー", m_color);
	my::MyDrawRectRotaGraph((Game::kScreenWidth / 2), (Game::kScreenHeight / 2), 0, 0, 3508, 2480, 0.45f, 0.0f, m_titleH, true, false);

	//メニュー項目を描画
	m_color = 0x000000;
	MenuDraw(5, 5);
	m_color = 0xaaa0ff;
	MenuDraw(0,0);


	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeValue);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TitleScene::MenuDraw(int X, int Y)
{
	Font::ChangeFontSize(kMenuFontSize);
	DrawString(SelectMenu[menuGameStart].x + X, SelectMenu[menuGameStart].y + Y, L"ゲームスタート", m_color);
	DrawString(SelectMenu[menuConfig].x + X, SelectMenu[menuConfig].y + Y, L"せってい", m_color);
	DrawString(SelectMenu[menuGameEnd].x + X, SelectMenu[menuGameEnd].y + Y, L"おわり", m_color);
	Font::ChangeFontSize(0);
}
