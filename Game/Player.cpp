#include "Player.h"
#include <DxLib.h>
#include "../game.h"
#include "../Util/DrawFunctions.h"

namespace
{
	constexpr int kSize = 5;

	//プレイヤーグラフィック
	constexpr int kGraphSizeWidth = 16;		//サイズ
	constexpr int kGraphSizeHeight = 16;	//サイズ
	constexpr float kDrawScale = 2.0f;		//拡大率
	constexpr int kMoveFrameNum = 4;	//歩く時のアニメーション枚数
	constexpr int kFrameNum = 2;		//そのほかのアニメーション枚数
	constexpr int kFrameSpeed = 15;		//アニメーションスピード

}

Player::Player(Position2 pos)
{
	m_rect.center = pos;
	m_rect.size = { static_cast<int>(kGraphSizeWidth * kDrawScale - kSize) ,static_cast<int>(kGraphSizeHeight * kDrawScale - kSize)};
	m_handle = my::MyLoadGraph(L"Data/Retro-Lines-Player-transparent.png");
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

	if(m_frame++ > kFrameSpeed )
	{
		if (m_idxY == 1)
		{
			m_idxX = (m_idxX + 1) % (kMoveFrameNum);
		}
		else
		{
			m_idxX = (m_idxX + 1) % (kFrameNum);
		}
		m_frame = 0;
	}
}

void Player::Draw()
{
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		m_idxX  * kGraphSizeWidth, m_idxY * kGraphSizeHeight, kGraphSizeWidth, kGraphSizeHeight, kDrawScale, 0.0f, m_handle, true, m_isLeft);
#ifdef _DEBUG
	m_rect.Draw(0xaaffaa);

	DrawFormatString(0, 0, 0xffffff, L"%d", m_idxX);
	DrawFormatString(0, 20, 0xffffff, L"%d", m_idxY);
#endif
}

void Player::Movement(Vector2 vec)
{
	m_rect.center += vec;
}

const Rect& Player::GetRect() const
{
	return m_rect;
}

void Player::Action(ActionType type)
{
	switch (type)
	{
	case ActionType::grah_no:
		m_idxY = 0;
		break;
	case ActionType::grah_walk:
		m_idxY = 1;
		break;
	case ActionType::grah_idle:
		m_idxY = 2;
		break;
	case ActionType::grah_attack:
		m_idxY = 3;
		break;
	case ActionType::grah_jump:
		m_idxY = 4;
		break;
	case ActionType::grah_hit:
		m_idxY = 5;
		break;
	case ActionType::grah_max:
		m_idxY = 6;
		break;
	default:
		m_idxY = 2;
		break;
	}
}
