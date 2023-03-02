#include "EnemyMoveUpDown.h"
#include <DxLib.h>
#include "../Util/DrawFunctions.h"
#include "../game.h"
#include "../Game/HpBar.h"
#include "../Game/Player.h"

namespace
{
	constexpr int kUpDownTouchAttackPower = 4;//�ڐG�������̍U����

	constexpr int anim_frame_speed = 5;//�ꖇ�ɕK�v�ȃt���[����
	constexpr int anim_frame_num = 5;//�A�j���[�V��������
	constexpr int kSize = 29;
	constexpr float kDrawScall = 1.0f;
	constexpr float kEnemyMoveSpeed = -4.0f;//�G�l�~�[�̈ړ����x

	constexpr int burst_img_width = 32;//�摜�T�C�YX
	constexpr int burst_img_height = 32;//�摜�T�C�YY
	constexpr float burst_draw_scale = 1.0f;//�g�嗦
	constexpr int burst_frame_num = 8;//�A�j���[�V��������
	constexpr int burst_frame_speed = 5;//�A�j���[�V�����X�s�[�h
}

EnemyMoveUpDown::EnemyMoveUpDown(std::shared_ptr<Player>player, const Position2 pos, int handle, int burstH, std::shared_ptr<ShotFactory> sFactory):
	EnemyBase(player,pos,sFactory),m_updateFunc(&EnemyMoveUpDown::NormalUpdate),m_drawFunc(&EnemyMoveUpDown::NormalDraw)
{
	m_handle = handle;
	m_burstHandle = burstH;
	m_rect = { pos, { static_cast<int>(kSize* kDrawScall),static_cast<int>(kSize* kDrawScall) } };
	
	m_hp->MaxHp(1);
}

EnemyMoveUpDown::~EnemyMoveUpDown()
{

}

void EnemyMoveUpDown::Update()
{
	//���݂��Ă��Ȃ��Ƃ��͍X�V���Ȃ�
	if (!m_isExist) return;
	(this->*m_updateFunc)();
}

void EnemyMoveUpDown::Draw()
{
	//���݂��Ă��Ȃ��Ƃ��͕\�����Ȃ�
	if (!m_isExist) return;
	(this->*m_drawFunc)();
}

void EnemyMoveUpDown::Movement(Vector2 vec)
{
	if (!m_isExist) return;
	if (m_isLeft) vec *= 1.0f;

	m_rect.center += vec;
}

int EnemyMoveUpDown::TouchAttackPower() const
{
	return kUpDownTouchAttackPower;
}

void EnemyMoveUpDown::Damage(int damage)
{
	m_hp->Damage(damage);
	//m_ultimateTimer = kUltimateFrame;//���G����
	if (m_hp->GetHp() == 0)
	{
		m_updateFunc = &EnemyMoveUpDown::BurstUpdate;
		m_drawFunc = &EnemyMoveUpDown::BurstDraw;
		m_idx = 0;
	}
}

bool EnemyMoveUpDown::IsCollidable() const
{
	//�����A�j���[�V�����łȂ��Ƃ��͓�����
	return (m_updateFunc != &EnemyMoveUpDown::BurstUpdate);
}

void EnemyMoveUpDown::NormalUpdate()
{
	m_rect.center.x += kEnemyMoveSpeed;
	m_idx = (m_idx + 1) % (anim_frame_speed * anim_frame_num);
}

void EnemyMoveUpDown::NormalDraw()
{
	int imgX = (m_idx / anim_frame_speed) * kSize;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		imgX, 0, kSize, kSize, kDrawScall, 0.0f, m_handle, true, m_isLeft);
#ifdef _DEBUG
	m_rect.Draw(0xff0000);
#endif
}

void EnemyMoveUpDown::BurstUpdate()
{
	m_idx++;
	if (m_idx == burst_frame_num * burst_frame_speed)
	{
		m_isExist = false;
	}
}

void EnemyMoveUpDown::BurstDraw()
{
	int imgX = (m_idx / burst_frame_speed) * burst_img_width;

	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		imgX, 0, burst_img_width, burst_img_height, burst_draw_scale, 0.0f, m_burstHandle, true, false);
}
