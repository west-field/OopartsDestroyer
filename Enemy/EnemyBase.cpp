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

/// �G�l�~�[�ړ�
void EnemyBase::Movement(Vector2 vec)
{
    //��ʂƈꏏ�Ɉړ�
    m_rect.center += vec;
}

/// ��`���擾
const Rect& EnemyBase::GetRect() const
{
    return m_rect;
}

/// �ڐG�������̍U��
int EnemyBase::TouchAttackPower() const
{
    return m_touchDamagePower;
}

/// ���̓G�͐����Ă��邩
bool EnemyBase::IsExist() const
{
    return m_isExist;
}

/// ��������
void EnemyBase::EraseExist()
{
    m_isExist = false;
}

/// ���������Ă��邩�ǂ�����Ԃ�
bool EnemyBase::IsLeft() const
{
    return m_isLeft;
}

/// ���������Ă��邩�ǂ��������
void EnemyBase::SetLeft(bool isLeft)
{
    m_isLeft = isLeft;
}

/// �W�����v���Ă��邩�ǂ�����Ԃ�
bool EnemyBase::IsJump() const
{
    return m_isJump;
}

/// �W�����v�����Ă��邩���Ă��Ȃ��������߂�
void EnemyBase::SetJump(bool isJump)
{
    m_isJump = isJump; 
    m_vec = {};
}

/// �ړ��ʂ�Ԃ�
Vector2 EnemyBase::GetVec() const
{
    return m_vec;
}

/// ���݂�HP��Ԃ�
int EnemyBase::GetHp() const
{
    return m_hp->GetHp();
}

/// �}�b�v�`�b�v����肷��
void EnemyBase::GetChip(int chipId)
{
    m_chipId = chipId;
}
