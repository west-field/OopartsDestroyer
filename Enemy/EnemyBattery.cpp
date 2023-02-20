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

EnemyBattery::EnemyBattery(std::shared_ptr<Player>player, const Position2 pos, int handle, std::shared_ptr<ShotFactory> sFactory, std::shared_ptr<Stage> stage,bool isLeft):
	EnemyBase(player,pos,sFactory,stage),m_handle(handle)
{
	//矩形とサイズ
	m_rect = { pos, { static_cast<int>(kSize * kDrawScall),static_cast<int>(kSize* kDrawScall) } };
	
	m_hp->MaxHp(1);//この敵のマックスHP
	m_isLeft = isLeft;
}

EnemyBattery::~EnemyBattery()
{

}

void EnemyBattery::Update()
{
	//存在していないときは更新しない
	if (!m_isExist) return;
	m_idx = (m_idx + (GetRand(10) % 3) ) % (anim_frame_speed * anim_frame_num);

	if ( m_idx / anim_frame_speed == 2 && num == 0)
	{
		num++;
		/*
		角度	まっすぐ0.0f
				縦　90度
				斜め30度60度
		*/
		//m_shotFactory->Create(ShotType::ShotBattery, m_rect.center, /*vel*/{0.0f,0.0f}, !m_isLeft);//まっすぐ
		
		Vector2 vel;
		vel.x = cos(5.0);
		vel.y = sin(5.0);
		m_shotFactory->Create(ShotType::ShotBattery, m_rect.center, vel, m_isLeft);//斜め
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
