#include "EnemyMoveLR.h"
#include <DxLib.h>
#include "../Util/DrawFunctions.h"
#include "../Game/HpBar.h"
#include "../game.h"
#include "../Util/Sound.h"
#include "../Game/ItemFactory.h"

namespace
{
	constexpr int kLeftRightTouchAttackPower = 4;//接触した時の攻撃力

	//エネミーアニメーション
	constexpr int anim_frame_speed = 5;//一枚に必要なフレーム数
	constexpr int anim_frame_num = 3;//アニメーション枚数
	constexpr int kLeftRightSize = 32;//グラフィック1つの大きさ
	constexpr float kDrawScall = 1.0f;//グラフィック拡大率
	constexpr float kEnemyMoveSpeed = 4.0f;//エネミーの移動速度

	//爆発アニメーション
	constexpr int burst_img_width = 32;//画像サイズX
	constexpr int burst_img_height = 32;//画像サイズY
	constexpr float burst_draw_scale = 1.0f;//拡大率
	constexpr int burst_frame_num = 8;//アニメーション枚数
	constexpr int burst_frame_speed = 5;//アニメーションスピード
}

EnemyMoveLR::EnemyMoveLR(std::shared_ptr<Player> player, const Position2 pos, int handle, int burstH, std::shared_ptr<ShotFactory> sFactory, std::shared_ptr<ItemFactory> itFactory) :
	EnemyBase(player, pos, sFactory,itFactory), m_updateFunc(&EnemyMoveLR::NormalUpdate),m_drawFunc(&EnemyMoveLR::NormalDraw)
{
	m_handle = handle;
	m_burstHandle = burstH;
	m_rect = { pos,{static_cast<int>(kLeftRightSize * Game::kScale),static_cast<int>(kLeftRightSize * Game::kScale)} };
	m_hp->MaxHp(3);

	if (GetRand(100) / 2 == 0)
	{
		m_isLeft = true;
	}
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

int EnemyMoveLR::TouchAttackPower() const
{
	return kLeftRightTouchAttackPower;
}

void EnemyMoveLR::Damage(int damage)
{
	m_hp->Damage(damage);
	
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
		img, 0, kLeftRightSize, kLeftRightSize, kDrawScall * Game::kScale, 0.0f, m_handle, true, m_isLeft);
#ifdef _DEBUG
	m_rect.Draw(0xff00ff);
#endif
}

void EnemyMoveLR::BurstUpdate()
{
	m_idx++;
	if (m_idx == burst_frame_num * burst_frame_speed)
	{
		m_isExist = false;
	}
}

void EnemyMoveLR::BurstDraw()
{
	int imgX = (m_idx / burst_frame_speed) * burst_img_width;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		imgX, 0, burst_img_width, burst_img_height, burst_draw_scale * Game::kScale, 0.0f, m_burstHandle, true, false);
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
