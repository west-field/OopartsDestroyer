#include "RockBuster.h"
#include <DxLib.h>
#include "../game.h"
#include "../Util/DrawFunctions.h"

namespace
{
	constexpr float kShotSpeed = 8.0f;
	constexpr int kSize = 10;
}

RockBuster::RockBuster():m_handle(-1)
{
	//m_handle = my::MyLoadGraph(L"Data/");
}

RockBuster::~RockBuster()
{
	DeleteGraph(m_handle);
}

void RockBuster::Start(Position2 pos,bool left)
{
	m_isExist = true;
	m_isLeft = left;
	m_rect.center = pos;
	m_vec.x = kShotSpeed;
	m_vec.y = 0.0f;
	m_rect.size = { kSize ,kSize };
	if (m_isLeft) m_vec *= -1.0f;
}

void RockBuster::Update()
{
	if (!m_isExist)	return;
	/*m_pos += m_vec;
	if (m_pos.x > m_fieldRight.x)
	{
		m_isExist = false;
	}
	else if (m_pos.x < m_fieldLeft.x)
	{
		m_isExist = false;
	}*/
}

void RockBuster::Draw()
{
	if (!m_isExist)	return;
	//DrawGraphF(m_posX, m_posY, m_handle, true);
	float X = m_rect.center.x - m_rect.size.w/2;
	float Y = m_rect.center.y - m_rect.size.h/2;

	DrawBoxAA(X, Y, X + m_rect.size.w, Y + m_rect.size.h, 0xff00ff, false);
	//DrawCircleAA(m_pos.x, m_pos.y, 10, 0xff00ff, true);
}
