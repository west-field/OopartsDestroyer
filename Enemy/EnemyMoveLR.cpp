#include "EnemyMoveLR.h"
#include <DxLib.h>
#include "../Util/DrawFunctions.h"
#include "../Game/HpBar.h"
#include "../game.h"
#include "../Info.h"
#include "../Util/Sound.h"
#include "../Game/ItemFactory.h"

namespace
{
	constexpr int kTouchAttackPower = 4;//接触した時の攻撃力

	//エネミーアニメーション
	constexpr int kAnimFrameSpeed = 5;//一枚に必要なフレーム数
	constexpr int kAnimFrameNum = 3;//アニメーション枚数
	constexpr int kLeftRightSize = 32;//グラフィック1つの大きさ
	constexpr float kDrawScale = 1.0f;//グラフィック拡大率
	constexpr float kEnemyMoveSpeed = 4.0f;//エネミーの移動速度
}

EnemyMoveLR::EnemyMoveLR(std::shared_ptr<Player> player, const Position2 pos, int handle, int burstH, std::shared_ptr<ShotFactory> sFactory, std::shared_ptr<ItemFactory> itFactory) :
	EnemyBase(player, pos,handle,burstH, sFactory,itFactory), m_updateFunc(&EnemyMoveLR::NormalUpdate),m_drawFunc(&EnemyMoveLR::NormalDraw)
{
	m_rect.size = { static_cast<int>(kLeftRightSize * Game::kScale),static_cast<int>(kLeftRightSize * Game::kScale)};
	m_hp->MaxHp(3);

	if (GetRand(100) / 2 == 0)
	{
		m_isLeft = true;
	}

	m_touchDamagePower = kTouchAttackPower;
}

EnemyMoveLR::~EnemyMoveLR()
{

}

void EnemyMoveLR::Update()
{
	(this->*m_updateFunc)();
}

void EnemyMoveLR::Draw()
{
	(this->*m_drawFunc)();
}

void EnemyMoveLR::Damage(int damage)
{
	m_hp->Damage(damage);
	m_isOnDamage = true;
	if (m_hp->GetHp() == 0)
	{
		SoundManager::GetInstance().Play(SoundId::EnemyBurst);
		m_updateFunc = &EnemyMoveLR::BurstUpdate;
		m_drawFunc = &EnemyMoveLR::BurstDraw;
		m_idx = 0;
		if (GetRand(100) % 2 == 0)
		{
			m_itemFactory->Create(ItemType::Heal, m_rect.center);
		}
		return;
	}
	SoundManager::GetInstance().Play(SoundId::EnemyHit);
}

bool EnemyMoveLR::IsCollidable() const
{
	//BurstUpdateの時は当たらない
	return (m_updateFunc != &EnemyMoveLR::BurstUpdate);
}

void EnemyMoveLR::NormalUpdate()
{
	//2秒間ぐらい止まる
	if (m_frame++ >= 120)
	{
		//半目
		m_idx = 1;
		//フレームカウントをゼロにする
		m_frame = 0;

		m_updateFunc = &EnemyMoveLR::MoveUpdate;
	}
	//止まっている間は目を閉じた画像を表示させる
	else
	{
		m_idx = 0;
	}
}

void EnemyMoveLR::NormalDraw()
{
	if (!m_isExist)	return;
	int img = m_idx * kLeftRightSize;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		img, 0, kLeftRightSize, kLeftRightSize, kDrawScale * Game::kScale, 0.0f, m_handle, true, m_isLeft);
	if (m_isOnDamage)
	{
		//ダメージを受けたとき点滅させる
		SetDrawBlendMode(DX_BLENDMODE_ADD, 255);//加算合成
		my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
			img, 0, kLeftRightSize, kLeftRightSize, kDrawScale * Game::kScale, 0.0f, m_handle, true, m_isLeft);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//もとに戻す
		m_isOnDamage = false;
}
#ifdef _DEBUG
	m_rect.Draw(0xff00ff);
#endif
}

void EnemyMoveLR::BurstUpdate()
{
	m_idx++;
	if (m_idx == kBurstAnimNum * kBurstAnimSpeed)
	{
		m_isExist = false;
	}
}

void EnemyMoveLR::BurstDraw()
{
	int imgX = (m_idx / kBurstAnimSpeed) * kBurstImgWidth;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		imgX, 0, kBurstImgWidth, kBurstImgHeight, kBurstDrawScale * Game::kScale, 0.0f, m_burstHandle, true, false);
}

void EnemyMoveLR::MoveUpdate()
{
	//目が開いている画像
	m_idx = 2;
	if (m_isLeft)
	{
		//左に移動する
		m_rect.center.x -= kEnemyMoveSpeed;
	}
	else
	{
		//右に移動
		m_rect.center.x += kEnemyMoveSpeed;
	}

	//壁にぶつかった時
	if (m_chipId == 1)
	{
		m_chipId = 0;
		//半目
		m_idx = 1;
		//向きを逆に
		m_isLeft = !m_isLeft;
		//通常の更新に切り替える
		m_updateFunc = &EnemyMoveLR::NormalUpdate;
	}
}
