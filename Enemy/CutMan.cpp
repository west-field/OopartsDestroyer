#include "CutMan.h"
#include <DxLib.h>
//#include "../Scene/GameplayingScene.h"
#include "../Util/DrawFunctions.h"
#include "../Game/ShotFactory.h"
#include "../Game/Player.h"

namespace
{
	constexpr int kCutManTouchAttackPower = 4;//接触した時の攻撃力

	constexpr float kSpeed = 4.0f;	//移動スピード
	constexpr int kSizeX = 30;	//グラフィックサイズ
	constexpr int kSizeY = kSizeX;

	constexpr int kJumpInterval = 100;

	//ジャンプ
	constexpr float kGravity = 0.8f;//重力
	constexpr float kJumpAcc = -10.0f;//ジャンプ力
}

CutMan::CutMan(std::shared_ptr<Player>player, const Position2& pos, int handle,std::shared_ptr<ShotFactory> sFactory):
	EnemyBase(player,pos,sFactory),updateFunc(&CutMan::StopUpdate), m_frame(0)
{
	m_isLeft = true;
	m_handle = handle;
	m_rect = { pos,{kSizeX,kSizeY} };
}

CutMan::~CutMan()
{
}

void CutMan::Update()
{
	if (!m_isExist)return;
	(this->*updateFunc)();
}

void CutMan::Draw()
{
	if (!m_isExist)return;
	int img = m_idx * kSizeX;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y), img, 0, kSizeX, kSizeY, 1.0f, 0.0f, m_handle, true, m_isLeft);
}

void CutMan::OnDamage(int damage)
{
	EnemyBase::Damage(damage);
	m_isOnDamage = true;
}

void CutMan::Movement(Vector2 vec)
{
	if (!m_isExist)	return;
	m_rect.center += vec;
}

int CutMan::TouchAttackPower() const
{
	return kCutManTouchAttackPower;
}

void CutMan::MoveUpdate()
{
	//移動速度をもとに戻す
	m_vec = { kSpeed, 0.0f };
	//左を向いていたら方向を変える
	if (m_isLeft) m_vec *= -1.0f;
	//ジャンプさせる
	m_vec.y = kJumpAcc - (GetRand(100) % 5);
	updateFunc = &CutMan::JumpUpdate;
	return;
}

void CutMan::StopUpdate()
{
	//ダメージを受けたら弾を打つ
	if (m_isOnDamage)
	{
		int rand = GetRand(10) % 2;
		switch (rand)
		{
		case 0:
			updateFunc = &CutMan::OneShotUpdate;
			return;
		case 1:
			updateFunc = &CutMan::TwoShotUpdate;
			return;
		default:
			break;
		}
	}
	//弾を打たないときは移動する
	else
	{
		updateFunc = &CutMan::MoveUpdate;
		return;
	}
}

void CutMan::JumpUpdate()
{
	//ジャンプした位置が壁の時は移動しない
	if (m_chipId == 0)
	{
		m_rect.center += m_vec;//プレイヤーを移動
		m_vec.y += kGravity;//下に落ちる
	}
	else
	{
		updateFunc = &CutMan::StopUpdate;
		return;
	}
}

void CutMan::OneShotUpdate()
{
	//自機狙い弾を作る　自機狙いベクトル=終点(プレイヤー座標)　-　始点(敵機自身の座標)
	auto vel = m_player->GetRect().GetCenter() - m_rect.center;

	if (vel.SQLength() == 0.0f)
	{
		vel = { -2.0f,0.0f };//敵機の時期が重なっているときは既定の方向　真左に飛ばしておく
	}
	else
	{
		vel.Normalize();
		vel *= 2.0f;
	}
	//ショットを一回打つ
	m_shotFactory->Create(ShotType::RockBuster, m_rect.center, vel, m_isLeft);

	//次の指示を待つ
	updateFunc = &CutMan::StopUpdate;
	return;
}

void CutMan::TwoShotUpdate()
{
	//自機狙い弾を作る　自機狙いベクトル=終点(プレイヤー座標)　-　始点(敵機自身の座標)
	auto vel = m_player->GetRect().GetCenter() - m_rect.center;

	if (vel.SQLength() == 0.0f)
	{
		vel = { -2.0f,0.0f };//敵機の時期が重なっているときは既定の方向　真左に飛ばしておく
	}
	else
	{
		vel.Normalize();
		vel *= 2.0f;
	}
	//ショットを二回打つ
	if (m_frame-- % 10 == 0)
	{
		m_shotFactory->Create(ShotType::RockBuster, m_rect.center, vel, m_isLeft);
	}

	if (m_frame == 0)
	{
		m_frame = 20;
		//次の指示を待つ
		updateFunc = &CutMan::StopUpdate;
		return;
	}
}
