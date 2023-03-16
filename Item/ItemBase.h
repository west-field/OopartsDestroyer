#pragma once
#include "../Util/Geometry.h"

class ItemBase
{
public:
	ItemBase(const Position2& pos);
	~ItemBase();
	virtual void Update() = 0;
	virtual void Draw(Vector2 vel) = 0;
	//矩形を取得
	const Rect& GetRect() const;
	//存在しているかを返す
	bool IsExist()const;
	//消えたかどうか
	void SetExist(bool isExist);
	//回復量
	virtual int GetHeal()const = 0;
protected:
	//画像ハンドル
	int m_handle = 1;
	//矩形
	Rect m_rect;
	//存在しているかどうか
	bool m_isExist = true;
	//加算合成用
	int m_addBlend = 0;
};

