#include "HealItem.h"
#include <DxLib.h>
#include "../Util/DrawFunctions.h"
#include "../game.h"
namespace
{
	constexpr int kHealSizeX = 32;
	constexpr int kHealSizeY = 32;

	constexpr float kHealScale = 1.0f;

	constexpr int kHeal = 2;
}

HealItem::HealItem(const Position2& pos, int handle) : ItemBase(pos)
{
	m_handle = handle;
	m_rect = { pos,{kHealSizeX,kHealSizeY} };
}

HealItem::~HealItem()
{

}

void HealItem::Update()
{
	if (m_addBlend > 100 )
	{
		add = -2;
	}
	else if(m_addBlend < 0)
	{
		add = 2;
	}
	m_addBlend += add;

	/*int w = m_rect.size.w / 2;
	int h = m_rect.size.h / 2;
	if (m_rect.center.x + w <= Game::kMapScreenLeftX || m_rect.center.x - w >= Game::kMapScreenRightX)
	{
		m_isExist = false;
	}
	else if (m_rect.center.y + h <= Game::kMapScreenTopY || m_rect.center.y - h <= Game::kMapScreenBottomY)
	{
		m_isExist = false;
	}*/
}

void HealItem::Draw(Vector2 vel)
{
	m_rect.center -= vel;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y), 0, 0, kHealSizeX, kHealSizeY,
		kHealScale, 0.0f, m_handle, true, false);

	SetDrawBlendMode(DX_BLENDMODE_ADD, m_addBlend);
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y), 0, 0, kHealSizeX, kHealSizeY,
		kHealScale, 0.0f, m_handle, true, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

#ifdef _DEBUG
	m_rect.Draw(0xaaffff);
	DrawFormatString(0, 800, 0x000000, L"center.x%3f,y%3f", m_rect.center.x, m_rect.center.y);
	DrawFormatString(0, 820, 0x000000, L"vel.x%3f,y%3f", vel.x, vel.y);
#endif
}

int HealItem::GetHeal() const
{
	return kHeal;
}
