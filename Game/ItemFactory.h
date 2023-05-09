#pragma once

#include <memory>
#include <list>
#include <map>
#include "../Util/Geometry.h"

class ItemBase;
#include "../ItemInfo.h"

class ItemFactory
{
public:
	ItemFactory();
	~ItemFactory();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 表示
	/// </summary>
	/// <param name="vel">画面移動量</param>
	void Draw(Vector2 vel);

	/// <summary>
	/// アイテムを作成
	/// </summary>
	/// <param name="type">アイテムの種類</param>
	/// <param name="pos">生成位置</param>
	/// <returns>アイテム</returns>
	std::shared_ptr<ItemBase> Create(ItemType type, const Position2 pos);
	/// <summary>
	/// すべてのアイテムを取得する
	/// </summary>
	/// <returns>全アイテム</returns>
	std::list<std::shared_ptr<ItemBase>>& GetItems();

private:
	std::list<std::shared_ptr<ItemBase>> m_items;//アイテム

	std::map<ItemType, int>m_handleMap;//アイテム画像ハンドル
};

