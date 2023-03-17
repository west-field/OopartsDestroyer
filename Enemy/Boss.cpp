#include "Boss.h"
#include <DxLib.h>
#include "../Util/DrawFunctions.h"
#include "../Game/ShotFactory.h"
#include "../Game/Player.h"
#include "../Game/HpBar.h"
#include "../game.h"
#include "../Util/Sound.h"
namespace
{
	constexpr int kCutManTouchAttackPower = 4;//接触した時の攻撃力
	
	//ボスアニメーション
	constexpr int kGraphNum = 13;	//ボスグラフィックの数
	constexpr int kGraphSizeWidth = 2600 / kGraphNum;//ボスグラフィックサイズ
	constexpr int kGraphSizeHeight = 400;	//ボスグラフィックサイズ
	constexpr float kDrawScale = 0.5f;		//ボスグラフィック拡大率
	constexpr int kAnimFrameSpeed = 5;	//ボスグラフィックアニメーションスピード

	//通常爆発アニメーション
	constexpr int burst_img_width = 32;//画像サイズX
	constexpr int burst_img_height = 32;//画像サイズY
	constexpr float burst_draw_scale = 1.0f;//拡大率
	constexpr int burst_frame_num = 8;//アニメーション枚数
	constexpr int burst_frame_speed = 5;//アニメーションスピード
	//ボス爆発アニメーション
	constexpr int boss_burst_img_width = 100;//画像サイズX
	constexpr int boss_burst_img_height = 100;//画像サイズY
	constexpr float boss_burst_draw_scale = 2.0f;//拡大率
	constexpr int boss_burst_frame_num = 61;//アニメーション枚数
	constexpr int boss_burst_frame_speed = 1;//アニメーションスピード
}

Boss::Boss(std::shared_ptr<Player>player, const Position2& pos, int handle, int bossBurstH, int burstH, std::shared_ptr<ShotFactory> sFactory, std::shared_ptr<ItemFactory> itFactory, std::shared_ptr<HpBar>hp) :
	EnemyBase(player, pos, sFactory,itFactory), m_updateFunc(&Boss::NormalUpdate), m_drawFunc(&Boss::NormalDraw),
	m_shotFrame(0)
{
	m_hp = hp;
	m_isLeft = true;
	m_handle = handle;
	m_bossBurstH = bossBurstH;
	m_burstHandle = burstH;
	m_rect = { {pos.x,pos.y - 8.0f},
		{static_cast<int>(kGraphSizeWidth * Game::kScale * kDrawScale / 2) - 20,static_cast<int>(kGraphSizeHeight * Game::kScale * kDrawScale) - 20} };
}

Boss::~Boss()
{
	DeleteGraph(m_bossBurstH);
}

void Boss::Update()
{
	(this->*m_updateFunc)();
}

void Boss::Draw()
{
	(this->*m_drawFunc)();
}

int Boss::TouchAttackPower() const
{
	return kCutManTouchAttackPower;
}

void Boss::Damage(int damage)
{
	m_hp->Damage(damage);

	if (m_hp->GetHp() == 0)
	{
		SoundManager::GetInstance().Play(SoundId::EnemyBurst);
		m_updateFunc = &Boss::BurstUpdate;
		m_drawFunc = &Boss::BurstDraw;
		m_idx = 0;
		return;
	}
	SoundManager::GetInstance().Play(SoundId::EnemyHit);
}

bool Boss::IsCollidable() const
{
	//BurstUpdateでない＆無敵時間が0の時当たる
	return (m_updateFunc != &Boss::BurstUpdate) && m_ultimateTimer == 0;
}

void Boss::NormalUpdate()
{
	//アニメーション
	if (m_animFrame++ > kAnimFrameSpeed)
	{
		m_idx = (m_idx + 1) % kGraphNum;
		m_animFrame = 0;
	}
	//無敵時間がある時は時間を減らしていく
	if (--m_ultimateTimer <= 0)
	{
		m_ultimateTimer = 0;
	}

	//1秒ごとに弾を打つ
	if (m_frame++ == 60)
	{
		m_frame = 0;
		m_isOnDamage = false;

		//HPが3分の2を切ったら二回攻撃する
		if (m_hp->GetHp() <= (m_hp->GetMaxHp() / 3) * 2)
		{
			m_shotFrame = 20;
		}
		else
		{
			m_shotFrame = 10;
		}
		m_updateFunc = &Boss::ShotUpdate;
	}
}

void Boss::NormalDraw()
{
	//無敵時間点滅させる
	if ((m_ultimateTimer / 10) % 2 == 1)	return;
	//ボス表示
	int img = m_idx * kGraphSizeWidth;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		img, 0, kGraphSizeWidth, kGraphSizeHeight, kDrawScale * Game::kScale, 0.0f, m_handle, true, m_isLeft);
#ifdef _DEBUG
	DrawFormatString(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y), 0xffffff, L"%d", m_idx);
	m_rect.Draw(0xaaffaa);
#endif
}

void Boss::BurstUpdate()
{
	//爆発アニメーション
	m_idx += 1;
	if (m_idx == (boss_burst_frame_num * 2) * boss_burst_frame_speed)
	{
		//爆発アニメーションが終わったら存在を消す
		m_isExist = false;
	}
}

void Boss::BurstDraw()
{
	//通常爆発エフェクトを重ねて表示
	int animNum = (m_idx / burst_frame_speed);
	int imgX = (animNum % burst_frame_num) * burst_img_width;
	int imgY = 0;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x + 20), static_cast<int>(m_rect.center.y + 50),
		imgX, imgY, burst_img_width, burst_img_height, burst_draw_scale * Game::kScale, 0.0f, m_burstHandle, true, false);
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x - 40), static_cast<int>(m_rect.center.y + 50),
		imgX, imgY, burst_img_width, burst_img_height, burst_draw_scale * Game::kScale, 0.0f, m_burstHandle, true, false);
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x + 20), static_cast<int>(m_rect.center.y - 50),
		imgX, imgY, burst_img_width, burst_img_height, burst_draw_scale * Game::kScale, 0.0f, m_burstHandle, true, false);
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x - 40), static_cast<int>(m_rect.center.y - 50),
		imgX, imgY, burst_img_width, burst_img_height, burst_draw_scale * Game::kScale, 0.0f, m_burstHandle, true, false);

	//ボス爆発エフェクトを表示
	animNum = (m_idx / boss_burst_frame_speed);
	if (animNum >= boss_burst_frame_num)
	{
		animNum -= boss_burst_frame_num;
	}
	imgX = animNum % 8 * boss_burst_img_width;
	imgY = animNum / 8 * boss_burst_img_height;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		imgX, imgY, boss_burst_img_width, boss_burst_img_height, boss_burst_draw_scale * Game::kScale, 0.0f, m_bossBurstH, true, false);
#ifdef _DEBUG
	DrawFormatStringF(m_rect.center.x, m_rect.center.y, 0x000000, L"%d", animNum);
	DrawFormatStringF(m_rect.center.x, m_rect.center.y - 20, 0x000000, L"X%d,Y%d", imgX / boss_burst_img_width, imgY / boss_burst_img_height);
#endif
}

void Boss::ShotUpdate()
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
	}
	//ショットを打つ
	if (m_shotFrame-- % 10 == 0)
	{
		m_shotFactory->Create(ShotType::ShotBattery, m_rect.center, vel, m_isLeft, false);
	}

	if (m_shotFrame <= 0)
	{
		//次の指示を待つ
		m_updateFunc = &Boss::NormalUpdate;
		return;
	}
}
