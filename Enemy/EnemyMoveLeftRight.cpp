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

	constexpr int burst_img_width = 32;//�摜�T�C�YX
	constexpr int burst_img_height = 32;//�摜�T�C�YY
	constexpr float burst_draw_scale = 1.0f;//�g�嗦
	constexpr int burst_frame_num = 8;//�A�j���[�V��������
	constexpr int burst_frame_speed = 5;//�A�j���[�V�����X�s�[�h
}

EnemyMoveLeftRight::EnemyMoveLeftRight(std::shared_ptr<Player> player, const Position2 pos, int handle, int burstH, std::shared_ptr<ShotFactory> sFactory) :
	EnemyBase(player, pos, sFactory), m_updateFunc(&EnemyMoveLeftRight::NormalUpdate),m_drawFunc(&EnemyMoveLeftRight::NormalDraw)
{
	m_handle = handle;
	m_burstHandle = burstH;
	m_rect = { pos,{kLeftRightSize,kLeftRightSize} };
	m_hp->MaxHp(3);
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
	(this->*m_drawFunc)();
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

void EnemyMoveLeftRight::Damage(int damage)
{
	m_hp->Damage(damage);
	//m_ultimateTimer = kUltimateFrame;//���G����
	if (m_hp->GetHp() == 0)
	{
		m_updateFunc = &EnemyMoveLeftRight::BurstUpdate;
		m_drawFunc = &EnemyMoveLeftRight::BurstDraw;
		m_idx = 0;
	}
}

bool EnemyMoveLeftRight::IsCollidable() const
{
	return (m_updateFunc != &EnemyMoveLeftRight::BurstUpdate);
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

void EnemyMoveLeftRight::NormalDraw()
{
	if (!m_isExist)	return;
	int img = m_idx * kLeftRightSize;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		img, 0, kLeftRightSize, kLeftRightSize, kDrawScall, 0.0f, m_handle, true, m_isLeft);
#ifdef _DEBUG
	m_rect.Draw(0xff00ff);
#endif
}

void EnemyMoveLeftRight::BurstUpdate()
{
	m_idx++;
	if (m_idx == burst_frame_num * burst_frame_speed)
	{
		m_isExist = false;
	}
}

void EnemyMoveLeftRight::BurstDraw()
{
	int imgX = (m_idx / burst_frame_speed) * burst_img_width;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		imgX, 0, burst_img_width, burst_img_height, burst_draw_scale, 0.0f, m_burstHandle, true, false);
}
