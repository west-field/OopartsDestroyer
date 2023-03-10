#include "RockBuster.h"
#include <DxLib.h>
#include "../game.h"
#include "../Util/DrawFunctions.h"

namespace
{
	constexpr float kShotSpeed = 8.0f;
	constexpr int kAttackPower = 1;//ロックバスターの攻撃力
}

RockBuster::RockBuster(int handle) : ShotBase(handle)
{
	
}

RockBuster::~RockBuster()
{
	
}

void RockBuster::Start(Position2 pos, Vector2 vel,bool left, bool isPlayer)
{
	m_isExist = true;
	m_isLeft = left;
	m_rect.center = pos;
	m_vel = vel;
	m_isPlayerShot = isPlayer;
	//if (m_isLeft) m_vel *= -1.0f;
}

void RockBuster::Update()
{
	if (!m_isExist)	return;
	ShotBase::Movement({ 8.0f ,0.0f});
	//画面の外に出たら消える
	float hsize, wsize;

	// 半分のサイズを算出
	wsize = m_rect.size.w * 0.5f;
	hsize = m_rect.size.h * 0.5f;
	//左端
	if (m_rect.GetCenter().x + wsize < Game::kMapScreenLeftX)
	{
		SetExist(false);
	}
	//右端
	if (m_rect.GetCenter().x - wsize > Game::kMapScreenRightX)
	{
		SetExist(false);
	}
	//上端
	if (m_rect.center.y + hsize < Game::kMapScreenTopY)
	{
		SetExist(false);
	}
	//下端
	if (m_rect.center.y - hsize > Game::kMapScreenBottomY)
	{
		SetExist(false);
	}

}

void RockBuster::Draw()
{
	if (!m_isExist)	return;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		0, 0, m_rect.size.w, m_rect.size.h, 1.5f, 0.0f, m_handle, true,m_isLeft);
#ifdef _DEBUG
	m_rect.Draw(0xaa00ff);
#endif
}

const int RockBuster::AttackPower() const
{
	return kAttackPower;
}
