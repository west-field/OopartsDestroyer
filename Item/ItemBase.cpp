#include "ItemBase.h"
#include <DxLib.h>

ItemBase::ItemBase(const Position2& pos) :m_handle(-1), m_rect(pos, {}),m_isExist(true)
{

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

void ItemBase::EraseExist()
{
	m_isExist = false;
}
