#include "ShotBase.h"
#include "DxLib.h"

namespace
{
	constexpr float kShotSpeed = 8.0f;
}

ShotBase::ShotBase(int handle) :m_handle(handle), m_rect({}, {}), m_vel(), m_isExist(false), m_isLeft(false), m_isPlayerShot(false)
{
	int X, Y;
	GetGraphSize(m_handle, &X, &Y);
	m_rect.size = { X,Y };
}

ShotBase::~ShotBase()
{
	
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
