#include "HpBar.h"
#include <DxLib.h>
#include "../game.h"
#include "../Util/DrawFunctions.h"

namespace
{
	//拡大率
	constexpr float kScale = 1.0f;
}

HpBar::HpBar():m_MaxHp(0),m_Hp(0),m_HpHandle(-1),m_pos(), m_size()
{
	for (auto& hp : m_isHpGraph)
	{
		hp = true;
	}
	m_pos = { Game::kMapScreenLeftX + 10,Game::kMapScreenTopY };
}

HpBar::~HpBar()
{
	DeleteGraph(m_HpHandle);
}

void HpBar::Init(int handle)
{
	m_HpHandle = handle;

	GetGraphSize(m_HpHandle, &m_size.w, &m_size.h);

	m_MaxHp = m_Hp = kHpMax;
}

void HpBar::Update()
{

}

void HpBar::Draw(bool isPlayer)
{
	int x = static_cast<int>(m_pos.x);
	int y = static_cast<int>(m_pos .y)+1;

	//プレイヤーじゃないとき横にずらす
	if (!isPlayer)	x += m_size.w + 10;

	//HPバーの背景をつける
	DrawBox(x - m_size.w/2, y- m_size.h/2, x - m_size.w/2 + m_size.w, y - m_size.h / 2 + kHpMax * m_size.h, 0x000000, true);

	for (int i = 0; i < kHpMax; i++)
	{
		if (m_isHpGraph[i])
		{
			my::MyDrawRectRotaGraph(x, static_cast<int>(y + i * (m_size.h * kScale)),
				0, 0, m_size.w, m_size.h, kScale, 0.0f, m_HpHandle, true,false);
		}
	}

}

void HpBar::Damage(int damage)
{
	SetHp(m_Hp - damage);

	for (int i = 0; i < kHpMax; i++)
	{
		if (kHpMax - m_Hp - 1 == i)
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
