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
	//ç∂ë§
	if (m_rect.center.x - wsize < Game::kMapScreenLeftX)
	{
		m_isExist = false;
	}
	//âEë§
	if (m_rect.center.x + wsize > Game::kMapScreenRightX)
	{
		m_isExist = false;
	}
	//è„í[
	if (m_rect.center.y + hsize < Game::kMapScreenTopY)
	{
		m_isExist = false;
	}
	//â∫í[
	if (m_rect.center.y - hsize > Game::kMapScreenBottomY)
	{
		m_isExist = false;
	}
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
