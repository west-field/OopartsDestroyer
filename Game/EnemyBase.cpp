#include "EnemyBase.h"

EnemyBase::EnemyBase(std::shared_ptr<Player>player, const Position2& pos, std::shared_ptr<ShotFactory> sFactory):
    m_player(player), m_rect(pos, {}),m_shotFactory(sFactory)
{
}

EnemyBase::~EnemyBase()
{
}

const Rect& EnemyBase::GetRect() const
{
    return m_rect;
}

bool EnemyBase::IsExist() const
{
    return m_isExist;
}
