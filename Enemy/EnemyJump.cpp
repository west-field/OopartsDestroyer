#include "EnemyJump.h"
#include <DxLib.h>
#include "../Util/DrawFunctions.h"
#include "../Util/Sound.h"
#include "../Game/HpBar.h"
#include "../game.h"
#include "../Game/Player.h"
#include "../Game/ItemFactory.h"
#include "../Util/Sound.h"

namespace
{
	constexpr int kJumpTouchAttackPower = 4;//�ڐG�������̍U����

	//�G�l�~�[�A�j���[�V����
	constexpr int anim_frame_speed = 5;//�ꖇ�ɕK�v�ȃt���[����
	constexpr int anim_frame_num = 2;//�A�j���[�V��������
	constexpr int kRand = anim_frame_num * anim_frame_speed;
	constexpr int kJumpSize = 32;//�O���t�B�b�N1�̑傫��
	constexpr float kDrawScall = 1.0f;//�O���t�B�b�N�g�嗦
	constexpr float kEnemyMoveSpeed = -4.0f;//�G�l�~�[�̈ړ����x
	constexpr float kGrap = 2.0f;//����
	constexpr float kJumpA = 5.0f;//�W�����v��

	//�����A�j���[�V����
	constexpr int burst_img_width = 32;//�摜�T�C�YX48
	constexpr int burst_img_height = 32;//�摜�T�C�YY
	constexpr float burst_draw_scale = 1.0f;//�g�嗦
	constexpr int burst_frame_num = 8;//�A�j���[�V��������
	constexpr int burst_frame_speed = 5;//�A�j���[�V�����X�s�[�h
}

EnemyJump::EnemyJump(std::shared_ptr<Player> player, const Position2 pos, int handle, int burstH, std::shared_ptr<ShotFactory> sFactory, std::shared_ptr<ItemFactory> itFactory):
	EnemyBase(player,pos,sFactory,itFactory),m_updateFunc(&EnemyJump::NormalUpdate),m_drawFunc(&EnemyJump::NormalDraw)
{
	m_handle = handle;//�G�n���h��
	m_burstHandle = burstH;//�����n���h��
	m_rect = { pos,{static_cast<int>(kJumpSize * Game::kScale * kDrawScall),static_cast<int>(kJumpSize * Game::kScale * kDrawScall)} };
	m_hp->MaxHp(1);
	m_frame = GetRand(kRand);//�W�����v����܂ł̎��Ԃ������_���Ō��߂�
}

EnemyJump::~EnemyJump()
{

}

void EnemyJump::Update()
{
	(this->*m_updateFunc)();
}

void EnemyJump::Draw()
{
	(this->*m_drawFunc)();
}

int EnemyJump::TouchAttackPower() const
{
	return kJumpTouchAttackPower;
}

void EnemyJump::Damage(int damage)
{
	m_hp->Damage(damage);

	if (m_hp->GetHp() == 0)
	{
		SoundManager::GetInstance().Play(SoundId::EnemyBurst);
		m_updateFunc = &EnemyJump::BurstUpdate;
		m_drawFunc = &EnemyJump::BurstDraw;
		m_idx = 0;
		if (GetRand(100) % 3 == 0)
		{
			m_itemFactory->Create(ItemType::Heal, m_rect.center);//�񕜃A�C�e��
		}
		return;
	}
	SoundManager::GetInstance().Play(SoundId::EnemyHit);
}

bool EnemyJump::IsCollidable() const
{
	//BurstUpdate�̎��͓�����Ȃ�
	return (m_updateFunc != &EnemyJump::BurstUpdate);
}

void EnemyJump::NormalUpdate()
{
	//�����_���Ȏ��ɃW�����v������
	if (m_frame-- <= 0)
	{
		m_frame = GetRand(kRand) * anim_frame_speed + 60;//���W�����v����܂ł̎���
		m_idx = 1;
		SoundManager::GetInstance().Play(SoundId::EnemyJump);
		m_posTemp =  m_rect.center.y - Game::kDrawSize * 3;//�O�u���b�N���炢�W�����v����

		//�v���C���[�̂�������Ɉړ�����
		if (m_player->GetRect().GetCenter().x > m_rect.center.x)
		{
			m_vec = { 1.0f,-kJumpA };
		}
		else
		{
			m_vec = { -1.0f,-kJumpA };
		}

		m_updateFunc = &EnemyJump::JumpUpdate;
		return;
	}
}

void EnemyJump::NormalDraw()
{
	int img = m_idx * kJumpSize;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		img, 0, kJumpSize, kJumpSize, kDrawScall * Game::kScale, 0.0f, m_handle, true, m_isLeft);
#ifdef _DEBUG
	m_rect.Draw(0xff0000);
#endif
}

void EnemyJump::BurstUpdate()
{
	m_idx++;
	if (m_idx == burst_frame_num * burst_frame_speed)
	{
		m_isExist = false;
	}
}

void EnemyJump::BurstDraw()
{
	int imgX = (m_idx / burst_frame_speed) * burst_img_width;

	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		imgX, 0, burst_img_width, burst_img_height, burst_draw_scale * Game::kScale, 0.0f, m_burstHandle, true,false);
}

void EnemyJump::JumpUpdate()
{
	m_rect.center += m_vec;

	if (m_rect.center.y <= m_posTemp)
	{
		m_vec.y = kGrap;
		m_updateFunc = &EnemyJump::DownUpdate;
		return;
	}
}

void EnemyJump::DownUpdate()
{
	m_rect.center += m_vec;
	//�����蔻��̂���u���b�N�ɓ���������~�߂�
	if (m_chipId == 1)
	{
		m_chipId = 0;
		m_idx = 0;
		m_rect.center.y -= kGrap;
		m_updateFunc = &EnemyJump::NormalUpdate;
		return;
	}
}