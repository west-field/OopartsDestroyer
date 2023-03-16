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

    //‰ñ•œ—Ê
    virtual int GetHeal()const;
private:
    int add = 1;
};

