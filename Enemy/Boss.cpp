#include "Boss.h"
#include <DxLib.h>
#include "../Util/DrawFunctions.h"
#include "../Game/ShotFactory.h"
#include "../Game/Player.h"
#include "../Game/HpBar.h"
#include "../game.h"
#include "../Util/Sound.h"
namespace
{
	constexpr int kCutManTouchAttackPower = 4;//�ڐG�������̍U����
	
	//�{�X�A�j���[�V����
	constexpr int kGraphNum = 13;	//�{�X�O���t�B�b�N�̐�
	constexpr int kGraphSizeWidth = 2600 / kGraphNum;//�{�X�O���t�B�b�N�T�C�Y
	constexpr int kGraphSizeHeight = 400;	//�{�X�O���t�B�b�N�T�C�Y
	constexpr float kDrawScale = 0.5f;		//�{�X�O���t�B�b�N�g�嗦
	constexpr int kAnimFrameSpeed = 5;	//�{�X�O���t�B�b�N�A�j���[�V�����X�s�[�h

	//�ʏ픚���A�j���[�V����
	constexpr int burst_img_width = 32;//�摜�T�C�YX
	constexpr int burst_img_height = 32;//�摜�T�C�YY
	constexpr float burst_draw_scale = 1.0f;//�g�嗦
	constexpr int burst_frame_num = 8;//�A�j���[�V��������
	constexpr int burst_frame_speed = 5;//�A�j���[�V�����X�s�[�h
	//�{�X�����A�j���[�V����
	constexpr int boss_burst_img_width = 100;//�摜�T�C�YX
	constexpr int boss_burst_img_height = 100;//�摜�T�C�YY
	constexpr float boss_burst_draw_scale = 2.0f;//�g�嗦
	constexpr int boss_burst_frame_num = 61;//�A�j���[�V��������
	constexpr int boss_burst_frame_speed = 1;//�A�j���[�V�����X�s�[�h
}

Boss::Boss(std::shared_ptr<Player>player, const Position2& pos, int handle, int bossBurstH, int burstH, std::shared_ptr<ShotFactory> sFactory, std::shared_ptr<ItemFactory> itFactory, std::shared_ptr<HpBar>hp) :
	EnemyBase(player, pos, sFactory,itFactory), m_updateFunc(&Boss::NormalUpdate), m_drawFunc(&Boss::NormalDraw),
	m_shotFrame(0)
{
	m_hp = hp;
	m_isLeft = true;
	m_handle = handle;
	m_bossBurstH = bossBurstH;
	m_burstHandle = burstH;
	m_rect = { {pos.x,pos.y - 8.0f},
		{static_cast<int>(kGraphSizeWidth * Game::kScale * kDrawScale / 2) - 20,static_cast<int>(kGraphSizeHeight * Game::kScale * kDrawScale) - 20} };
}

Boss::~Boss()
{
	DeleteGraph(m_bossBurstH);
}

void Boss::Update()
{
	(this->*m_updateFunc)();
}

void Boss::Draw()
{
	(this->*m_drawFunc)();
}

int Boss::TouchAttackPower() const
{
	return kCutManTouchAttackPower;
}

void Boss::Damage(int damage)
{
	m_hp->Damage(damage);

	if (m_hp->GetHp() == 0)
	{
		SoundManager::GetInstance().Play(SoundId::EnemyBurst);
		m_updateFunc = &Boss::BurstUpdate;
		m_drawFunc = &Boss::BurstDraw;
		m_idx = 0;
		return;
	}
	SoundManager::GetInstance().Play(SoundId::EnemyHit);
}

bool Boss::IsCollidable() const
{
	//BurstUpdate�łȂ������G���Ԃ�0�̎�������
	return (m_updateFunc != &Boss::BurstUpdate) && m_ultimateTimer == 0;
}

void Boss::NormalUpdate()
{
	//�A�j���[�V����
	if (m_animFrame++ > kAnimFrameSpeed)
	{
		m_idx = (m_idx + 1) % kGraphNum;
		m_animFrame = 0;
	}
	//���G���Ԃ����鎞�͎��Ԃ����炵�Ă���
	if (--m_ultimateTimer <= 0)
	{
		m_ultimateTimer = 0;
	}

	//1�b���Ƃɒe��ł�
	if (m_frame++ == 60)
	{
		m_frame = 0;
		m_isOnDamage = false;

		//HP��3����2��؂�������U������
		if (m_hp->GetHp() <= (m_hp->GetMaxHp() / 3) * 2)
		{
			m_shotFrame = 20;
		}
		else
		{
			m_shotFrame = 10;
		}
		m_updateFunc = &Boss::ShotUpdate;
	}
}

void Boss::NormalDraw()
{
	//���G���ԓ_�ł�����
	if ((m_ultimateTimer / 10) % 2 == 1)	return;
	//�{�X�\��
	int img = m_idx * kGraphSizeWidth;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		img, 0, kGraphSizeWidth, kGraphSizeHeight, kDrawScale * Game::kScale, 0.0f, m_handle, true, m_isLeft);
#ifdef _DEBUG
	DrawFormatString(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y), 0xffffff, L"%d", m_idx);
	m_rect.Draw(0xaaffaa);
#endif
}

void Boss::BurstUpdate()
{
	//�����A�j���[�V����
	m_idx += 1;
	if (m_idx == (boss_burst_frame_num * 2) * boss_burst_frame_speed)
	{
		//�����A�j���[�V�������I������瑶�݂�����
		m_isExist = false;
	}
}

void Boss::BurstDraw()
{
	//�ʏ픚���G�t�F�N�g���d�˂ĕ\��
	int animNum = (m_idx / burst_frame_speed);
	int imgX = (animNum % burst_frame_num) * burst_img_width;
	int imgY = 0;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x + 20), static_cast<int>(m_rect.center.y + 50),
		imgX, imgY, burst_img_width, burst_img_height, burst_draw_scale * Game::kScale, 0.0f, m_burstHandle, true, false);
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x - 40), static_cast<int>(m_rect.center.y + 50),
		imgX, imgY, burst_img_width, burst_img_height, burst_draw_scale * Game::kScale, 0.0f, m_burstHandle, true, false);
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x + 20), static_cast<int>(m_rect.center.y - 50),
		imgX, imgY, burst_img_width, burst_img_height, burst_draw_scale * Game::kScale, 0.0f, m_burstHandle, true, false);
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x - 40), static_cast<int>(m_rect.center.y - 50),
		imgX, imgY, burst_img_width, burst_img_height, burst_draw_scale * Game::kScale, 0.0f, m_burstHandle, true, false);

	//�{�X�����G�t�F�N�g��\��
	animNum = (m_idx / boss_burst_frame_speed);
	if (animNum >= boss_burst_frame_num)
	{
		animNum -= boss_burst_frame_num;
	}
	imgX = animNum % 8 * boss_burst_img_width;
	imgY = animNum / 8 * boss_burst_img_height;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		imgX, imgY, boss_burst_img_width, boss_burst_img_height, boss_burst_draw_scale * Game::kScale, 0.0f, m_bossBurstH, true, false);
#ifdef _DEBUG
	DrawFormatStringF(m_rect.center.x, m_rect.center.y, 0x000000, L"%d", animNum);
	DrawFormatStringF(m_rect.center.x, m_rect.center.y - 20, 0x000000, L"X%d,Y%d", imgX / boss_burst_img_width, imgY / boss_burst_img_height);
#endif
}

void Boss::ShotUpdate()
{
	//���@�_���e�����@���@�_���x�N�g��=�I�_(�v���C���[���W)�@-�@�n�_(�G�@���g�̍��W)
	auto vel = m_player->GetRect().GetCenter() - m_rect.center;

	if (vel.SQLength() == 0.0f)
	{
		vel = { -2.0f,0.0f };//�G�@�̎������d�Ȃ��Ă���Ƃ��͊���̕����@�^���ɔ�΂��Ă���
	}
	else
	{
		vel.Normalize();
	}
	//�V���b�g��ł�
	if (m_shotFrame-- % 10 == 0)
	{
		m_shotFactory->Create(ShotType::ShotBattery, m_rect.center, vel, m_isLeft, false);
	}

	if (m_shotFrame <= 0)
	{
		//���̎w����҂�
		m_updateFunc = &Boss::NormalUpdate;
		return;
	}
}
