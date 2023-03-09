#include "EnemyMoveLeft.h"
#include <DxLib.h>
#include "../Util/DrawFunctions.h"
#include "../game.h"
#include "../Game/HpBar.h"
#include "../Game/Player.h"

namespace
{
	constexpr int kUpDownTouchAttackPower = 4;//�ڐG�������̍U����

	//constexpr int anim_frame_speed = 5;//�ꖇ�ɕK�v�ȃt���[����
	//constexpr int anim_frame_num = 5;//�A�j���[�V��������
	//constexpr int kSize = 29;
	//constexpr float kDrawScall = 1.0f;
	constexpr float kEnemyMoveSpeed = -4.0f;//�G�l�~�[�̈ړ����x

	constexpr int kSize = 96;//�摜�T�C�YX
	constexpr float kDrawScall = 0.3f;//�g�嗦
	constexpr int anim_frame_num = 4;//�A�j���[�V��������
	constexpr int anim_frame_speed = 20;//�A�j���[�V�����X�s�[�h

	constexpr int burst_img_width = 32;//�摜�T�C�YX
	constexpr int burst_img_height = 32;//�摜�T�C�YY
	constexpr float burst_draw_scale = 1.0f;//�g�嗦
	constexpr int burst_frame_num = 8;//�A�j���[�V��������
	constexpr int burst_frame_speed = 5;//�A�j���[�V�����X�s�[�h
	
}

EnemyMoveLeft::EnemyMoveLeft(std::shared_ptr<Player>player, const Position2 pos, int handle, int burstH, std::shared_ptr<ShotFactory> sFactory):
	EnemyBase(player,pos,sFactory),m_updateFunc(&EnemyMoveLeft::NormalUpdate),m_drawFunc(&EnemyMoveLeft::NormalDraw)
{
	m_idx = 0;
	m_handle = handle;
	m_burstHandle = burstH;
	m_rect = { pos, { static_cast<int>(kSize* kDrawScall),static_cast<int>(kSize* kDrawScall) } };
	
	m_hp->MaxHp(1);
}

EnemyMoveLeft::~EnemyMoveLeft()
{

}

void EnemyMoveLeft::Update()
{
	//���݂��Ă��Ȃ��Ƃ��͍X�V���Ȃ�
	if (!m_isExist) return;
	(this->*m_updateFunc)();
}

void EnemyMoveLeft::Draw()
{
	//���݂��Ă��Ȃ��Ƃ��͕\�����Ȃ�
	if (!m_isExist) return;
	(this->*m_drawFunc)();
}

void EnemyMoveLeft::Movement(Vector2 vec)
{
	if (!m_isExist) return;
	if (m_isLeft) vec *= 1.0f;

	m_rect.center += vec;
}

int EnemyMoveLeft::TouchAttackPower() const
{
	return kUpDownTouchAttackPower;
}

void EnemyMoveLeft::Damage(int damage)
{
	m_hp->Damage(damage);
	//m_ultimateTimer = kUltimateFrame;//���G����
	if (m_hp->GetHp() == 0)
	{
		m_updateFunc = &EnemyMoveLeft::BurstUpdate;
		m_drawFunc = &EnemyMoveLeft::BurstDraw;
		m_idx = 0;
	}
}

bool EnemyMoveLeft::IsCollidable() const
{
	//�����A�j���[�V�����łȂ��Ƃ��͓�����
	return (m_updateFunc != &EnemyMoveLeft::BurstUpdate);
}

void EnemyMoveLeft::NormalUpdate()
{
	m_rect.center.x += kEnemyMoveSpeed;
	m_idx = (m_idx + 1) % (anim_frame_speed * anim_frame_num);
}

void EnemyMoveLeft::NormalDraw()
{
	int imgX = (m_idx / anim_frame_speed) * kSize;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		imgX, 0, kSize, kSize, kDrawScall, 0.0f, m_handle, true, m_isLeft);
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
		imgX, 0, burst_img_width, burst_img_height, burst_draw_scale, 0.0f, m_burstHandle, true, false);
}
