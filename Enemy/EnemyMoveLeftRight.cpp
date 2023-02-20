#include "EnemyMoveLeftRight.h"
#include <DxLib.h>
#include "../Util/DrawFunctions.h"
#include "../Game/HpBar.h"
#include "../Stage.h"

namespace
{
	constexpr int kLeftRightTouchAttackPower = 4;//接触した時の攻撃力

	constexpr int anim_frame_speed = 5;//一枚に必要なフレーム数
	constexpr int anim_frame_num = 3;//アニメーション枚数

	constexpr int kLeftRightSize = 32;//グラフィック1つの大きさ
	constexpr float kDrawScall = 1.0f;//グラフィック拡大率

	constexpr float kEnemyMoveSpeed = 4.0f;//エネミーの移動速度
}

EnemyMoveLeftRight::EnemyMoveLeftRight(std::shared_ptr<Player> player, const Position2 pos, int handle, std::shared_ptr<ShotFactory> sFactory, std::shared_ptr<Stage> stage) :
	EnemyBase(player, pos, sFactory,stage), m_handle(handle),m_updateFunc(&EnemyMoveLeftRight::NormalUpdate)
{
	int X, Y;
	GetGraphSize(m_handle, &X, &Y);
	m_rect = { pos,{X/anim_frame_num,Y} };
	m_hp->MaxHp(5);
}

EnemyMoveLeftRight::~EnemyMoveLeftRight()
{
}

void EnemyMoveLeftRight::Update()
{
	if (!m_isExist)	return;
	
	(this->*m_updateFunc)();
}

void EnemyMoveLeftRight::Draw()
{
	if (!m_isExist)	return;
	int img = (m_idx / anim_frame_speed) * kLeftRightSize;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		img, 0, kLeftRightSize, kLeftRightSize, kDrawScall, 0.0f, m_handle, true, m_isLeft);
#ifdef _DEBUG
	m_rect.Draw(0xff00ff);
#endif
}

void EnemyMoveLeftRight::Movement(Vector2 vec)
{
	if (!m_isExist)	return;

	m_rect.center += vec;
}

int EnemyMoveLeftRight::TouchAttackPower() const
{
	return kLeftRightTouchAttackPower;
}

void EnemyMoveLeftRight::NormalUpdate()
{
	m_idx = 0;
	//移動したところが壁かどうか
	if (m_frame++ >= 120)
	{
		m_idx = 1;
		m_frame = 0;
		if (m_isLeft)
		{
			m_updateFunc = &EnemyMoveLeftRight::LeftUpdate;
		}
		else
		{
			m_updateFunc = &EnemyMoveLeftRight::RightUpdate;
		}
	}
}

void EnemyMoveLeftRight::LeftUpdate()
{
	m_idx = 2;
	m_rect.center.x -= kEnemyMoveSpeed;
	if (m_chipId == 1)
	{
		m_idx = 1;
		m_isLeft = false;
		m_updateFunc = &EnemyMoveLeftRight::NormalUpdate;
	}
}

void EnemyMoveLeftRight::RightUpdate()
{
	m_idx = 2;
	m_rect.center.x += kEnemyMoveSpeed;
	if (m_chipId == 1)
	{
		m_idx = 1;
		m_isLeft = true;
		m_updateFunc = &EnemyMoveLeftRight::NormalUpdate;
	}
}
