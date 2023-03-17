#include "EnemyMoveLeft.h"
#include <DxLib.h>
#include "../Util/DrawFunctions.h"
#include "../game.h"
#include "../Game/HpBar.h"
#include "../Game/Player.h"
#include "../Game/ItemFactory.h"
#include "../Util/Sound.h"

namespace
{
	constexpr int kUpDownTouchAttackPower = 4;//�ڐG�������̍U����
	
	//�G�l�~�[�A�j���[�V����
	constexpr float kEnemyMoveSpeed = -4.0f;//�G�l�~�[�̈ړ����x
	constexpr int kSize = 96;//�摜�T�C�YX
	constexpr float kDrawScall = 0.3f;//�g�嗦
	constexpr int anim_frame_num = 4;//�A�j���[�V��������
	constexpr int anim_frame_speed = 20;//�A�j���[�V�����X�s�[�h

	//�����A�j���[�V����
	constexpr int burst_img_width = 32;//�摜�T�C�YX
	constexpr int burst_img_height = 32;//�摜�T�C�YY
	constexpr float burst_draw_scale = 1.0f;//�g�嗦
	constexpr int burst_frame_num = 8;//�A�j���[�V��������
	constexpr int burst_frame_speed = 5;//�A�j���[�V�����X�s�[�h
	
}

EnemyMoveLeft::EnemyMoveLeft(std::shared_ptr<Player>player, const Position2 pos, int handle, int burstH, std::shared_ptr<ShotFactory> sFactory, std::shared_ptr<ItemFactory> itFactory):
	EnemyBase(player,pos,sFactory,itFactory),m_updateFunc(&EnemyMoveLeft::NormalUpdate),m_drawFunc(&EnemyMoveLeft::NormalDraw)
{
	m_idx = 0;
	m_handle = handle;
	m_burstHandle = burstH;
	m_rect = { pos, { static_cast<int>(kSize * Game::kScale * kDrawScall),static_cast<int>(kSize * Game::kScale * kDrawScall) } };
	
	m_hp->MaxHp(1);
}

EnemyMoveLeft::~EnemyMoveLeft()
{

}

void EnemyMoveLeft::Update()
{
	(this->*m_updateFunc)();
}

void EnemyMoveLeft::Draw()
{
	(this->*m_drawFunc)();
}

int EnemyMoveLeft::TouchAttackPower() const
{
	return kUpDownTouchAttackPower;
}

void EnemyMoveLeft::Damage(int damage)
{
	m_hp->Damage(damage);

	if (m_hp->GetHp() == 0)
	{
		SoundManager::GetInstance().Play(SoundId::EnemyBurst);
		m_updateFunc = &EnemyMoveLeft::BurstUpdate;
		m_drawFunc = &EnemyMoveLeft::BurstDraw;
		m_idx = 0;
		if (GetRand(100) % 3 == 0)
		{
			m_itemFactory->Create(ItemType::Heal, m_rect.center);//�񕜃A�C�e��
		}
		return;
	}
	SoundManager::GetInstance().Play(SoundId::EnemyHit);
}

bool EnemyMoveLeft::IsCollidable() const
{
	//BurstUpdate�̎��͓�����Ȃ�
	return (m_updateFunc != &EnemyMoveLeft::BurstUpdate);
}

void EnemyMoveLeft::NormalUpdate()
{
	Movement({ kEnemyMoveSpeed ,0.0f});//�G�l�~�[�ړ�
	m_idx = (m_idx + 1) % (anim_frame_speed * anim_frame_num);
}

void EnemyMoveLeft::NormalDraw()
{
	int imgX = (m_idx / anim_frame_speed) * kSize;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		imgX, 0, kSize, kSize, kDrawScall * Game::kScale, 0.0f, m_handle, true, m_isLeft);
#ifdef _DEBUG
	m_rect.Draw(0xff0000);
#endif
}

void EnemyMoveLeft::BurstUpdate()
{
	m_idx++;
	if (m_idx == burst_frame_num * burst_frame_speed)
	{
		m_isExist = false;
	}
}

void EnemyMoveLeft::BurstDraw()
{
	int imgX = (m_idx / burst_frame_speed) * burst_img_width;

	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		imgX, 0, burst_img_width, burst_img_height, burst_draw_scale * Game::kScale, 0.0f, m_burstHandle, true, false);
}
