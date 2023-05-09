#include "EnemyBase.h"
#include "../Game/HpBar.h"
#include "../Util/Sound.h"

EnemyBase::EnemyBase(std::shared_ptr<Player>player, const Position2 pos, int handle, int burstH, std::shared_ptr<ShotFactory> sFactory, std::shared_ptr<ItemFactory> itFactory):
    Object(), m_player(player), m_shotFactory(sFactory), m_itemFactory(itFactory),
    m_vec(), m_burstHandle(burstH),m_chipId(0), m_isOnDamage(false), m_touchDamagePower(1)
{
    m_rect.center = pos;
    m_handle = handle;
    m_hp = std::make_shared<HpBar>(Position2{ 0.0f,0.0f });
    m_type = ObjectType::Enemy;
}

EnemyBase::~EnemyBase()
{
}

/// 接触した時の攻撃
int EnemyBase::TouchAttackPower() const
{
    return m_touchDamagePower;
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
