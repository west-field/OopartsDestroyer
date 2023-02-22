#include "CutMan.h"
#include <DxLib.h>
//#include "../Scene/GameplayingScene.h"
#include "../Util/DrawFunctions.h"
#include "../Game/ShotFactory.h"
#include "../Game/Player.h"

namespace
{
	constexpr int kCutManTouchAttackPower = 4;//�ڐG�������̍U����

	constexpr float kSpeed = 4.0f;	//�ړ��X�s�[�h
	constexpr int kSizeX = 30;	//�O���t�B�b�N�T�C�Y
	constexpr int kSizeY = kSizeX;

	constexpr int kJumpInterval = 100;

	//�W�����v
	constexpr float kGravity = 0.8f;//�d��
	constexpr float kJumpAcc = -10.0f;//�W�����v��
}

CutMan::CutMan(std::shared_ptr<Player>player, const Position2& pos, int handle,std::shared_ptr<ShotFactory> sFactory):
	EnemyBase(player,pos,sFactory),updateFunc(&CutMan::StopUpdate), m_frame(0)
{
	m_isLeft = true;
	m_handle = handle;
	m_rect = { pos,{kSizeX,kSizeY} };
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
	int img = m_idx * kSizeX;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y), img, 0, kSizeX, kSizeY, 1.0f, 0.0f, m_handle, true, m_isLeft);
}

void CutMan::OnDamage(int damage)
{
	EnemyBase::Damage(damage);
	m_isOnDamage = true;
}

void CutMan::Movement(Vector2 vec)
{
	if (!m_isExist)	return;
	m_rect.center += vec;
}

int CutMan::TouchAttackPower() const
{
	return kCutManTouchAttackPower;
}

void CutMan::MoveUpdate()
{
	//�ړ����x�����Ƃɖ߂�
	m_vec = { kSpeed, 0.0f };
	//���������Ă����������ς���
	if (m_isLeft) m_vec *= -1.0f;
	//�W�����v������
	m_vec.y = kJumpAcc - (GetRand(100) % 5);
	updateFunc = &CutMan::JumpUpdate;
	return;
}

void CutMan::StopUpdate()
{
	//�_���[�W���󂯂���e��ł�
	if (m_isOnDamage)
	{
		int rand = GetRand(10) % 2;
		switch (rand)
		{
		case 0:
			updateFunc = &CutMan::OneShotUpdate;
			return;
		case 1:
			updateFunc = &CutMan::TwoShotUpdate;
			return;
		default:
			break;
		}
	}
	//�e��ł��Ȃ��Ƃ��͈ړ�����
	else
	{
		updateFunc = &CutMan::MoveUpdate;
		return;
	}
}

void CutMan::JumpUpdate()
{
	//�W�����v�����ʒu���ǂ̎��͈ړ����Ȃ�
	if (m_chipId == 0)
	{
		m_rect.center += m_vec;//�v���C���[���ړ�
		m_vec.y += kGravity;//���ɗ�����
	}
	else
	{
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
	if (m_frame-- % 10 == 0)
	{
		m_shotFactory->Create(ShotType::RockBuster, m_rect.center, vel, m_isLeft);
	}

	if (m_frame == 0)
	{
		m_frame = 20;
		//���̎w����҂�
		updateFunc = &CutMan::StopUpdate;
		return;
	}
}
