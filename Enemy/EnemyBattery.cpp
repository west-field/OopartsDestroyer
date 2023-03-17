#include "EnemyBattery.h"
#include <DxLib.h>
#include "../Util/DrawFunctions.h"
#include "../game.h"
#include "../Game/HpBar.h"
#include "../Game/ShotFactory.h"
#include "../Game/ItemFactory.h"
#include "../Game/Player.h"
#include "../Util/Sound.h"

namespace
{
	constexpr int kBatteryTouchAttackPower = 1;//接触した時の攻撃力

	//エネミーアニメーション
	constexpr int anim_frame_speed = 20;//一枚に必要なフレーム数
	constexpr int anim_frame_num = 5;//アニメーション枚数
	constexpr int kSize = 32;//大きさ
	constexpr float kDrawScall = 1.0f;//表示拡大率

	//爆発アニメーション
	constexpr int burst_img_width = 32;//画像サイズX
	constexpr int burst_img_height = 32;//画像サイズY
	constexpr float burst_draw_scale = 1.0f;//拡大率
	constexpr int burst_frame_num = 8;//アニメーション枚数
	constexpr int burst_frame_speed = 5;//アニメーションスピード
}

EnemyBattery::EnemyBattery(std::shared_ptr<Player>player, const Position2 pos, int handle, int burstH, std::shared_ptr<ShotFactory> sFactory, std::shared_ptr<ItemFactory> itFactory,bool isLeft):
	EnemyBase(player,pos,sFactory,itFactory),m_updateFunc(&EnemyBattery::NormalUpdate),m_drawFunc(&EnemyBattery::NormalDraw)
{
	m_handle = handle;
	m_burstHandle = burstH;
	//矩形とサイズ
	m_rect = { pos, { static_cast<int>(kSize * Game::kScale * kDrawScall),static_cast<int>(kSize * Game::kScale * kDrawScall) } };
	
	m_hp->MaxHp(1);//この敵のマックスHP
	m_isLeft = isLeft;
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

int EnemyBattery::TouchAttackPower() const
{
	return kBatteryTouchAttackPower;
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
	m_idx = (m_idx + (GetRand(10) % 3)) % (anim_frame_speed * anim_frame_num);
	//攻撃モーションの時弾を発射する
	if (m_idx / anim_frame_speed == 2 && m_num == 0)
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
	else if (m_idx / anim_frame_speed == 0)
	{
		m_num = 0;
	}
}

void EnemyBattery::NormalDraw()
{
	int imgX = (m_idx / anim_frame_speed) * kSize;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		imgX, 0, kSize, kSize, kDrawScall * Game::kScale, 0.0f, m_handle, true, m_isLeft);
#ifdef _DEBUG
	m_rect.Draw(0xff0000);
	DrawFormatString(0, 360, 0xffffff, L"グラフィック切り替え%d", m_idx / anim_frame_speed);
#endif
}

void EnemyBattery::BurstUpdate()
{
	m_idx++;
	if (m_idx == burst_frame_num * burst_frame_speed)
	{
		m_isExist = false;
	}
}

void EnemyBattery::BurstDraw()
{
	int imgX = (m_idx / burst_frame_speed) * burst_img_width;

	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		imgX, 0, burst_img_width, burst_img_height, burst_draw_scale * Game::kScale, 0.0f, m_burstHandle, true, false);
}
