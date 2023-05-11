#include "ItemBase.h"
#include <DxLib.h>
#include "../game.h"
#include "../Util/DrawFunctions.h"


ItemBase::ItemBase(const Position2& pos) :
	m_handle(-1), m_rect(pos, {}),m_isExist(true),
	m_idx(0), m_itemType(ItemType::Point), m_animSpeed(0), m_animNum(0), m_drawScale(0)
{

}

ItemBase::~ItemBase()
{
	
}

void ItemBase::Update()
{
	m_idx = (m_idx + 1) % (m_animSpeed * m_animNum);

	//�\���͈͂���o�������
	int w = m_rect.size.w / 2;
	int h = m_rect.size.h / 2;
	if (m_rect.center.x + w < Game::kMapScreenLeftX || m_rect.center.x - w > Game::kMapScreenRightX)
	{
		m_isExist = false;
	}
}

//��`���擾����
const Rect& ItemBase::GetRect() const
{
	return m_rect;
}

//���݂��Ă��邩�ǂ���
bool ItemBase::IsExist() const
{
	return m_isExist;
}

//�擾�����㑶�݂�����
void ItemBase::EraseExist()
{
	m_isExist = false;
}

//�A�C�e�������ׂĎ擾����
ItemType ItemBase::OnGet()
{
	return m_itemType;
}


