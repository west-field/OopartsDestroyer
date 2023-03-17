#include "EnemyMoveLR.h"
#include <DxLib.h>
#include "../Util/DrawFunctions.h"
#include "../Game/HpBar.h"
#include "../game.h"
#include "../Util/Sound.h"
#include "../Game/ItemFactory.h"

namespace
{
	constexpr int kLeftRightTouchAttackPower = 4;//�ڐG�������̍U����

	//�G�l�~�[�A�j���[�V����
	constexpr int anim_frame_speed = 5;//�ꖇ�ɕK�v�ȃt���[����
	constexpr int anim_frame_num = 3;//�A�j���[�V��������
	constexpr int kLeftRightSize = 32;//�O���t�B�b�N1�̑傫��
	constexpr float kDrawScall = 1.0f;//�O���t�B�b�N�g�嗦
	constexpr float kEnemyMoveSpeed = 4.0f;//�G�l�~�[�̈ړ����x

	//�����A�j���[�V����
	constexpr int burst_img_width = 32;//�摜�T�C�YX
	constexpr int burst_img_height = 32;//�摜�T�C�YY
	constexpr float burst_draw_scale = 1.0f;//�g�嗦
	constexpr int burst_frame_num = 8;//�A�j���[�V��������
	constexpr int burst_frame_speed = 5;//�A�j���[�V�����X�s�[�h
}

EnemyMoveLR::EnemyMoveLR(std::shared_ptr<Player> player, const Position2 pos, int handle, int burstH, std::shared_ptr<ShotFactory> sFactory, std::shared_ptr<ItemFactory> itFactory) :
	EnemyBase(player, pos, sFactory,itFactory), m_updateFunc(&EnemyMoveLR::NormalUpdate),m_drawFunc(&EnemyMoveLR::NormalDraw)
{
	m_handle = handle;
	m_burstHandle = burstH;
	m_rect = { pos,{static_cast<int>(kLeftRightSize * Game::kScale),static_cast<int>(kLeftRightSize * Game::kScale)} };
	m_hp->MaxHp(3);

	if (GetRand(100) / 2 == 0)
	{
		m_isLeft = true;
	}
}

EnemyMoveLR::~EnemyMoveLR()
{

}

void EnemyMoveLR::Update()
{
	(this->*m_updateFunc)();
}

void EnemyMoveLR::Draw()
{
	(this->*m_drawFunc)();
}

int EnemyMoveLR::TouchAttackPower() const
{
	return kLeftRightTouchAttackPower;
}

void EnemyMoveLR::Damage(int damage)
{
	m_hp->Damage(damage);
	
	if (m_hp->GetHp() == 0)
	{
		SoundManager::GetInstance().Play(SoundId::EnemyBurst);
		m_updateFunc = &EnemyMoveLR::BurstUpdate;
		m_drawFunc = &EnemyMoveLR::BurstDraw;
		m_idx = 0;
		if (GetRand(100) % 2 == 0)
		{
			m_itemFactory->Create(ItemType::Heal, m_rect.center);
		}
		return;
	}
	SoundManager::GetInstance().Play(SoundId::EnemyHit);
}

bool EnemyMoveLR::IsCollidable() const
{
	//BurstUpdate�̎��͓�����Ȃ�
	return (m_updateFunc != &EnemyMoveLR::BurstUpdate);
}

void EnemyMoveLR::NormalUpdate()
{
	//2�b�Ԃ��炢�~�܂�
	if (m_frame++ >= 120)
	{
		//����
		m_idx = 1;
		//�t���[���J�E���g���[���ɂ���
		m_frame = 0;

		m_updateFunc = &EnemyMoveLR::MoveUpdate;
	}
	//�~�܂��Ă���Ԃ͖ڂ�����摜��\��������
	else
	{
		m_idx = 0;
	}
}

void EnemyMoveLR::NormalDraw()
{
	if (!m_isExist)	return;
	int img = m_idx * kLeftRightSize;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		img, 0, kLeftRightSize, kLeftRightSize, kDrawScall * Game::kScale, 0.0f, m_handle, true, m_isLeft);
#ifdef _DEBUG
	m_rect.Draw(0xff00ff);
#endif
}

void EnemyMoveLR::BurstUpdate()
{
	m_idx++;
	if (m_idx == burst_frame_num * burst_frame_speed)
	{
		m_isExist = false;
	}
}

void EnemyMoveLR::BurstDraw()
{
	int imgX = (m_idx / burst_frame_speed) * burst_img_width;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		imgX, 0, burst_img_width, burst_img_height, burst_draw_scale * Game::kScale, 0.0f, m_burstHandle, true, false);
}

void EnemyMoveLR::MoveUpdate()
{
	//�ڂ��J���Ă���摜
	m_idx = 2;
	if (m_isLeft)
	{
		//���Ɉړ�����
		m_rect.center.x -= kEnemyMoveSpeed;
	}
	else
	{
		//�E�Ɉړ�
		m_rect.center.x += kEnemyMoveSpeed;
	}

	//�ǂɂԂ�������
	if (m_chipId == 1)
	{
		m_chipId = 0;
		//����
		m_idx = 1;
		//�������t��
		m_isLeft = !m_isLeft;
		//�ʏ�̍X�V�ɐ؂�ւ���
		m_updateFunc = &EnemyMoveLR::NormalUpdate;
	}
}
