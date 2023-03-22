#include "Player.h"
#include <DxLib.h>
#include "../game.h"
#include "../Util/DrawFunctions.h"
#include "HpBar.h"

namespace
{
	constexpr int kSize = 5;//当たり判定を小さくする

	//プレイヤーグラフィック
	constexpr int kGraphSizeWidth = 32;		//サイズ
	constexpr int kGraphSizeHeight = 32;	//サイズ
	constexpr float kDrawScale = 2.0f;		//拡大率
	constexpr int kFrameSpeed = 10;		//アニメーションスピード

	constexpr int kUltimateFrame = 120;//無敵時間 2秒

	//回復エフェクト
	constexpr int kHealImgWidth = 100;//画像サイズX
	constexpr int kHealImgHeight = 100;//画像サイズY
	constexpr float kHealDrawScale = 1.5f;//拡大率
	constexpr int kHealAnimNum = 61;//アニメーション枚数
	constexpr int kHealAnimSpeed = 1;//アニメーションスピード
}

Player::Player(Position2 pos, std::shared_ptr<HpBar>hp):m_hp(hp)
{
	m_drawScale = kDrawScale;
	m_rect.center = pos;
	m_rect.size = { static_cast<int>(kGraphSizeWidth*Game::kScale - kSize) ,static_cast<int>(kGraphSizeHeight * Game::kScale - kSize)};
	m_handle = my::MyLoadGraph(L"Data/player.png");
	m_healH = my::MyLoadGraph(L"Data/heal.png");
}

Player::~Player()
{
	DeleteGraph(m_handle);
	DeleteGraph(m_healH);
}

void Player::Update()
{
	float chipSizeHalf = Game::kDrawSize / 2;
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
		switch (m_idxY)
		{
		case static_cast<int>(ActionType::grah_idle):
			m_idxX = (m_idxX + 1) % (1);
			break;
		case static_cast<int>(ActionType::grah_walk):
			m_idxX = (m_idxX + 1) % (2);
			break;
		case  static_cast<int>(ActionType::grah_jump):
		case  static_cast<int>(ActionType::grah_attack):
			m_idxX = (m_idxX + 1) % (4);
			break;
		case  static_cast<int>(ActionType::grah_death):
			m_idxX = (m_idxX + 1) % (6);
			if (m_idxX == 0)
			{
				m_isExist = false;
			}
			break;
		}
		if (m_idxX == 0)
		{
			m_idxY = 0;
		}

		m_frame = 0;
	}

	//0よりも小さかったら0を返す
	if (--m_ultimateTimer <= 0)
	{
		m_ultimateTimer = 0;
	}

	if (m_isHeal)
	{
		m_healIdx += 1;
		if (m_healIdx == (kHealAnimNum ) * kHealAnimSpeed)
		{
			m_isHeal = false;
			m_healIdx = 0;
		}
	}
}

void Player::Draw()
{
	if ((m_ultimateTimer / 10) % 2 == 1)
	{
		return;
	}
	//プレイヤーを表示
	my::MyDrawRectRotaGraph(m_rect.center.x, m_rect.center.y - kGraphSizeHeight / 2,
		m_idxX * kGraphSizeWidth, m_idxY * kGraphSizeHeight, kGraphSizeWidth, kGraphSizeHeight, m_drawScale * Game::kScale, 0.0f, m_handle, true, m_isLeft);
#ifdef _DEBUG
	m_rect.Draw(0xaaffaa);

	DrawFormatString(0, 0, 0xaaaaaa, L"X%d", m_idxX);
	DrawFormatString(0, 20, 0xaaaaaa, L"Y%d", m_idxY);
#endif
	if (m_isHeal)
	{
		int animNum = (m_healIdx / kHealAnimSpeed);
		int imgX = animNum % 6 * kHealImgWidth;
		int imgY = animNum / 6 * kHealImgHeight;
		my::MyDrawRectRotaGraph(m_rect.center.x, m_rect.center.y - kGraphSizeHeight / 2,
			imgX, imgY,kHealImgWidth, kHealImgHeight, kHealDrawScale,
			0.0f, m_healH, true, false);
	}
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
	case ActionType::grah_idle:
		m_idxY = static_cast<int>(ActionType::grah_idle);
		break;
	case ActionType::grah_walk:
		if (m_idxX == 0 && m_idxY == 0) m_idxY = static_cast<int>(ActionType::grah_walk);
		break;
	case ActionType::grah_jump:
		m_idxY = static_cast<int>(ActionType::grah_jump);
		break;
	case ActionType::grah_attack:
		m_idxY = static_cast<int>(ActionType::grah_attack);
		m_idxX = 0;
		break;
	case ActionType::grah_death:
		m_idxY = static_cast<int>(ActionType::grah_death);
		break;
	default:
		m_idxY = 0;
		break;
	}
	
}

bool Player::IsCollidable() const
{
	return m_ultimateTimer == 0;
}

void Player::Damage(int damage)
{
	m_ultimateTimer = kUltimateFrame;
	m_hp->Damage(damage);
}

void Player::Heal(int Heal)
{
	m_isHeal = true;
	m_hp->Heal(Heal);
}

void Player::ScaleEnlarge(float scale)
{
	if (m_drawScale >= 4.0f)	return;
	if(m_ultimateTimer < 0) m_ultimateTimer = 0;
	m_drawScale += scale;
}

