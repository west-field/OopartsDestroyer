#include "CutMan.h"
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

	constexpr float kSpeed = 4.0f;	//�ړ��X�s�[�h
	constexpr int kGraphNum = 13;	//�O���t�B�b�N�̐�
	constexpr int kGraphSizeWidth = 2600/kGraphNum;		//�T�C�Y
	constexpr int kGraphSizeHeight = 400;	//�T�C�Y
	constexpr float kDrawScale = 0.5f;		//�g�嗦
	constexpr int kAnimFrameSpeed = 5;	//�O���t�B�b�N�A�j���[�V�����X�s�[�h
	constexpr int kSizeX = 30;	//�O���t�B�b�N�T�C�Y
	constexpr int kSizeY = kSizeX;

	constexpr int kJumpInterval = 100;

	//�W�����v
	constexpr float kGravity = 0.8f;//�d��
	constexpr float kJumpAcc = -10.0f;//�W�����v��
	//�����A�j���[�V����
	constexpr int burst_img_width = 32;//�摜�T�C�YX
	constexpr int burst_img_height = 32;//�摜�T�C�YY
	constexpr float burst_draw_scale = 1.0f;//�g�嗦
	constexpr int burst_frame_num = 8;//�A�j���[�V��������
	constexpr int burst_frame_speed = 5;//�A�j���[�V�����X�s�[�h
}

CutMan::CutMan(std::shared_ptr<Player>player, const Position2& pos, int handle, int burstH,std::shared_ptr<ShotFactory> sFactory):
	EnemyBase(player,pos,sFactory),updateFunc(&CutMan::StopUpdate),m_drawFunc(&CutMan::NormalDraw),
	m_shotFrame(0), m_JumpFrame(kJumpInterval)
{
	m_hp->MaxHp(28);
	m_isLeft = true;
	m_handle = handle;
	m_burstHandle = burstH;
	m_rect = { {pos.x,pos.y-8.0f},{static_cast<int>(kGraphSizeWidth * Game::kScale * kDrawScale / 2),static_cast<int>(kGraphSizeHeight * Game::kScale * kDrawScale)} };
	//m_vec = { kSpeed, kJumpAcc };
}

CutMan::~CutMan()
{
}

void CutMan::Update()
{
	if (!m_isExist)return;
	(this->*updateFunc)();
}

void CutMan::Draw()
{
	if (!m_isExist)return;
	(this->*m_drawFunc)();
}

void CutMan::Movement(Vector2 vec)
{
	if (!m_isExist)	return;
	//��ʂƈꏏ�Ɉړ�
	m_rect.center += vec;
}

void CutMan::EnemyMovement(Vector2 vec)
{
	if (!m_isExist)	return;
	//�W�����v���Ă���Ƃ�
	m_rect.center.y += vec.y;
	if (m_isJump)
	{
		m_rect.center.x += vec.x;
	}
}

int CutMan::TouchAttackPower() const
{
	return kCutManTouchAttackPower;
}

void CutMan::Damage(int damage)
{
	m_hp->Damage(damage);
	SoundManager::GetInstance().Play(SoundId::EnemyHit);
	if (m_hp->GetHp() == 0)
	{
		SoundManager::GetInstance().Play(SoundId::EnemyBurst);
		updateFunc = &CutMan::BurstUpdate;
		m_drawFunc = &CutMan::BurstDraw;
		m_idx = 0;
	}
}

bool CutMan::IsCollidable() const
{
	return (updateFunc != &CutMan::BurstUpdate) && m_ultimateTimer == 0;
}

void CutMan::MoveUpdate()
{
	//���������Ă����������ς���
	if (m_isLeft) m_vec.x *= -1.0f;
	//�W�����v������
	m_vec.y = kJumpAcc - (GetRand(100) % 5);
	updateFunc = &CutMan::JumpUpdate;
	return;
}

void CutMan::StopUpdate()
{
	if (m_frame++ > kAnimFrameSpeed)
	{
		m_idx = (m_idx + 1) % kGraphNum;
		m_frame = 0;
	}

	if (--m_ultimateTimer <= 0)
	{
		m_ultimateTimer = 0;
	}
	return;

	//�_���[�W���󂯂���e��ł�
	if (m_isOnDamage)
	{
		if (m_hp->GetHp() <= m_hp->GetMaxHp() / 2)
		{
			updateFunc = &CutMan::TwoShotUpdate;
			m_isOnDamage = false;
			return;
		}
		else
		{
			updateFunc = &CutMan::OneShotUpdate;
			m_isOnDamage = false;
			return;
		}
	}
	//�e��ł��Ȃ��Ƃ��͈ړ�����
	else if(m_JumpFrame++ >= kJumpInterval)
	{
		m_JumpFrame = 0;
		
		//���������Ă����������ς���
		if (m_isLeft) m_vec.x *= -1.0f;
		//�W�����v������
		//m_vec.y = kJumpAcc - (GetRand(100) % 5);
		m_posTemp = m_rect.center.y + ( kJumpAcc - (GetRand(100) % 5));
		updateFunc = &CutMan::JumpUpdate;
		return;
	}
}

void CutMan::JumpUpdate()
{
	m_rect.center.y += m_vec.y;

	if (m_rect.center.y <= m_posTemp) 
	{
		updateFunc = &CutMan::DownUpdate;
		return;
	}
}

void CutMan::DownUpdate()
{
	m_vec.y += kGravity;//���ɗ�����

	m_rect.center.x += m_vec.x;

	if(!m_isJump)
	{
		//�v���C���[�̕����ɃW�����v
		if (m_player->GetRect().GetCenter().x > m_rect.center.x)
		{
			m_isLeft = false;
		}
		else
		{
			m_isLeft = true;
		}
		updateFunc = &CutMan::StopUpdate;
		return;
	}
}

void CutMan::OneShotUpdate()
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
		vel *= 2.0f;
	}
	//�V���b�g�����ł�
	m_shotFactory->Create(ShotType::RockBuster, m_rect.center, vel, m_isLeft);

	//���̎w����҂�
	m_JumpFrame = 0;
	updateFunc = &CutMan::StopUpdate;
	return;
}

void CutMan::TwoShotUpdate()
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
		vel *= 2.0f;
	}
	//�V���b�g����ł�
	if (m_shotFrame-- % 10 == 0)
	{
		m_shotFactory->Create(ShotType::RockBuster, m_rect.center, vel, m_isLeft);
	}

	if (m_shotFrame == 0)
	{
		m_shotFrame = 20;
		//���̎w����҂�
		m_JumpFrame = 0;
		updateFunc = &CutMan::StopUpdate;
		return;
	}
}

void CutMan::NormalDraw()
{
	//���G���ԓ_�ł�����
	if ((m_ultimateTimer / 10) % 2 == 1)	return;

	int img = m_idx * kGraphSizeWidth;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		img, 0, kGraphSizeWidth, kGraphSizeHeight, kDrawScale * Game::kScale, 0.0f, m_handle, true, m_isLeft);
#ifdef _DEBUG
	DrawFormatString(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y), 0xffffff, L"%d", m_idx);
	m_rect.Draw(0xaaffaa);
#endif
}

void CutMan::BurstUpdate()
{
	m_idx+=1;
	if (m_idx == burst_frame_num * burst_frame_speed)
	{
		m_isExist = false;
	}
}

void CutMan::BurstDraw()
{
	int imgX = (m_idx / burst_frame_speed) * burst_img_width;

	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		imgX, 0, burst_img_width, burst_img_height, burst_draw_scale * Game::kScale, 0.0f, m_burstHandle, true, false);

#ifdef _DEBUG
	DrawFormatStringF(m_rect.center.x, m_rect.center.y, 0xaaaaaa, L"%d", m_idx);
#endif
}
