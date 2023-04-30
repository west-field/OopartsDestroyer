#pragma once
#include "ItemBase.h"
class HealItem :
    public ItemBase
{
public:
    HealItem(const Position2& pos,int handle);
    ~HealItem();

    ///�X�V
    virtual void Update()override;
    /// <summary>
    /// �\��
    /// </summary>
    /// <param name="vel">��ʈړ�</param>
    virtual void Draw(Vector2 vel)override;

    /// <summary>
    /// �񕜂���ʂ�Ԃ�
    /// </summary>
    /// <returns>�񕜗�</returns>
    virtual int GetHeal()const;
private:
    int m_idx;//�摜�C���f�b�N�X
};

