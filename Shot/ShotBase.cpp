#include "ShotBase.h"
#include "DxLib.h"

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

void ShotBase::Movement(Vector2 vec)
{
	if (m_isLeft) vec *= -1.0f;
	m_rect.center += vec;
}

const Rect& ShotBase::GetRect() const
{
	return m_rect;
}

const int ShotBase::AttackPower() const
{
	return m_hitDamagePower;
}
