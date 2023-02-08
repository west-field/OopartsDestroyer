#include "HpBar.h"
#include <DxLib.h>
#include "../Util/DrawFunctions.h"

namespace
{
	//拡大率
	constexpr float kScale = 1.0f;
	//グラフィックの大きさ
	constexpr int kGraphHeight = 3;
	constexpr int kGraphWidth = 30;
	//グラフィックの表示位置
	constexpr int kGraphPosX = kGraphWidth;
	constexpr int kGraphPosY = 0;
}

HpBar::HpBar():m_MaxHp(0),m_Hp(0),m_HpHandle(-1),m_pos()
{
	for (auto& hp : m_isHpGraph)
	{
		hp = true;
	}
}

HpBar::~HpBar()
{
	DeleteGraph(m_HpHandle);
}

void HpBar::Init(int handle)
{
	m_HpHandle = handle;

	m_MaxHp = m_Hp = kHpMax;
}

void HpBar::Update()
{

}

void HpBar::Draw(bool isPlayer)
{
	int x = kGraphPosX;
	int y = kGraphPosY+1;
	if (!isPlayer)	x += kGraphWidth + 10;

	for (int i = 0; i < kHpMax; i++)
	{
		if (m_isHpGraph[i])
		{
			my::MyDrawRectRotaGraph(x, static_cast<int>(y + i * (kGraphHeight * kScale)), 0, 0, kGraphWidth, kGraphHeight, kScale, 0.0f, m_HpHandle, true);
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
