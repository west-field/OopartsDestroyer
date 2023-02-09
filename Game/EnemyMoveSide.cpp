#include "EnemyMoveSide.h"
#include <DxLib.h>
#include "../Util/DrawFunctions.h"
#include "../game.h"

namespace
{
	constexpr int anim_frame_speed = 5;//一枚に必要なフレーム数
	constexpr int anim_frame_num = 5;//アニメーション枚数
	constexpr int kSize = 29;
	constexpr float kDrawScall = 0.8f;
}

MoveSide::MoveSide(std::shared_ptr<Player>player, const Position2 pos, int handle):
	EnemyBase(player,pos),m_handle(handle)
{
	m_rect = { pos, { static_cast<int>(kSize* kDrawScall),static_cast<int>(kSize* kDrawScall) } };
}

MoveSide::~MoveSide()
{

}

void MoveSide::Update()
{
	if (m_rect.center.x < -kSize-20)
	{
		m_isExist = false;
	}
	else if (m_rect.center.x > Game::kScreenWidth + kSize+20)
	{
		m_isExist = false;
	}
}

void MoveSide::Draw()
{
	int imgX = (m_idx / anim_frame_speed) * 29;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		imgX, 0, kSize, kSize, kDrawScall, 0.0f, m_handle, true, m_isLeft);
#ifdef _DEBUG
	m_rect.Draw(0xff0000);
#endif
}

void MoveSide::Movement(Vector2 vec)
{
	if (!m_isExist) return;
	if (m_isLeft) vec *= 1.0f;
	m_rect.center += vec;

	m_idx = (m_idx + 1) % (anim_frame_speed * anim_frame_num);
}

void MoveSide::Damage(int damage)
{
	m_isExist = false;
}
