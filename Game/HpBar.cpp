#include "HpBar.h"
#include <DxLib.h>
#include "../game.h"
#include "../Util/DrawFunctions.h"

namespace
{
	//ägëÂó¶
	constexpr float kScale = 4.0f;
}

HpBar::HpBar():m_MaxHp(0),m_Hp(0),m_HpHandle(-1), m_isHpGraph()
{
	for (auto& hp : m_isHpGraph)
	{
		hp = true;
	}
	m_rect = { { static_cast<float>(Game::kMapScreenLeftX - 100) ,Game::kScreenHeight / 3} ,{} };
}

HpBar::~HpBar()
{
	DeleteGraph(m_HpHandle);
}

void HpBar::Init(int handle)
{
	m_HpHandle = handle;

	GetGraphSize(m_HpHandle, &m_rect.size.w, &m_rect.size.h);

	m_rect.size.w /= 5;
	m_rect.size.h /= 2;

	m_MaxHp = m_Hp = kHpMax;
}

void HpBar::MaxHp(int maxHp)
{
	m_MaxHp = m_Hp = maxHp;
}

void HpBar::Update()
{
	idxX = m_Hp / 10;
	idxY = idxX / 5;

	switch (m_Hp)
	{
	case 20:
	case 19:
		idxX = 0;
		idxY = 0;
		break;
	case 18:
	case 17:
		idxX = 1;
		idxY = 0;
		break;
	case 16:
	case 15:
		idxX = 2;
		idxY = 0;
		break;
	case 14:
	case 13:
		idxX = 3;
		idxY = 0;
		break;
	case 12:
	case 11:
		idxX = 4;
		idxY = 0;
		break;
	case 10:
	case 9:
		idxX = 0;
		idxY = 1;
		break;
	case 8:
	case 7:
		idxX = 1;
		idxY = 1;
		break;
	case 6:
	case 5:
		idxX = 2;
		idxY = 1;
		break;
	case 4:
	case 3:
		idxX = 3;
		idxY = 1;
		break;
	case 2:
	case 1:
	case 0:
		idxX = 4;
		idxY = 1;
		break;
	default:
		break;
	}
}

void HpBar::Draw(bool isPlayer)
{
	int x = static_cast<int>(m_rect.center.x + m_rect.size.w / 2);
	int y = static_cast<int>(m_rect.center.y + m_rect.size.h / 2);

	//ÉvÉåÉCÉÑÅ[Ç∂Ç·Ç»Ç¢Ç∆Ç´â°Ç…Ç∏ÇÁÇ∑
	if (!isPlayer)	x = Game::kMapScreenRightX + m_rect.size.w + 10;

	my::MyDrawRectRotaGraph(x, y,
		idxX * m_rect.size.w, idxY* m_rect.size.h,
		m_rect.size.w, m_rect.size.h, kScale, 0.0f, m_HpHandle, true,false);
}

void HpBar::Damage(int damage)
{
	SetHp(m_Hp - damage);

	for (int i = 0; i < kHpMax; i++)
	{
		if (kHpMax - m_Hp - 1 >= i)
		{
			m_isHpGraph[i] = false;
		}
	}
}

void HpBar::Heal(const int heal)
{
	SetHp(m_Hp + heal);
}

void HpBar::SetHp(const int hp)
{
	if (hp >= m_MaxHp)
	{
		m_Hp = m_MaxHp;
	}
	else if (hp < 0)
	{
		m_Hp = 0;
	}
	else
	{
		m_Hp = hp;
	}
}
