#include "ShotBattery.h"
#include <DxLib.h>
#include "../Util/DrawFunctions.h"
#include "../game.h"

namespace
{
	constexpr int kShotFive = 2;//攻撃力
	constexpr float kShotBatterySpeed = 5.0f;//攻撃スピード
}

ShotBattery::ShotBattery(int handle):
	ShotBase(handle)
{
}

ShotBattery::~ShotBattery()
{
}

void ShotBattery::Start(Position2 pos, Vector2 vel, bool left, bool isPlayer)
{
	ShotBase::Start(pos, vel, left, isPlayer);

	m_vel *= kShotBatterySpeed;
	if (m_isLeft) m_vel *= -1.0f;
	m_hitDamagePower = kShotFive;
}

void ShotBattery::Update()
{
	int wsize = m_rect.size.w / 2;
	int hsize = m_rect.size.h / 2;
	Movement(m_vel);
	//左側
	if (m_rect.center.x - wsize < Game::kMapScreenLeftX)
	{
		m_isExist = false;
	}
	//右側
	if (m_rect.center.x + wsize > Game::kMapScreenRightX)
	{
		m_isExist = false;
	}
	//上端
	if (m_rect.center.y + hsize < Game::kMapScreenTopY)
	{
		m_isExist = false;
	}
	//下端
	if (m_rect.center.y - hsize > Game::kMapScreenBottomY)
	{
		m_isExist = false;
	}
}

void ShotBattery::Draw()
{
	if (!m_isExist)	return;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		0, 0, m_rect.size.w, m_rect.size.h, 1.5f, 0.0f, m_handle, true, m_isLeft);
#ifdef _DEBUG
	m_rect.Draw(0xaa00ff);
	DrawFormatString(Game::kMapScreenRightX, Game::kMapScreenTopY, 0x000000,L"%3f,%3f", m_vel.x, m_vel.y);
#endif
}

