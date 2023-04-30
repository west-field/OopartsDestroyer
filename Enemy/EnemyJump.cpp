#include "EnemyJump.h"
#include <DxLib.h>
#include "../Util/DrawFunctions.h"
#include "../Util/Sound.h"
#include "../Game/HpBar.h"
#include "../game.h"
#include "../Info.h"
#include "../Game/Player.h"
#include "../Game/ItemFactory.h"
#include "../Util/Sound.h"

namespace
{
	constexpr int kTouchAttackPower = 4;//接触した時の攻撃力

	//エネミーアニメーション
	constexpr int kAnimFrameSpeed = 5;//一枚に必要なフレーム数
	constexpr int kAnimFrameNum = 2;//アニメーション枚数
	constexpr int kRand = kAnimFrameNum * kAnimFrameSpeed;
	constexpr int kJumpSize = 32;//グラフィック1つの大きさ
	constexpr float kDrawScale = 1.0f;//グラフィック拡大率
	constexpr float kEnemyMoveSpeed = -4.0f;//エネミーの移動速度
	constexpr float kGrap = 2.0f;//落下
	constexpr float kJumpA = 5.0f;//ジャンプ力
}

EnemyJump::EnemyJump(std::shared_ptr<Player> player, const Position2 pos, int handle, int burstH, std::shared_ptr<ShotFactory> sFactory, std::shared_ptr<ItemFactory> itFactory):
	EnemyBase(player,pos,handle,burstH,sFactory,itFactory),m_updateFunc(&EnemyJump::NormalUpdate),m_drawFunc(&EnemyJump::NormalDraw),
	m_frame(0),m_posTemp(0.0f)
{
	m_rect.size = { static_cast<int>(kJumpSize * Game::kScale * kDrawScale),
					static_cast<int>(kJumpSize * Game::kScale * kDrawScale)};
	m_hp->MaxHp(1);
	m_frame = GetRand(kRand);//ジャンプするまでの時間をランダムで決める

	m_touchDamagePower = kTouchAttackPower;
}

EnemyJump::~EnemyJump()
{

}

void EnemyJump::Update()
{
	(this->*m_updateFunc)();
}

void EnemyJump::Draw()
{
	(this->*m_drawFunc)();
}

//当たり判定対象かどうか
bool EnemyJump::IsCollidable() const
{
	//BurstUpdateの時は当たらない
	return (m_updateFunc != &EnemyJump::BurstUpdate);
}

//ダメージを受けた
void EnemyJump::Damage(int damage)
{
	m_hp->Damage(damage);

	if (m_hp->GetHp() == 0)
	{
		SoundManager::GetInstance().Play(SoundId::EnemyBurst);
		m_updateFunc = &EnemyJump::BurstUpdate;
		m_drawFunc = &EnemyJump::BurstDraw;
		m_idx = 0;
		m_itemFactory->Create(ItemType::Heal, m_rect.center);//回復アイテム
		return;
	}
	SoundManager::GetInstance().Play(SoundId::EnemyHit);
}
//通常更新
void EnemyJump::NormalUpdate()
{
	//ランダムな時にジャンプさせる
	if (m_frame-- <= 0)
	{
		m_frame = GetRand(kRand) * kAnimFrameSpeed + 60;//次ジャンプするまでの時間
		m_idx = 1;
		SoundManager::GetInstance().Play(SoundId::EnemyJump);
		m_posTemp =  m_rect.center.y - Game::kDrawSize * 3;//三ブロックぐらいジャンプする

		//プレイヤーのいる方向に移動する
		if (m_player->GetRect().GetCenter().x > m_rect.center.x)
		{
			m_vec = { 1.0f,-kJumpA };
		}
		else
		{
			m_vec = { -1.0f,-kJumpA };
		}

		m_updateFunc = &EnemyJump::JumpUpdate;
		return;
	}
}

//通常表示
void EnemyJump::NormalDraw()
{
	int img = m_idx * kJumpSize;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		img, 0, kJumpSize, kJumpSize, kDrawScale * Game::kScale, 0.0f, m_handle, true, m_isLeft);
#ifdef _DEBUG
	m_rect.Draw(0xff0000);
#endif
}

//爆発更新
void EnemyJump::BurstUpdate()
{
	m_idx++;
	if (m_idx == kBurstAnimNum * kBurstAnimSpeed)
	{
		m_isExist = false;
	}
}

//爆発表示
void EnemyJump::BurstDraw()
{
	int imgX = (m_idx / kBurstAnimSpeed) * kBurstImgWidth;

	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		imgX, 0, kBurstImgWidth, kBurstImgHeight, kBurstDrawScale * Game::kScale, 0.0f, m_burstHandle, true,false);
}

//ジャンプする
void EnemyJump::JumpUpdate()
{
	m_rect.center += m_vec;

	if (m_rect.center.y <= m_posTemp)
	{
		m_vec.y = kGrap;
		m_updateFunc = &EnemyJump::DownUpdate;
		return;
	}
}

//落下する
void EnemyJump::DownUpdate()
{
	m_rect.center += m_vec;
	//当たり判定のあるブロックに当たったら止める
	if (m_chipId == 1)
	{
		m_chipId = 0;
		m_idx = 0;
		m_rect.center.y -= kGrap;
		m_updateFunc = &EnemyJump::NormalUpdate;
		return;
	}
}