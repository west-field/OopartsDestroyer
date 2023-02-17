#include "EnemyJump.h"
#include <DxLib.h>
#include "../Util/DrawFunctions.h"
#include "../Game/HpBar.h"

namespace
{
	constexpr int kJumpTouchAttackPower = 4;//�ڐG�������̍U����

	constexpr int anim_frame_speed = 5;//�ꖇ�ɕK�v�ȃt���[����
	constexpr int anim_frame_num = 2;//�A�j���[�V��������

	constexpr int kJumpSize = 32;//�O���t�B�b�N1�̑傫��
	constexpr float kDrawScall = 1.0f;//�O���t�B�b�N�g�嗦

	constexpr float kEnemyMoveSpeed = -4.0f;//�G�l�~�[�̈ړ����x
}

EnemyJump::EnemyJump(std::shared_ptr<Player> player, const Position2 pos, int handle, std::shared_ptr<ShotFactory> sFactory):
	EnemyBase(player,pos,sFactory),m_handle(handle)
{
	m_hp->MaxHp(1);
}

EnemyJump::~EnemyJump()
{
}

void EnemyJump::Update()
{
	if (!m_isExist)	return;
	if (m_frame-- == 0)
	{
		m_idx = 1;
		m_frame = GetRand(anim_frame_speed * anim_frame_num);
	}
}

void EnemyJump::Draw()
{
	if (!m_isExist)	return;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		m_idx, 0, kJumpSize, kJumpSize, kDrawScall, 0.0f, m_handle, true, m_isLeft);
#ifdef _DEBUG
	m_rect.Draw(0xff0000);
#endif
}

void EnemyJump::Movement(Vector2 vec)
{
	if (!m_isExist)	return;

}

int EnemyJump::TouchAttackPower() const
{
	return kJumpTouchAttackPower;
}