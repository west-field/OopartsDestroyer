#include "RockBuster.h"
#include <DxLib.h>
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

//ショット開始
void RockBuster::Start(Position2 pos, Vector2 vel,bool left, bool isPlayer)
{
	ShotBase::Start(pos,vel,left,isPlayer);

	m_hitDamagePower = kAttackPower;
}

void RockBuster::Update()
{
	if (!m_isExist)	return;
	Movement({ kShotSpeed ,0.0f});
	ShotBase::Update();

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

//移動
void RockBuster::Movement(Vector2 vec)
{
	//移動速度が8.0fじゃないときは別のショットなので移動させない
	if (vec.x != 8.0f)	return;
	ShotBase::Movement(vec);
}
