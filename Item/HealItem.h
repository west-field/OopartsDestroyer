#pragma once
#include "ItemBase.h"
class HealItem :
    public ItemBase
{
public:
    HealItem(const Position2& pos,int handle);
    ~HealItem();

    virtual void Update()override;
    virtual void Draw(Vector2 vel)override;

    //�񕜗�
    virtual int GetHeal()const;
private:
    //���Z�����p
    int m_addBlend = 0;
    int add = 1;

    int m_idx = 0;
};

