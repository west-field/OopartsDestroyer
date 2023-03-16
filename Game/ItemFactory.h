#pragma once

#include <memory>
#include <list>
#include <map>
#include "../Util/Geometry.h"

class ItemBase;

enum class ItemType
{
	Heal,

	Max
};

class ItemFactory
{
public:
	ItemFactory();
	~ItemFactory();

	void Update();
	void Draw(Vector2 vel);

	std::shared_ptr<ItemBase> Create(ItemType type, const Position2 pos);
	std::list<std::shared_ptr<ItemBase>>& GetItems();

private:
	std::list<std::shared_ptr<ItemBase>> m_items;

	std::map<ItemType, int>m_handleMap;
};

