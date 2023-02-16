#include "EnemyBattery.h"
#include <DxLib.h>
#include "../Util/DrawFunctions.h"
#include "../game.h"
#include "../Game/HpBar.h"
#include "../Game/ShotFactory.h"
#include "../Game/Player.h"

namespace
{
	constexpr int kBatteryTouchAttackPower = 1;//接触した時の攻撃力

	constexpr int anim_frame_speed = 20;//一枚に必要なフレーム数
	constexpr int anim_frame_num = 5;//アニメーション枚数
	constexpr int kSize = 32;
	constexpr float kDrawScall = 1.0f;
}

EnemyBattery::EnemyBattery(std::shared_ptr<Player>player, const Position2 pos, int handle, std::shared_ptr<ShotFactory> sFactory):
	EnemyBase(player,pos,sFactory),m_handle(handle)
{
	//矩形とサイズ
	m_rect = { pos, { static_cast<int>((kSize - kSize * 0.5)* kDrawScall),static_cast<int>(kSize* kDrawScall) } };
	//HPバー
	m_hp = std::make_shared<HpBar>();
	m_hp->MaxHp(1);//この敵のマックスHP
}

EnemyBattery::~EnemyBattery()
{

}

void EnemyBattery::Update()
{
	//存在していないときは更新しない
	if (!m_isExist) return;
	m_idx = (m_idx + 1) % (anim_frame_speed * anim_frame_num);

	if ( m_idx / anim_frame_speed == 2 && num == 0)
	{
		num++;
		m_shotFactory->Create(ShotType::RockBuster, m_rect.center, /*vel*/{0.0f,0.0f}, !m_isLeft);
	}
	else if(m_idx / anim_frame_speed == 0)
	{
		num = 0;
	}
}

void EnemyBattery::Draw()
{
	int imgX = (m_idx / anim_frame_speed) * kSize;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		imgX, 0, kSize, kSize, kDrawScall, 0.0f, m_handle, false, m_isLeft);
#ifdef _DEBUG
	m_rect.Draw(0xff0000);
	DrawFormatString(0, 360, 0xffffff, L"グラフィック切り替え%d", m_idx / anim_frame_speed);
#endif
}

void EnemyBattery::Movement(Vector2 vec)
{
	if (!m_isExist) return;

	m_rect.center += vec;
}

int EnemyBattery::TouchAttackPower() const
{
	return kBatteryTouchAttackPower;
}

void EnemyBattery::Damage(int damage)
{
	m_hp->Damage(damage);
	//HPが0のとき、存在しない
	if (m_hp->GetHp() == 0)
	{ 
		m_isExist = false; 
	}
}
