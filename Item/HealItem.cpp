#include "HealItem.h"
#include <DxLib.h>
#include "../Util/DrawFunctions.h"
#include "../game.h"
namespace
{
	constexpr int kHealSizeX = 32;//�����蔻��T�C�Y
	constexpr int kHealSizeY = 32;//�����蔻��T�C�Y

	constexpr float kHealScale = 1.5f;//�摜�g�嗦

	constexpr int kHealCardAnimNum = 8;//�A�j���[�V��������
	constexpr int kHealCardAnimSpeed = 10;//�A�j���[�V�������x
	constexpr int kHealCardSizeX = 192 / kHealCardAnimNum;//�摜�T�C�Y
	constexpr int kHealCardSizeY = 24;//�摜�T�C�Y

	constexpr int kHeal = 2;//�񕜗�
}

HealItem::HealItem(const Position2& pos, int handle) : ItemBase(pos), m_idx(0)
{
	m_handle = handle;
	m_rect = { pos,{kHealSizeX,kHealSizeY} };
}

HealItem::~HealItem()
{

}

void HealItem::Update()
{
	m_idx = (m_idx + 1) % (kHealCardAnimSpeed * kHealCardAnimNum);

	//�\���͈͂���o�������
	int w = m_rect.size.w / 2;
	int h = m_rect.size.h / 2;
	if (m_rect.center.x + w < Game::kMapScreenLeftX || m_rect.center.x - w > Game::kMapScreenRightX)
	{
		m_isExist = false;
	}
}

void HealItem::Draw(Vector2 vel)
{
	m_rect.center -= vel;

	int img = (m_idx / kHealCardAnimSpeed) * kHealCardSizeX;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y), img, 0,
		kHealCardSizeX, kHealCardSizeY,kHealScale, 0.0f, m_handle, true, false);

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
