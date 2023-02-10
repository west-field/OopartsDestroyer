#include "RockBuster.h"
#include <DxLib.h>
#include "../game.h"
#include "../Util/DrawFunctions.h"

namespace
{
	constexpr float kShotSpeed = 8.0f;
	constexpr int kAttackPower = 1;//ロックバスターの攻撃力
}

RockBuster::RockBuster()
{
	
}

RockBuster::~RockBuster()
{
	
}

void RockBuster::Start(Position2 pos,bool left)
{
	m_isExist = true;
	m_isLeft = left;
	m_rect.center = pos;
	m_vel.x = kShotSpeed;
	m_vel.y = 0.0f;
	if (m_isLeft) m_vel *= -1.0f;
}

void RockBuster::Update()
{
	if (!m_isExist)	return;
}

void RockBuster::Draw()
{
	if (!m_isExist)	return;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		0, 0, m_rect.size.w, m_rect.size.h, 1.0f, 0.0f, m_handle, true,m_isLeft);
	m_rect.Draw(0xaa00ff);
}

const int RockBuster::AttackPower() const
{
	return kAttackPower;
}
