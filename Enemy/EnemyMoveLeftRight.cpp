#include "EnemyMoveLeftRight.h"

#include "../Util/DrawFunctions.h"
#include "../Game/HpBar.h"

namespace
{
	constexpr int kLeftRightTouchAttackPower = 4;//�ڐG�������̍U����

	constexpr int anim_frame_speed = 5;//�ꖇ�ɕK�v�ȃt���[����
	constexpr int anim_frame_num = 5;//�A�j���[�V��������

	constexpr int kLeftRightSize = 32;//�O���t�B�b�N1�̑傫��
	constexpr float kDrawScall = 1.0f;//�O���t�B�b�N�g�嗦

	constexpr float kEnemyMoveSpeed = -4.0f;//�G�l�~�[�̈ړ����x
}

EnemyMoveLeftRight::EnemyMoveLeftRight(std::shared_ptr<Player> player, const Position2 pos, int handle, std::shared_ptr<ShotFactory> sFactory) :
	EnemyBase(player, pos, sFactory), m_handle(handle)
{
	m_hp->MaxHp(5);
}

EnemyMoveLeftRight::~EnemyMoveLeftRight()
{
}

void EnemyMoveLeftRight::Update()
{
	if (!m_isExist)	return;

}

void EnemyMoveLeftRight::Draw()
{
	if (!m_isExist)	return;
	int img = (m_idx / anim_frame_speed) * kLeftRightSize;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		img, 0, kLeftRightSize, kLeftRightSize, kDrawScall, 0.0f, m_handle, true, m_isLeft);
#ifdef _DEBUG
	m_rect.Draw(0xff0000);
#endif
}

void EnemyMoveLeftRight::Movement(Vector2 vec)
{
	if (!m_isExist)	return;
	if (m_isLeft) vec *= -1.0f;

	m_rect.center += vec;
	m_rect.center.x += kEnemyMoveSpeed;
	m_idx = (m_idx + 1) % (anim_frame_speed * anim_frame_num);
}

int EnemyMoveLeftRight::TouchAttackPower() const
{
	return kLeftRightTouchAttackPower;
}
