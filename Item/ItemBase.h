#pragma once
#include "../Util/Geometry.h"
#include "../ItemInfo.h"

class ItemBase
{
public:
	ItemBase(const Position2& pos);
	~ItemBase();
	virtual void Update();
	virtual void Draw(Vector2 vel) = 0;
	//矩形を取得
	const Rect& GetRect() const;
	//存在しているかを返す
	bool IsExist()const;
	//存在を消す
	void EraseExist();
	
	//回復量
	virtual int GetHeal()const = 0;

	//アイテムの種類を返す
	ItemType OnGet();

protected:
	
	int m_handle;//画像ハンドル
	
	Rect m_rect;//矩形

	bool m_isExist;	//存在しているかどうか

	int m_idx;//画像インデックス

	ItemType m_itemType;//どのアイテムか

	int m_animSpeed;//アニメーションスピード
	int m_animNum;//アニメーション枚数

	float m_drawScale;//拡大率
};

