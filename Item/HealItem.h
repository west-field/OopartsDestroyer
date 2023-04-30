#pragma once
#include "ItemBase.h"
class HealItem :
    public ItemBase
{
public:
    HealItem(const Position2& pos,int handle);
    ~HealItem();

    ///更新
    virtual void Update()override;
    /// <summary>
    /// 表示
    /// </summary>
    /// <param name="vel">画面移動</param>
    virtual void Draw(Vector2 vel)override;

    /// <summary>
    /// 回復する量を返す
    /// </summary>
    /// <returns>回復量</returns>
    virtual int GetHeal()const;
private:
    int m_idx;//画像インデックス
};

