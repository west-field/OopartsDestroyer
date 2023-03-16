#include "ItemBase.h"
#include <DxLib.h>

ItemBase::ItemBase(const Position2& pos)
{
	m_rect.center = pos;
}

ItemBase::~ItemBase()
{
	
}

const Rect& ItemBase::GetRect() const
{
	return m_rect;
}

bool ItemBase::IsExist() const
{
	return m_isExist;
}

void ItemBase::SetExist(bool isExist)
{
	m_isExist = isExist;
}
