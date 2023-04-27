#include "EnemyBattery.h"
#include <DxLib.h>
#include "../Util/DrawFunctions.h"
#include "../game.h"
#include "../Info.h"
#include "../Game/HpBar.h"
#include "../Game/ShotFactory.h"
#include "../Game/ItemFactory.h"
#include "../Game/Player.h"
#include "../Util/Sound.h"

namespace
{
	constexpr int kTouchAttackPower = 1;//接触した時の攻撃力

	//エネミーアニメーション
	constexpr int kAnimFrameSpeed = 20;//一枚に必要なフレーム数
	constexpr int kAnimFrameNum = 5;//アニメーション枚数
	constexpr int kSize = 32;//大きさ
	constexpr float kDrawScale = 1.0f;//表示拡大率
}

EnemyBattery::EnemyBattery(std::shared_ptr<Player>player, const Position2 pos, int handle, int burstH, std::shared_ptr<ShotFactory> sFactory, std::shared_ptr<ItemFactory> itFactory,bool isLeft):
	EnemyBase(player,pos, handle, burstH, sFactory,itFactory),m_updateFunc(&EnemyBattery::NormalUpdate),m_drawFunc(&EnemyBattery::NormalDraw)
{
	m_rect.size = {  static_cast<int>(kSize * Game::kScale * kDrawScale),static_cast<int>(kSize * Game::kScale * kDrawScale) };
	
	m_hp->MaxHp(1);//この敵のマックスHP
	m_isLeft = isLeft;
	m_touchDamagePower = kTouchAttackPower;
}

EnemyBattery::~EnemyBattery()
{
	
}

void EnemyBattery::Update()
{
	(this->*m_updateFunc)();
}

void EnemyBattery::Draw()
{
	(this->*m_drawFunc)();
}

void EnemyBattery::Damage(int damage)
{
	m_hp->Damage(damage);
	
	if (m_hp->GetHp() == 0)
	{
		SoundManager::GetInstance().Play(SoundId::EnemyBurst);
		m_updateFunc = &EnemyBattery::BurstUpdate;
		m_drawFunc = &EnemyBattery::BurstDraw;
		m_idx = 0;
		//左を向ているときは回復アイテムを落とさない
		if (GetRand(100) % 3 == 0 && m_isLeft)
		{
			m_itemFactory->Create(ItemType::Heal, m_rect.center);
		}
		return;
	}
	SoundManager::GetInstance().Play(SoundId::EnemyHit);
}

bool EnemyBattery::IsCollidable() const
{
	//BurstUpdateの時は当たらない
	return (m_updateFunc != &EnemyBattery::BurstUpdate);
}

void EnemyBattery::NormalUpdate()
{
	//アニメーション
	m_idx = (m_idx + (GetRand(10) % 3)) % (kAnimFrameSpeed * kAnimFrameNum);
	//攻撃モーションの時弾を発射する
	if (m_idx / kAnimFrameSpeed == 2 && m_num == 0)
	{
		Vector2 vel = { 0.0f,0.0f };
		//ランダムに方向と速度を決定する（斜め
		vel.x = -1.0f;
		vel.y = static_cast<float>(GetRand(100) - 50) / 50.0f;
		if (!m_isLeft) vel *= -1.0f;
		vel.Normalize();
		
		m_shotFactory->Create(ShotType::ShotBattery, m_rect.center, vel, m_isLeft,false);
		m_num++;
	}
	//攻撃モーションを終えたら初期化する
	else if (m_idx / kAnimFrameSpeed == 0)
	{
		m_num = 0;
	}
}

void EnemyBattery::NormalDraw()
{
	int imgX = (m_idx / kAnimFrameSpeed) * kSize;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		imgX, 0, kSize, kSize, kDrawScale * Game::kScale, 0.0f, m_handle, true, m_isLeft);
#ifdef _DEBUG
	m_rect.Draw(0xff0000);
	DrawFormatString(0, 360, 0xffffff, L"グラフィック切り替え%d", m_idx / kAnimFrameSpeed);
#endif
}

void EnemyBattery::BurstUpdate()
{
	m_idx++;
	if (m_idx == kBurstAnimNum * kBurstAnimSpeed)
	{
		m_isExist = false;
	}
}

void EnemyBattery::BurstDraw()
{
	int imgX = (m_idx / kBurstAnimSpeed) * kBurstImgWidth;

	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		imgX, 0, kBurstImgWidth, kBurstImgHeight, kBurstDrawScale * Game::kScale, 0.0f, m_burstHandle, true, false);
}
