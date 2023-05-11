#pragma once
#include "ItemBase.h"
class HealItem :
    public ItemBase
{
public:
    HealItem(const Position2& pos,int handle);
    ~HealItem();

    //•\Ž¦
    virtual void Draw(Vector2 vel);

    /// <summary>
    /// ‰ñ•œ‚·‚é—Ê‚ð•Ô‚·
    /// </summary>
    /// <returns>‰ñ•œ—Ê</returns>
    virtual int GetHeal()const override;
private:
    
};

