#include "EnemyJump.h"
#include <DxLib.h>
#include "../Util/DrawFunctions.h"
#include "../Util/Sound.h"
#include "../Game/HpBar.h"
#include "../game.h"
#include "../Game/Player.h"

namespace
{
	constexpr int kJumpTouchAttackPower = 4;//接触した時の攻撃力

	constexpr int anim_frame_speed = 5;//一枚に必要なフレーム数
	constexpr int anim_frame_num = 2;//アニメーション枚数

	constexpr int kRand = anim_frame_num * anim_frame_speed;

	constexpr int kJumpSize = 32;//グラフィック1つの大きさ
	constexpr float kDrawScall = 1.0f;//グラフィック拡大率

	constexpr float kEnemyMoveSpeed = -4.0f;//エネミーの移動速度

	constexpr float kGrap = 2.0f;
}

EnemyJump::EnemyJump(std::shared_ptr<Player> player, const Position2 pos, int handle, std::shared_ptr<ShotFactory> sFactory):
	EnemyBase(player,pos,sFactory),m_updateFunc(&EnemyJump::MoveUpdate)
{
	m_handle = handle;
	m_rect = { pos,{static_cast<int>(kJumpSize * kDrawScall),static_cast<int>(kJumpSize * kDrawScall)} };
	m_hp->MaxHp(1);
	m_frame = GetRand(kRand);
}

EnemyJump::~EnemyJump()
{
}

void EnemyJump::Update()
{
	if (!m_isExist)	return;

	(this->*m_updateFunc)();
}

void EnemyJump::Draw()
{
	if (!m_isExist)	return;
	int img = m_idx * kJumpSize;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		img, 0, kJumpSize, kJumpSize, kDrawScall, 0.0f, m_handle, false, m_isLeft);
#ifdef _DEBUG
	m_rect.Draw(0xff0000);
#endif
}

void EnemyJump::Movement(Vector2 vec)
{
	if (!m_isExist)	return;
	m_rect.center += vec;
}

int EnemyJump::TouchAttackPower() const
{
	return kJumpTouchAttackPower;
}

void EnemyJump::MoveUpdate()
{
	//ランダムな時にジャンプさせる
	if (m_frame-- <= 0)
	{
		m_frame = GetRand(kRand) * anim_frame_speed + 60;
		m_idx = 1;
		m_rect.center.y -= Game::ChipSize * 2;
		m_rect.center.x -= 5.0f;
		Sound::Play(Sound::EnemyJump);
		m_updateFunc = &EnemyJump::JumpUpdate;
	}
}

void EnemyJump::JumpUpdate()
{
	if (m_player->GetRect().GetCenter().x > m_rect.center.x)
	{
		m_rect.center.x += 1.0f;
	}
	else
	{
		m_rect.center.x -= 1.0f;
	}

	m_updateFunc = &EnemyJump::DownUpdate;
}

void EnemyJump::DownUpdate()
{
	m_rect.center.y += kGrap;

	if (m_player->GetRect().GetCenter().x > m_rect.center.x)
	{
		m_rect.center.x += 1.0f;
	}
	else
	{
		m_rect.center.x -= 1.0f;
	}

	if (m_chipId == 1)
	{
		m_idx = 0;
		m_rect.center.y -= kGrap;
		m_updateFunc = &EnemyJump::MoveUpdate;
	}
}
