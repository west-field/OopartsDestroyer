#pragma once
#include "ItemBase.h"
class HealItem :
    public ItemBase
{
public:
    HealItem(const Position2& pos,int handle);
    ~HealItem();

    //�\��
    virtual void Draw(Vector2 vel);

    /// <summary>
    /// �񕜂���ʂ�Ԃ�
    /// </summary>
    /// <returns>�񕜗�</returns>
    virtual int GetHeal()const override;
private:
    
};

