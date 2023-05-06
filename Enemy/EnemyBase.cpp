#include "EnemyBase.h"
#include "../Game/HpBar.h"
#include "../Util/Sound.h"

EnemyBase::EnemyBase(std::shared_ptr<Player>player, const Position2& pos, int handle, int burstH, std::shared_ptr<ShotFactory> sFactory, std::shared_ptr<ItemFactory> itFactory):
    m_player(player), m_shotFactory(sFactory), m_itemFactory(itFactory),
    m_rect(pos, {}), m_isExist(true), m_vec(), m_handle(handle), m_burstHandle(burstH), m_idx(0),
    m_isLeft(false), m_isJump(false), m_chipId(0), m_isOnDamage(false), m_touchDamagePower(1),m_isBoss(false)
{
    m_hp = std::make_shared<HpBar>(Position2{ 0.0f,0.0f });
}

EnemyBase::~EnemyBase()
{
}

/// エネミー移動
void EnemyBase::Movement(Vector2 vec)
{
    //画面と一緒に移動
    m_rect.center += vec;
}

/// 矩形を取得
const Rect& EnemyBase::GetRect() const
{
    return m_rect;
}

/// 接触した時の攻撃
int EnemyBase::TouchAttackPower() const
{
    return m_touchDamagePower;
}

/// この敵は生きているか
bool EnemyBase::IsExist() const
{
    return m_isExist;
}

/// 消すため
void EnemyBase::EraseExist()
{
    m_isExist = false;
}

/// 左を向いているかどうかを返す
bool EnemyBase::IsLeft() const
{
    return m_isLeft;
}

/// 左を向いているかどうかを入力
void EnemyBase::SetLeft(bool isLeft)
{
    m_isLeft = isLeft;
}

/// ジャンプしているかどうかを返す
bool EnemyBase::IsJump() const
{
    return m_isJump;
}

/// ジャンプをしているかしていないかを決める
void EnemyBase::SetJump(bool isJump)
{
    m_isJump = isJump; 
    m_vec = {};
}

/// 移動量を返す
Vector2 EnemyBase::GetVec() const
{
    return m_vec;
}

/// 現在のHPを返す
int EnemyBase::GetHp() const
{
    return m_hp->GetHp();
}

/// マップチップを入手する
void EnemyBase::GetChip(int chipId)
{
    m_chipId = chipId;
}
