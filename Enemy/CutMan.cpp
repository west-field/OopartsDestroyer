#include "CutMan.h"
#include <DxLib.h>
#include "../Util/DrawFunctions.h"
#include "../Game/ShotFactory.h"
#include "../Game/Player.h"
#include "../Game/HpBar.h"

namespace
{
	constexpr int kCutManTouchAttackPower = 4;//接触した時の攻撃力

	constexpr float kSpeed = 4.0f;	//移動スピード
	constexpr int kGraphNum = 13;	//グラフィックの数
	constexpr int kGraphSizeWidth = 2600/kGraphNum;		//サイズ
	constexpr int kGraphSizeHeight = 400;	//サイズ
	constexpr float kDrawScale = 0.5f;		//拡大率
	constexpr int kAnimFrameSpeed = 5;	//グラフィックアニメーションスピード
	constexpr int kSizeX = 30;	//グラフィックサイズ
	constexpr int kSizeY = kSizeX;

	constexpr int kJumpInterval = 100;

	//ジャンプ
	constexpr float kGravity = 0.8f;//重力
	constexpr float kJumpAcc = -10.0f;//ジャンプ力
}

CutMan::CutMan(std::shared_ptr<Player>player, const Position2& pos, int handle,std::shared_ptr<ShotFactory> sFactory):
	EnemyBase(player,pos,sFactory),updateFunc(&CutMan::StopUpdate), m_shotFrame(0), m_JumpFrame(kJumpInterval)
{
	m_isLeft = true;
	m_handle = handle;
	m_rect = { pos,{static_cast<int>(kGraphSizeWidth* kDrawScale/2),static_cast<int>(kGraphSizeHeight* kDrawScale)} };
	//m_vec = { kSpeed, kJumpAcc };
}

CutMan::~CutMan()
{
}

void CutMan::Update()
{
	if (!m_isExist)return;
	if (m_frame++ > kAnimFrameSpeed) 
	{
		m_idx = (m_idx + 1) % kGraphNum; 
		m_frame = 0;
	}
	
	if (--m_ultimateTimer <= 0)
	{
		m_ultimateTimer = 0;
	}
	(this->*updateFunc)();
}

void CutMan::Draw()
{
	if (!m_isExist)return;
	
	//無敵時間点滅させる
	if ((m_ultimateTimer / 10) % 2 == 1)	return;

	int img = m_idx * kGraphSizeWidth;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y), img, 0, kGraphSizeWidth, kGraphSizeHeight, kDrawScale, 0.0f, m_handle, true, m_isLeft);
#ifdef _DEBUG
	DrawFormatString(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y), 0xffffff, L"%d", m_idx);
	m_rect.Draw(0xaaffaa);
#endif

}

void CutMan::Movement(Vector2 vec)
{
	if (!m_isExist)	return;
	//画面と一緒に移動
	m_rect.center += vec;
}

void CutMan::EnemyMovement(Vector2 vec)
{
	if (!m_isExist)	return;
	//ジャンプしているとき
	m_rect.center.y += vec.y;
	if (m_isJump)
	{
		m_rect.center.x += vec.x;
	}
}

int CutMan::TouchAttackPower() const
{
	return kCutManTouchAttackPower;
}

bool CutMan::IsCollidable() const
{
	return /*(updateFunc == &CutMan::MoveUpdate) &&*/ m_ultimateTimer == 0;
}

void CutMan::MoveUpdate()
{
	//左を向いていたら方向を変える
	if (m_isLeft) m_vec.x *= -1.0f;
	//ジャンプさせる
	m_vec.y = kJumpAcc - (GetRand(100) % 5);
	updateFunc = &CutMan::JumpUpdate;
	return;
}

void CutMan::StopUpdate()
{
	return;

	//ダメージを受けたら弾を打つ
	if (m_isOnDamage)
	{
		if (m_hp->GetHp() <= m_hp->GetMaxHp() / 2)
		{
			updateFunc = &CutMan::TwoShotUpdate;
			m_isOnDamage = false;
			return;
		}
		else
		{
			updateFunc = &CutMan::OneShotUpdate;
			m_isOnDamage = false;
			return;
		}
	}
	//弾を打たないときは移動する
	else if(m_JumpFrame++ >= kJumpInterval)
	{
		m_JumpFrame = 0;
		
		//左を向いていたら方向を変える
		if (m_isLeft) m_vec.x *= -1.0f;
		//ジャンプさせる
		//m_vec.y = kJumpAcc - (GetRand(100) % 5);
		m_posTemp = m_rect.center.y + ( kJumpAcc - (GetRand(100) % 5));
		updateFunc = &CutMan::JumpUpdate;
		return;
	}
}

void CutMan::JumpUpdate()
{
	m_rect.center.y += m_vec.y;

	if (m_rect.center.y <= m_posTemp) 
	{
		updateFunc = &CutMan::DownUpdate;
		return;
	}
}

void CutMan::DownUpdate()
{
	m_vec.y += kGravity;//下に落ちる

	m_rect.center.x += m_vec.x;

	if(!m_isJump)
	{
		//プレイヤーの方向にジャンプ
		if (m_player->GetRect().GetCenter().x > m_rect.center.x)
		{
			m_isLeft = false;
		}
		else
		{
			m_isLeft = true;
		}
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
	m_JumpFrame = 0;
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
	if (m_shotFrame-- % 10 == 0)
	{
		m_shotFactory->Create(ShotType::RockBuster, m_rect.center, vel, m_isLeft);
	}

	if (m_shotFrame == 0)
	{
		m_shotFrame = 20;
		//次の指示を待つ
		m_JumpFrame = 0;
		updateFunc = &CutMan::StopUpdate;
		return;
	}
}
