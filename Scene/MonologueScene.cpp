#include "MonologueScene.h"
#include <DxLib.h>

#include "../game.h"

#include "../Util/Sound.h"
#include "../Util/InputState.h"
#include "../Util/DrawFunctions.h"
#include "../Util/Graph.h"

#include "SceneManager.h"
#include "GameplayingScene.h"
#include "OptionScene.h"

#include "../Game/HpBar.h"
#include "../Game/Player.h"
#include "../Game/ShotFactory.h"

void MonologueScene::FadeInUpdat(const InputState& input)
{
	//◇どんどん明るくなる
	m_fadeValue = 255 * static_cast<int>(m_fadeTimer) / static_cast<int>(kFadeInterval);
	ChangeVolumeSoundMem(255 - m_fadeValue,m_BgmH);
	if (--m_fadeTimer == 0)
	{
		m_updateFunc = &MonologueScene::NormalUpdat;
		m_fadeValue = 0;
	}
}

void MonologueScene::NormalUpdat(const InputState& input)
{
	//背景
	Background::GetInstance().Update();//m_scroll = m_scroll + 1;
	
	if (m_monoX+=4 >= Game::kScreenWidth)
	{
		m_monoX = Game::kScreenWidth;
		//「次へ」ボタンが押されたら次シーンへ移行する
		if (input.IsTriggered(InputType::next))
		{
			SoundManager::GetInstance().Play(SoundId::Determinant);
			m_updateFunc = &MonologueScene::FadeOutUpdat;
		}
	}

	//「次へ」ボタンが押されたらすべて表示する
	else if (input.IsTriggered(InputType::next))
	{
		m_monoX = Game::kScreenWidth;
	}
}

void MonologueScene::FadeOutUpdat(const InputState& input)
{
	m_fadeValue = 255 * static_cast<int>(m_fadeTimer) / static_cast<int>(kFadeInterval);
	ChangeVolumeSoundMem(255 - m_fadeValue,m_BgmH);
	if (++m_fadeTimer == kFadeInterval)
	{
		m_manager.ChangeScene(new GameplayingScene(m_manager));
		return;
	}
}

void MonologueScene::SetBlock()
{

}

MonologueScene::MonologueScene(SceneManager& manager) : Scene(manager),m_updateFunc(&MonologueScene::FadeInUpdat)
{	
	m_monoH = my::MyLoadGraph(L"Data/mono.png");
	m_monoX = 100;
	m_hp = std::make_shared<HpBar>();
	m_player = std::make_shared<Player>(Position2{ 0.0f,0.0f },m_hp);
	m_shot = std::make_shared<ShotFactory>();
	
	Background::GetInstance().Init();
	m_BgmH = LoadSoundMem(L"Sound/BGM/noranekonokuchibue.mp3");
	ChangeVolumeSoundMem( 0, m_BgmH);
	PlaySoundMem(m_BgmH, DX_PLAYTYPE_LOOP,true);
}

MonologueScene::~MonologueScene()
{
	DeleteSoundMem(m_BgmH);
	DeleteGraph(m_monoH);
}

void
MonologueScene::Update(const InputState& input)
{
	//◇メンバ関数ポインタを呼び出す　演算子　->*
	(this->*m_updateFunc)(input);
}

void MonologueScene::Draw()
{
	//背景
	Background::GetInstance().Draw(0);
	
	//my::MyDrawRectRotaGraph(Game::kScreenWidth / 2, Game::kScreenHeight / 2, 0, 0, m_monoX, 900, 1.0f, 0.0f, m_monoH, true, false);
	DrawRectGraph(0, 0, 0, 0, m_monoX, 900, m_monoH, true, false);
#ifdef _DEBUG

#endif


	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeValue);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

