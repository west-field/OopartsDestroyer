#include "ShotBase.h"
#include "DxLib.h"

ShotBase::ShotBase(int handle) :m_handle(handle), m_rect({}, {}), m_vel(), m_isExist(false), m_isLeft(false), m_isPlayerShot(false)
{
	int X, Y;
	GetGraphSize(m_handle, &X, &Y);
	m_rect.size = { X,Y };
}

ShotBase::~ShotBase()
{
	
}

const Rect& ShotBase::GetRect() const
{
	return m_rect;
}
