#include "TitleScene.h"
#include <DxLib.h>

#include "../game.h"

#include "../Util/Sound.h"
#include "../Util/Font.h"
#include "../Util/InputState.h"
#include "../Util/DrawFunctions.h"

#include "SceneManager.h"
#include "GameplayingScene.h"
#include "OptionScene.h"

namespace
{
	//constexpr float bg_scale = 1.0f;//�w�i�g�嗦
}

void TitleScene::FadeInUpdat(const InputState& input)
{
	//���ǂ�ǂ񖾂邭�Ȃ�
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
		Sound::Play(Sound::Cursor);
		for (int i = 0; i < menuNum; i++)
		{
			if (i == m_selectNum)
			{
				SelectMenu[i].x = kMovedPosX;//�ړ��ʒu�Ɉړ�����
			}
			else
			{
				SelectMenu[i].x = kOriginalPosX;//���̈ʒu�ɖ߂�
			}
		}
	}
	//�u���ցv�{�^���������ꂽ�玟�V�[���ֈڍs����
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
		//���ݑI�𒆂̏�Ԃɂ���ď����𕪊�
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
	
	//m_bgImgs[0].handle = my::MyLoadGraph(L"Data/Background.png");//bg.jpg
#if false
	bg_scale = 1.0f;
	//�R
	m_bgImgs[0].handle = my::MyLoadGraph(L"Data/bg1/sky.png");
	m_bgImgs[1].handle = my::MyLoadGraph(L"Data/bg1/clouds_1.png");
	m_bgImgs[2].handle = my::MyLoadGraph(L"Data/bg1/clouds_2.png");
	m_bgImgs[3].handle = my::MyLoadGraph(L"Data/bg1/rocks_1.png");
	m_bgImgs[4].handle = my::MyLoadGraph(L"Data/bg1/clouds_3.png");
	m_bgImgs[5].handle = my::MyLoadGraph(L"Data/bg1/rocks_2.png");
	m_bgImgs[6].handle = my::MyLoadGraph(L"Data/bg1/clouds_4.png");

	m_bgImgs[0].scrollSpeed = 0.5f;	//��ԉ���
	m_bgImgs[1].scrollSpeed = 0.5f;	//��ԉ���
	m_bgImgs[2].scrollSpeed = 0.75f;//����
	m_bgImgs[3].scrollSpeed = 0.75f;//����
	m_bgImgs[4].scrollSpeed = 1.0f;	//��Ԏ�O
	m_bgImgs[5].scrollSpeed = 1.0f;	//��Ԏ�O
	m_bgImgs[6].scrollSpeed = 1.0f;	//��Ԏ�O
#else
	bg_scale = 3.0f;
	//�p��
	m_bgImgs[0].handle = my::MyLoadGraph(L"Data/bg2/1.png");
	m_bgImgs[1].handle = my::MyLoadGraph(L"Data/bg2/2.png");
	m_bgImgs[2].handle = my::MyLoadGraph(L"Data/bg2/3.png");
	m_bgImgs[3].handle = my::MyLoadGraph(L"Data/bg2/4.png");
	m_bgImgs[4].handle = my::MyLoadGraph(L"Data/bg2/5.png");

	m_bgImgs[0].scrollSpeed = 0.5f;	//��ԉ���
	m_bgImgs[1].scrollSpeed = 0.6f;	//��ԉ���
	m_bgImgs[2].scrollSpeed = 0.75f;//����
	m_bgImgs[3].scrollSpeed = 0.8f;//����
	m_bgImgs[4].scrollSpeed = 1.0f;	//��Ԏ�O
#endif
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
	for (auto& bg : m_bgImgs)
	{
		DeleteGraph(bg.handle);
	}
	Sound::StopBgm(Sound::BgmTitle);
}

void
TitleScene::Update(const InputState& input)
{
	//�������o�֐��|�C���^���Ăяo���@���Z�q�@->*
	(this->*m_updateFunc)(input);
}

void
TitleScene::Draw()
{
	//�w�i
	for (auto& bg : m_bgImgs)
	{
		auto bgWidth = bg.imgSize.w * bg_scale;
		int scroll = static_cast<int>(m_scroll * bg.scrollSpeed) % static_cast<int>(bgWidth);

		DrawRotaGraph(bgWidth / 2 - scroll,
			bg.imgSize.h / 2 * bg_scale,//�\�����SY���W
			bg_scale,
		  0.0f,
		  bg.handle, true, false, false);
		DrawRotaGraph(bgWidth * (1 + 0.5) - scroll,
			bg.imgSize.h / 2 * bg_scale,
			bg_scale, 
			0.0f, 
			bg.handle, true, false, false);
	}

	//���j���[���ڂ�`��
	Font::ChangeFontSize(kTitleFontSize);
	DrawFormatString(Game::kScreenWidth / 3, Game::kScreenHeight / 2 - kTitleFontSize, 0xaaffaa, L"�I�[�p�[�c�f�B�t�F���_�[");
	Font::ChangeFontSize(kMenuFontSize);
	DrawFormatString(SelectMenu[menuGameStart].x, SelectMenu[menuGameStart].y, 0xaaffaa, L"�Q�[���X�^�[�g");
	DrawFormatString(SelectMenu[menuConfig].x, SelectMenu[menuConfig].y, 0xaaffaa, L"�����Ă�");
	DrawFormatString(SelectMenu[menuGameEnd].x, SelectMenu[menuGameEnd].y, 0xaaffaa, L"�����");
	Font::ChangeFontSize(0);


	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeValue);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}