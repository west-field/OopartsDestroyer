#include "EnemyBattery.h"
#include <DxLib.h>
#include "../Util/DrawFunctions.h"
#include "../game.h"
#include "HpBar.h"

namespace
{
	constexpr int kBatteryTouchAttackPower = 1;//接触した時の攻撃力

	constexpr int anim_frame_speed = 5;//一枚に必要なフレーム数
	constexpr int anim_frame_num = 5;//アニメーション枚数
	constexpr int kSize = 29;
	constexpr float kDrawScall = 0.8f;
}

EnemyBattery::EnemyBattery(std::shared_ptr<Player>player, const Position2 pos, int handle, std::shared_ptr<ShotFactory> sFactory):
	EnemyBase(player,pos,sFactory),m_handle(handle)
{
	m_rect = { pos, { static_cast<int>(kSize* kDrawScall),static_cast<int>(kSize* kDrawScall) } };
	m_hp = std::make_shared<HpBar>();
	m_hp->MaxHp(1);
}

EnemyBattery::~EnemyBattery()
{

}

void EnemyBattery::Update()
{
	if (m_rect.center.x < -kSize-20)
	{
		m_isExist = false;
	}
	else if (m_rect.center.x > Game::kScreenWidth + kSize+20)
	{
		m_isExist = false;
	}
}

void EnemyBattery::Draw()
{
	int imgX = (m_idx / anim_frame_speed) * 29;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		imgX, 0, kSize, kSize, kDrawScall, 0.0f, m_handle, true, m_isLeft);
#ifdef _DEBUG
	m_rect.Draw(0xff0000);
#endif
}

void EnemyBattery::Movement(Vector2 vec)
{
	if (!m_isExist) return;
	m_rect.center.y += vec.y;

	m_idx = (m_idx + 1) % (anim_frame_speed * anim_frame_num);
}

int EnemyBattery::TouchAttackPower() const
{
	return kBatteryTouchAttackPower;
}

void EnemyBattery::Damage(int damage)
{
	m_hp->Damage(damage);
	if (m_hp->GetHp() == 0)
	{ 
		m_isExist = false; 
	}
}
