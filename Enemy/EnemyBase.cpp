#include "EnemyBase.h"
#include "../Game/HpBar.h"
#include "../Util/Sound.h"

EnemyBase::EnemyBase(std::shared_ptr<Player>player, const Position2& pos, std::shared_ptr<ShotFactory> sFactory, std::shared_ptr<ItemFactory> itFactory):
    m_player(player), m_rect(pos, {}), m_vec(), m_shotFactory(sFactory),m_itemFactory(itFactory)
{
    m_hp = std::make_shared<HpBar>();
}

EnemyBase::~EnemyBase()
{
}

void EnemyBase::Movement(Vector2 vec)
{
    //画面と一緒に移動
    m_rect.center += vec;
}

const Rect& EnemyBase::GetRect() const
{
    return m_rect;
}

bool EnemyBase::IsExist() const
{
    return m_isExist;
}

int EnemyBase::GetHp() const
{
    return m_hp->GetHp();
}
