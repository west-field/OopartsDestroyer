#include "GameoverScene.h"
#include <DxLib.h>
#include "../game.h"
#include "../Util/Sound.h"
#include "../Util/Graph.h"
#include "../InputState.h"
#include "../Util/DrawFunctions.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "GameplayingScene.h"

#include "../Game/Player.h"

namespace
{
	const TCHAR* const kMoji[GameoverScene::kMojiNum] =
	{
		L"G",
		L"a",
		L"m",
		L"e",
		L" ",
		L"O",
		L"v",
		L"e",
		L"r",
	};

	constexpr float kMojiNum = 30.0f;
	constexpr float kMoveNum = 2.0f;
	constexpr int kMojiSize = 90;
	constexpr int kMenuFontSize = 50;
}
GameoverScene::GameoverScene(SceneManager& manager, std::shared_ptr<Player>player) :
	Scene(manager), m_player(player), m_updateFunc(&GameoverScene::FadeInUpdat),
	m_drawFunc(&GameoverScene::NormalDraw) 
{
	Background::GetInstance().Init();
	float posX = (Game::kScreenWidth - kMojiNum * kMojiSize) / 2;
	for (int i = 0; i < kMojiNum; i++)
	{
		m_moji[i].pos = { static_cast<float>(posX + i * kMojiSize) ,Game::kScreenHeight / 3};
		m_moji[i].moveY = i * -1.0f;
		m_moji[i].add = 0.5f;
	}
	m_BgmH = LoadSoundMem(L"Sound/BGM/shizukanoumi.mp3");
}

GameoverScene::~GameoverScene()
{
	DeleteSoundMem(m_BgmH);
	SoundManager::GetInstance().StopBgm(SoundId::Gameover);
}

void
GameoverScene::Update(const InputState& input)
{
	Background::GetInstance().Update();
	(this->*m_updateFunc)(input);
}

void
GameoverScene::Draw()
{
	Background::GetInstance().Draw(0);

	(this->*m_drawFunc)();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeValue);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, m_fadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameoverScene::FadeInUpdat(const InputState& input)
{
	m_fadeValue = 255 * m_fadeTimer / kFadeInterval;
	if (--m_fadeTimer == 0)
	{
		m_updateFunc = &GameoverScene::NormalUpdat;
		m_fadeColor = 0x000000;
		m_fadeValue = 0;
		SoundManager::GetInstance().Play(SoundId::Gameover);
	}
}

void GameoverScene::NormalUpdat(const InputState& input)
{
	m_player->Update();

	//�v���C���[����ʒ����Ɉړ�������
	auto vel = Vector2{static_cast<float>(Game::kScreenWidth / 2),static_cast<float>(Game::kScreenHeight / 2)} - m_player->GetRect().GetCenter();

	float Num = vel.SQLength();
	if (Num <= kMoveNum)
	{
		m_player->Action(ActionType::grah_death);
		vel = { 0.0f,0.0f };
	}
	else
	{
		vel.Normalize();
		vel *= kMoveNum;
		m_player->ScaleEnlarge(0.03f);
	}

	m_player->Movement(vel);

	if (!m_player->IsExist())
	{
		m_updateFunc = &GameoverScene::MojiUpdate;
		m_drawFunc = &GameoverScene::MojiDraw;
		ChangeVolumeSoundMem(0, m_BgmH);
		PlaySoundMem(m_BgmH, DX_PLAYTYPE_LOOP, true);
		return;
	}
}

void GameoverScene::MojiUpdate(const InputState& input)
{
	if (m_soundVolume++ >= SoundManager::GetInstance().GetBGMVolume())
	{
		m_soundVolume = SoundManager::GetInstance().GetBGMVolume();
	}
	ChangeVolumeSoundMem(m_soundVolume, m_BgmH);

	//������h�炷
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

	//���j���[
	bool isPress = false;//�L�[�������ꂽ���ǂ���
	if (input.IsTriggered(InputType::down))
	{
		m_selectNum = (m_selectNum + 1) % menuNum;//�I����Ԃ��������
		isPress = true;
	}
	else if (input.IsTriggered(InputType::up))
	{
		m_selectNum = (m_selectNum + (menuNum - 1)) % menuNum;//�I����Ԃ���グ��
		isPress = true;
	}

	if (isPress)
	{
		SoundManager::GetInstance().Play(SoundId::Cursor);
		for (int i = 0; i < menuNum; i++)
		{
			if (i == m_selectNum)
			{
				SelectMenu[i].x = kMovedPosX;//�ړ��ʒu�Ɉړ�����
				SelectMenu[i].color = 0xffa0aa;//�F��ς���
			}
			else
			{
				SelectMenu[i].x = kOriginalPosX;//���̈ʒu�ɖ߂�
				SelectMenu[i].color = 0xaaa0ff;//���̐F�ɖ߂�
			}
		}
	}
	
	//�u���ցv�{�^���������ꂽ�玟�V�[���ֈڍs����
	if (!m_player->IsExist() && input.IsTriggered(InputType::next))
	{
		SoundManager::GetInstance().Play(SoundId::Determinant);

		m_updateFunc = &GameoverScene::FadeOutUpdat;
	}
}

void GameoverScene::NormalDraw()
{
	m_player->Draw();
}

void GameoverScene::MojiDraw()
{
	SetFontSize(kMojiSize);
	//DrawString(0, 0, L"�Q�[���I�[�o�[", 0xffffff);
	for (int i = 0;i < kMojiNum;i++)
	{
		DrawStringF(m_moji[i].pos.x+2, m_moji[i].pos.y+2 + m_moji[i].moveY, kMoji[i], 0x000000);
		DrawStringF(m_moji[i].pos.x , m_moji[i].pos.y + m_moji[i].moveY, kMoji[i], 0xff0000);
	}
	SetFontSize(0);

	SetFontSize(kMenuFontSize);
	DrawString(SelectMenu[menuGameEnd].x + 5, SelectMenu[menuGameEnd].y + 5, L"�^�C�g���ɖ߂�", 0x000000);
	DrawString(SelectMenu[menuGameEnd].x, SelectMenu[menuGameEnd].y, L"�^�C�g���ɖ߂�", SelectMenu[menuGameEnd].color);
	DrawString(SelectMenu[menuRestart].x + 5, SelectMenu[menuRestart].y + 5, L"�ŏ�����", 0x000000);
	DrawString(SelectMenu[menuRestart].x, SelectMenu[menuRestart].y, L"�ŏ�����", SelectMenu[menuRestart].color);
	SetFontSize(0);
}

void GameoverScene::FadeOutUpdat(const InputState& input)
{
	m_fadeValue = 255 * m_fadeTimer / kFadeInterval;
	ChangeVolumeSoundMem(SoundManager::GetInstance().GetBGMVolume() - m_fadeValue, m_BgmH);
	if (++m_fadeTimer == kFadeInterval)
	{
		switch (m_selectNum)
		{
		case menuRestart:
			m_manager.ChangeScene(new GameplayingScene(m_manager));
			return;
		case menuGameEnd:
		default:
			m_manager.ChangeScene(new TitleScene(m_manager));
			return;
		}
	}
}

