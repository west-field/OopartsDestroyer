#include "ShotBase.h"
#include "DxLib.h"
#include "../game.h"

ShotBase::ShotBase(int handle) :
	m_handle(handle), m_rect({}, {}), m_vel(), m_isExist(false), 
	m_isLeft(false), m_isPlayerShot(false), m_hitDamagePower(0)
{
	int X, Y;
	GetGraphSize(m_handle, &X, &Y);
	m_rect.size = { X,Y };
}

ShotBase::~ShotBase()
{
	
}

void ShotBase::Start(Position2 pos, Vector2 vel, bool left, bool isPlayer)
{
	m_isExist = true;
	m_isLeft = left;
	m_rect.center = pos;
	m_vel = vel;
	m_isPlayerShot = isPlayer;
}

void ShotBase::Update()
{
	int wsize = m_rect.size.w / 2;
	int hsize = m_rect.size.h / 2;
	//左側
	if (m_rect.center.x - wsize < Game::kMapScreenLeftX)
	{
		m_isExist = false;
	}
	//右側
	if (m_rect.center.x + wsize > Game::kMapScreenRightX)
	{
		m_isExist = false;
	}
	//上端
	if (m_rect.center.y + hsize < Game::kMapScreenTopY)
	{
		m_isExist = false;
	}
	//下端
	if (m_rect.center.y - hsize > Game::kMapScreenBottomY)
	{
		m_isExist = false;
	}
}

//移動させる
void ShotBase::Movement(Vector2 vec)
{
	//左を向いていたら、逆方向にする
	if (m_isLeft) vec *= -1.0f;
	m_rect.center += vec;
}

//矩形を取得
const Rect& ShotBase::GetRect() const
{
	return m_rect;
}

//攻撃力を取得する
const int ShotBase::AttackPower() const
{
	return m_hitDamagePower;
}
