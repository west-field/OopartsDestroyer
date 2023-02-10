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

	constexpr int ultimate_frame = 180;//無敵時間

}

Player::Player(Position2 pos):updateFunc_(&Player::NormalUpdate),drawFunc_(&Player::NormalDraw)
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
	(this->*updateFunc_)();
}

void Player::Draw()
{
	(this->*drawFunc_)();
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

bool Player::isCollidable() const
{
	return (updateFunc_ == &Player::NormalUpdate) && m_ultimateTimer == 0;
}

void Player::NormalUpdate()
{
	float chipSizeHalf = Game::ChipSize / 2;
	//画面の左端についたらそれ以上動けないようにする
	if (m_rect.center.x < Game::kMapScreenLeftX + chipSizeHalf)
	{
		m_rect.center.x = Game::kMapScreenLeftX + chipSizeHalf;
	}
	//画面の右端についたらそれ以上動けないようにする
	else if (m_rect.center.x > Game::kMapScreenRightX - chipSizeHalf)
	{
		m_rect.center.x = Game::kMapScreenRightX - chipSizeHalf;
	}
	//画面の上端についたらそれ以上動けないように
	if (m_rect.center.y < Game::kMapScreenTopY + chipSizeHalf)
	{
		m_rect.center.y = Game::kMapScreenTopY + chipSizeHalf;
	}
	//画面の下端についたらそれ以上動けないように
	else if (m_rect.center.y > Game::kMapScreenBottomY - chipSizeHalf)
	{
		m_rect.center.y = Game::kMapScreenBottomY - chipSizeHalf;
	}

	if (m_frame++ > kFrameSpeed)
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

	//どちらか大きいほうを返す
	//m_ultimateTimer = std::max(--m_ultimateTimer, 0);
}

void Player::NormalDraw()
{
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		m_idxX * kGraphSizeWidth, m_idxY * kGraphSizeHeight, kGraphSizeWidth, kGraphSizeHeight, kDrawScale, 0.0f, m_handle, true, m_isLeft);
#ifdef _DEBUG
	m_rect.Draw(0xaaffaa);

	DrawFormatString(0, 0, 0xffffff, L"%d", m_idxX);
	DrawFormatString(0, 20, 0xffffff, L"%d", m_idxY);
#endif
}

void Player::BurstUpdate()
{
}

void Player::BurstDraw()
{
}
