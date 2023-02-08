#include "Player.h"
#include <DxLib.h>
#include "../game.h"

namespace
{
	constexpr int kSize = -5;
}

Player::Player(Position2 pos)
{
	m_rect.center = pos;
	m_rect.size = { Game::ChipSize+kSize ,Game::ChipSize+kSize };
}

Player::~Player()
{
}

void Player::Update()
{
	float chipSizeHalf = Game::ChipSize / 2;
	if (m_rect.center.x < chipSizeHalf)
	{
		m_rect.center.x = chipSizeHalf;
	}
	else if (m_rect.center.x > Game::kScreenWidth - chipSizeHalf)
	{
		m_rect.center.x = Game::kScreenWidth - chipSizeHalf;
	}
	if (m_rect.center.y < chipSizeHalf)
	{
		m_rect.center.y = chipSizeHalf;
	}
	else if (m_rect.center.y > Game::kScreenHeight - chipSizeHalf)
	{
		m_rect.center.y = Game::kScreenHeight - chipSizeHalf;
	}
}

void Player::Draw()
{
	//ç∂è„ç¿ïW
	float LeftX = m_rect.center.x - (m_rect.size.w / 2);
	float LeftY = m_rect.center.y - (m_rect.size.h / 2);

	DrawBoxAA(LeftX, LeftY, LeftX + m_rect.size.w, LeftY + m_rect.size.h, 0x00ffff, true);
}

void Player::Movement(Vector2 vec)
{
	m_rect.center += vec;
}

const Rect& Player::GetRect() const
{
	return m_rect;
}