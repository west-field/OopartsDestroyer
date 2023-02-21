#include "EnemyMoveLeftRight.h"
#include <DxLib.h>
#include "../Util/DrawFunctions.h"
#include "../Game/HpBar.h"

namespace
{
	constexpr int kLeftRightTouchAttackPower = 4;//�ڐG�������̍U����

	constexpr int anim_frame_speed = 5;//�ꖇ�ɕK�v�ȃt���[����
	constexpr int anim_frame_num = 3;//�A�j���[�V��������

	constexpr int kLeftRightSize = 32;//�O���t�B�b�N1�̑傫��
	constexpr float kDrawScall = 1.0f;//�O���t�B�b�N�g�嗦

	constexpr float kEnemyMoveSpeed = 4.0f;//�G�l�~�[�̈ړ����x
}

EnemyMoveLeftRight::EnemyMoveLeftRight(std::shared_ptr<Player> player, const Position2 pos, int handle, std::shared_ptr<ShotFactory> sFactory) :
	EnemyBase(player, pos, sFactory), m_handle(handle),m_updateFunc(&EnemyMoveLeftRight::NormalUpdate)
{
	int X, Y;
	GetGraphSize(m_handle, &X, &Y);
	m_rect = { pos,{X/anim_frame_num,Y} };
	m_hp->MaxHp(5);
}

EnemyMoveLeftRight::~EnemyMoveLeftRight()
{
}

void EnemyMoveLeftRight::Update()
{
	if (!m_isExist)	return;
	
	(this->*m_updateFunc)();
}

void EnemyMoveLeftRight::Draw()
{
	if (!m_isExist)	return;
	int img = m_idx * kLeftRightSize;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		img, 0, kLeftRightSize, kLeftRightSize, kDrawScall, 0.0f, m_handle, true, m_isLeft);
#ifdef _DEBUG
	m_rect.Draw(0xff00ff);
#endif
}

void EnemyMoveLeftRight::Movement(Vector2 vec)
{
	if (!m_isExist)	return;

	m_rect.center += vec;
}

int EnemyMoveLeftRight::TouchAttackPower() const
{
	return kLeftRightTouchAttackPower;
}

void EnemyMoveLeftRight::NormalUpdate()
{
	//2�b�Ԃ��炢�~�܂�
	if (m_frame++ >= 120)
	{
		//����
		m_idx = 1;
		//�t���[���J�E���g���[���ɂ���
		m_frame = 0;
		//�ǂ����������Ă��邩�ǂ���
		if (m_isLeft)
		{
			m_updateFunc = &EnemyMoveLeftRight::LeftUpdate;
		}
		else
		{
			m_updateFunc = &EnemyMoveLeftRight::RightUpdate;
		}
	}
	//�~�܂��Ă���Ԃ͖ڂ�����摜��\��������
	else
	{
		m_idx = 0;
	}
}

void EnemyMoveLeftRight::LeftUpdate()
{
	//�ڂ��J���Ă���摜
	m_idx = 2;
	//���Ɉړ�����
	m_rect.center.x -= kEnemyMoveSpeed;
	//�ǂɂԂ�������
	if (m_chipId == 1)
	{
		//����
		m_idx = 1;
		//�������t��
		m_isLeft = false;
		//�ʏ�̍X�V�ɐ؂�ւ���
		m_updateFunc = &EnemyMoveLeftRight::NormalUpdate;
	}
}

void EnemyMoveLeftRight::RightUpdate()
{
	m_idx = 2;
	//�E�Ɉړ�
	m_rect.center.x += kEnemyMoveSpeed;
	if (m_chipId == 1)
	{
		m_idx = 1;
		m_isLeft = true;
		m_updateFunc = &EnemyMoveLeftRight::NormalUpdate;
	}
}
