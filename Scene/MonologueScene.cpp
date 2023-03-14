#include "MonologueScene.h"
#include <DxLib.h>

#include "../game.h"

#include "../Util/Sound.h"
#include "../Util/InputState.h"
#include "../Util/DrawFunctions.h"
#include "../Util/Graph.h"

#include "SceneManager.h"
#include "GameplayingScene.h"

#include "../Game/HpBar.h"
#include "../Game/Player.h"
#include "../Game/ShotFactory.h"

namespace
{
	constexpr int kButtonFontSize = 50;//�t�H���g�T�C�Y

	constexpr int kButtonSize = 480;//�{�^����̃T�C�Y
	constexpr float kButtonDrawScale = 0.1f;//�g�嗦
	constexpr int kButtonDrawSpeed = 10;//�摜���x
	//�v���C���[�O���t�B�b�N
	constexpr int kGraphSizeWidth = 32;		//�T�C�Y
	constexpr int kGraphSizeHeight = 32;	//�T�C�Y
	constexpr float kDrawScale = 2.0f;		//�g�嗦
	constexpr int kFrameSpeed = 10;		//�A�j���[�V�����X�s�[�h
}

void MonologueScene::FadeInUpdat(const InputState& input)
{
	//���ǂ�ǂ񖾂邭�Ȃ�
	m_fadeValue = 255 * static_cast<int>(m_fadeTimer) / static_cast<int>(kFadeInterval);
	ChangeVolumeSoundMem(200 - m_fadeValue,m_BgmH);
	if (--m_fadeTimer == 0)
	{
		m_updateFunc = &MonologueScene::NormalUpdat;
		m_fadeValue = 0;
	}
}

void MonologueScene::NormalUpdat(const InputState& input)
{
	m_monoX += 4;
	if (m_monoX >= Game::kScreenWidth)
	{
		m_monoX = Game::kScreenWidth;
		//���ׂĕ\�����ꂽ��A���̃V�[���ֈړ�
		m_updateFunc = &MonologueScene::PlayerOnStage;
		return;
	}
	//�u���ցv�{�^���������ꂽ�炷�ׂĕ\������
	else if (m_monoX >= 100 && input.IsTriggered(InputType::next))
	{
		m_monoX = Game::kScreenWidth;
	}
}

void MonologueScene::FadeOutUpdat(const InputState& input)
{
	m_fadeValue = 255 * static_cast<int>(m_fadeTimer) / static_cast<int>(kFadeInterval);
	ChangeVolumeSoundMem(200 - m_fadeValue,m_BgmH);
	if (++m_fadeTimer == kFadeInterval)
	{
		m_manager.ChangeScene(new GameplayingScene(m_manager));
		return;
	}
}

void MonologueScene::PlayerOnStage(const InputState& input)
{
	m_isPlayer = true;
	//�v���C���[����ʒ����Ɉړ�������
	auto vel = Vector2{ static_cast<float>(Game::kScreenWidth / 2),m_player.center.y } - m_player.center;

	if (input.IsTriggered(InputType::next))
	{
		SoundManager::GetInstance().Play(SoundId::Determinant);
		m_player.center+=vel;
		vel = Vector2{ static_cast<float>(Game::kScreenWidth / 2),m_player.center.y } - m_player.center;
	}

	float Num = vel.SQLength();
	if (Num <= 2.0f)
	{
		vel = { 0.0f,0.0f };

		if (button.buttonFram++ >= kButtonDrawSpeed)
		{
			button.buttonIdxX = (button.buttonIdxX + 1) % 3;
			button.buttonFram = 0;
		}
		m_isButton = true;
		if (input.IsTriggered(InputType::shot))
		{
			m_isShotFire = true;
			m_isButton = false;
			SoundManager::GetInstance().Play(SoundId::PlayeyShot);
			m_shotPos = m_player.center;
			m_idxY = 4;
			m_idxX = 0;
			m_updateFunc = &MonologueScene::PlayerMoveUpdate;
			return;
		}
	}
	else
	{
		vel.Normalize();
		vel *= 5.0f;
	}

	m_player.center+=vel;
	m_idxY = 1;
}

void MonologueScene::PlayerMoveUpdate(const InputState& input)
{
	m_shotPos += {8.0f, 0.0f};
	if (m_isShotFire)
	{
		if (m_idxY == 0)
		{
			m_isShotFire = false;
		}
		if (input.IsTriggered(InputType::next))
		{
			SoundManager::GetInstance().Play(SoundId::Determinant);
			m_shotPos.x = Game::kScreenWidth + 20;
		}
	}
	else
	{
		m_player.center += { 5.0f,0.0f };
		if (m_idxX == 0 && m_idxY == 0) 
		{
			m_idxY = 1;
			m_idxX = 0;
		}
		if (input.IsTriggered(InputType::next))
		{
			auto vel = Vector2{ static_cast<float>(Game::kScreenWidth),m_player.center.y } - m_player.center;
			m_player.center += vel;
			return;
		}
	}

	if (m_player.center.x >= Game::kScreenWidth + m_player.size.w)
	{
		m_updateFunc = &MonologueScene::FadeOutUpdat;
		return;
	}
	
}

MonologueScene::MonologueScene(SceneManager& manager) : Scene(manager),
m_updateFunc(&MonologueScene::FadeInUpdat)
{	
	m_monoH = my::MyLoadGraph(L"Data/mono.png");
	m_monoX = 100;
	
	m_shotH = my::MyLoadGraph(L"Data/rockBuster.png");
	m_shotPos = {-10.0f,-10.0f};

	m_playerH = my::MyLoadGraph(L"Data/player.png");
	m_player = { {0.0f,(Game::kMapScreenBottomY - Game::kDrawSize * 5)}, {kGraphSizeWidth, kGraphSizeHeight} };

	button.buttonFram = 0;
	button.buttonIdxX = 0;
	button.buttonIdxY = 2;
	button.buttonPos = { Game::kScreenWidth/2,m_player.center.y + m_player.size.h*2 };
	m_buttonH = my::MyLoadGraph(L"Data/button.png");

	Background::GetInstance().Init();
	m_BgmH = LoadSoundMem(L"Sound/BGM/noranekonokuchibue.mp3");
	ChangeVolumeSoundMem( 0, m_BgmH);
	PlaySoundMem(m_BgmH, DX_PLAYTYPE_LOOP,true);
}

MonologueScene::~MonologueScene()
{
	DeleteSoundMem(m_BgmH);
	DeleteGraph(m_monoH);
	DeleteGraph(m_shotH);
	DeleteGraph(m_buttonH);
	DeleteGraph(m_playerH);
}

void
MonologueScene::Update(const InputState& input)
{
	//�w�i
	Background::GetInstance().Update();
	if (m_frame++ > kFrameSpeed)
	{
		switch (m_idxY)
		{
		case 0:
			m_idxX = (m_idxX + 1) % (1);
			break;
		case 1:
			m_idxX = (m_idxX + 1) % (2);
			break;
		case  3:
		case  4:
			m_idxX = (m_idxX + 1) % (4);
			break;
		case  5:
			m_idxX = (m_idxX + 1) % (6);
			break;
		}
		if (m_idxX == 0)
		{
			m_idxY = 0;
		}

		m_frame = 0;
	}
	//�������o�֐��|�C���^���Ăяo���@���Z�q�@->*
	(this->*m_updateFunc)(input);
}

void MonologueScene::Draw()
{
	//�w�i
	Background::GetInstance().Draw(0);
	
	//����
	DrawRectGraph(0, 0, 0, 0, m_monoX, 900, m_monoH, true, false);
#ifdef _DEBUG
	DrawFormatString(0, 0, 0x000000, L"m_monoX=%d", m_monoX);
#endif
	
	if (m_isPlayer)
	{
		my::MyDrawRectRotaGraph(static_cast<int>(m_player.center.x), static_cast<int>(m_player.center.y - kGraphSizeHeight / 2),
			m_idxX * kGraphSizeWidth, m_idxY * kGraphSizeHeight, kGraphSizeWidth, kGraphSizeHeight, kDrawScale * Game::kScale, 0.0f, m_playerH, true, false);

		my::MyDrawGraph(static_cast<int>(m_shotPos.x), static_cast<int>(m_shotPos.y), m_shotH, true);
	}
	if (m_isButton)
	{
		SetFontSize(kButtonFontSize);
		DrawFormatString(static_cast<int>(button.buttonPos.x - 120)+3, 
			static_cast<int>(button.buttonPos.y - (kButtonSize* kButtonDrawScale/2))+3, 0x000000, L"shot");
		DrawFormatString(static_cast<int>(button.buttonPos.x - 120),
			static_cast<int>(button.buttonPos.y - (kButtonSize * kButtonDrawScale / 2)), 0x0000ff, L"shot");
		SetFontSize(0);
		my::MyDrawRectRotaGraph(static_cast<int>(button.buttonPos.x), static_cast<int>(button.buttonPos.y),
			static_cast<int>(button.buttonIdxX * kButtonSize), static_cast<int>(button.buttonIdxY * kButtonSize),
			kButtonSize, kButtonSize, kButtonDrawScale, 0.0f, m_buttonH, true, false);
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeValue);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

