#include "EnemyMoveUD.h"
#include <DxLib.h>
#include "../Util/DrawFunctions.h"
#include "../Game/HpBar.h"
#include "../game.h"

namespace
{
	constexpr int kLeftRightTouchAttackPower = 4;//接触した時の攻撃力

	constexpr int anim_frame_speed = 5;//一枚に必要なフレーム数
	constexpr int anim_frame_num = 3;//アニメーション枚数

	constexpr int kLeftRightSize = 32;//グラフィック1つの大きさ
	constexpr float kDrawScall = 1.0f;//グラフィック拡大率

	constexpr float kEnemyMoveSpeed = 4.0f;//エネミーの移動速度

	constexpr int burst_img_width = 32;//画像サイズX
	constexpr int burst_img_height = 32;//画像サイズY
	constexpr float burst_draw_scale = 1.0f;//拡大率
	constexpr int burst_frame_num = 8;//アニメーション枚数
	constexpr int burst_frame_speed = 5;//アニメーションスピード
}

EnemyMoveUD::EnemyMoveUD(std::shared_ptr<Player> player, const Position2 pos, int handle, int burstH, std::shared_ptr<ShotFactory> sFactory) :
	EnemyBase(player, pos, sFactory), m_updateFunc(&EnemyMoveUD::NormalUpdate),m_drawFunc(&EnemyMoveUD::NormalDraw)
{
	m_handle = handle;
	m_burstHandle = burstH;
	m_rect = { pos,{static_cast<int>(kLeftRightSize * Game::kScale),static_cast<int>(kLeftRightSize * Game::kScale)} };
	m_hp->MaxHp(3);
}

EnemyMoveUD::~EnemyMoveUD()
{

}

void EnemyMoveUD::Update()
{
	if (!m_isExist)	return;
	
	(this->*m_updateFunc)();
}

void EnemyMoveUD::Draw()
{
	(this->*m_drawFunc)();
}

void EnemyMoveUD::Movement(Vector2 vec)
{
	if (!m_isExist)	return;

	m_rect.center += vec;
}

int EnemyMoveUD::TouchAttackPower() const
{
	return kLeftRightTouchAttackPower;
}

void EnemyMoveUD::Damage(int damage)
{
	m_hp->Damage(damage);
	//m_ultimateTimer = kUltimateFrame;//無敵時間
	if (m_hp->GetHp() == 0)
	{
		m_updateFunc = &EnemyMoveUD::BurstUpdate;
		m_drawFunc = &EnemyMoveUD::BurstDraw;
		m_idx = 0;
	}
}

bool EnemyMoveUD::IsCollidable() const
{
	return (m_updateFunc != &EnemyMoveUD::BurstUpdate);
}

void EnemyMoveUD::NormalUpdate()
{
	//2秒間ぐらい止まる
	if (m_frame++ >= 120)
	{
		//半目
		m_idx = 1;
		//フレームカウントをゼロにする
		m_frame = 0;
		//どっちを向いているかどうか
		if (m_isLeft)
		{
			m_updateFunc = &EnemyMoveUD::UpUpdate;
		}
		else
		{
			m_updateFunc = &EnemyMoveUD::DownUpdate;
		}
	}
	//止まっている間は目を閉じた画像を表示させる
	else
	{
		m_idx = 0;
	}
}

void EnemyMoveUD::UpUpdate()
{
	//目が開いている画像
	m_idx = 2;
	//上に移動する
	m_rect.center.y -= kEnemyMoveSpeed;
	//壁にぶつかった時
	if (m_chipId != 0)
	{
		m_chipId = 0;
		//半目
		m_idx = 1;
		//向きを逆に
		m_isLeft = false;
		//通常の更新に切り替える
		m_updateFunc = &EnemyMoveUD::NormalUpdate;
	}
}

void EnemyMoveUD::DownUpdate()
{
	m_idx = 2;
	//下に移動
	m_rect.center.y += kEnemyMoveSpeed;
	if (m_chipId != 0)
	{
		m_chipId = 0;
		m_idx = 1;
		m_isLeft = true;
		m_updateFunc = &EnemyMoveUD::NormalUpdate;
	}
}

void EnemyMoveUD::NormalDraw()
{
	if (!m_isExist)	return;
	int img = m_idx * kLeftRightSize;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		img, 0, kLeftRightSize, kLeftRightSize, kDrawScall * Game::kScale, 0.0f, m_handle, true, m_isLeft);
#ifdef _DEBUG
	m_rect.Draw(0xff00ff);
#endif
}

void EnemyMoveUD::BurstUpdate()
{
	m_idx++;
	if (m_idx == burst_frame_num * burst_frame_speed)
	{
		m_isExist = false;
	}
}

void EnemyMoveUD::BurstDraw()
{
	int imgX = (m_idx / burst_frame_speed) * burst_img_width;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		imgX, 0, burst_img_width, burst_img_height, burst_draw_scale * Game::kScale, 0.0f, m_burstHandle, true, false);
}
