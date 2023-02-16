#include "EnemyMoveUpDown.h"
#include <DxLib.h>
#include "../Util/DrawFunctions.h"
#include "../game.h"
#include "../Game/HpBar.h"
#include "../Game/Player.h"

namespace
{
	constexpr int kUpDownTouchAttackPower = 4;//接触した時の攻撃力

	constexpr int anim_frame_speed = 5;//一枚に必要なフレーム数
	constexpr int anim_frame_num = 5;//アニメーション枚数
	constexpr int kSize = 29;
	constexpr float kDrawScall = 0.8f;
	constexpr float kEnemyMoveSpeed = -4.0f;//エネミーの移動速度
}

EnemyMoveUpDown::EnemyMoveUpDown(std::shared_ptr<Player>player, const Position2 pos, int handle, std::shared_ptr<ShotFactory> sFactory):
	EnemyBase(player,pos,sFactory),m_handle(handle)
{
	m_rect = { pos, { static_cast<int>(kSize* kDrawScall),static_cast<int>(kSize* kDrawScall) } };
	m_hp = std::make_shared<HpBar>();
	m_hp->MaxHp(1);
}

EnemyMoveUpDown::~EnemyMoveUpDown()
{

}

void EnemyMoveUpDown::Update()
{
	//存在していないときは更新しない
	if (!m_isExist) return;
}

void EnemyMoveUpDown::Draw()
{
	int imgX = (m_idx / anim_frame_speed) * 29;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		imgX, 0, kSize, kSize, kDrawScall, 0.0f, m_handle, true, m_isLeft);
#ifdef _DEBUG
	m_rect.Draw(0xff0000);
#endif
}

void EnemyMoveUpDown::Movement(Vector2 vec)
{
	if (!m_isExist) return;
	if (m_isLeft) vec *= 1.0f;

	m_rect.center += vec;
	m_rect.center.x += kEnemyMoveSpeed;
	m_idx = (m_idx + 1) % (anim_frame_speed * anim_frame_num);
}

int EnemyMoveUpDown::TouchAttackPower() const
{
	return kUpDownTouchAttackPower;
}

void EnemyMoveUpDown::Damage(int damage)
{
	m_hp->Damage(damage);
	if (m_hp->GetHp() == 0)
	{ 
		m_isExist = false; 
	}
}
