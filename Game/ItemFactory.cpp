#include "ItemFactory.h"
#include <DxLib.h>
#include <algorithm>
#include "../game.h"
#include "../Util/DrawFunctions.h"
#include "../Item/ItemBase.h"
#include "../Item/HealItem.h"

ItemFactory::ItemFactory()
{
	m_handleMap[ItemType::Heal] = my::MyLoadGraph(L"Data/Card.png");
}

ItemFactory::~ItemFactory()
{
}

void ItemFactory::Update()
{
	//とったアイテムは消える
	auto rmIt = std::remove_if(m_items.begin(), m_items.end(),
		[](const std::shared_ptr<ItemBase>& item) {
			return !item->IsExist();
		});
	m_items.erase(rmIt, m_items.end());

	for (auto& item : m_items)
	{
		if (item->IsExist())
		{
			item->Update();
		}
	}
}

void ItemFactory::Draw(Vector2 vel)
{
	for (auto& item : m_items)
	{
		if (item->IsExist())
		{
			item->Draw(vel);
		}
	}
}

//アイテムを生成
std::shared_ptr<ItemBase> ItemFactory::Create(ItemType type, const Position2 pos)
{
	switch (type)
	{
	case ItemType::Heal:
		m_items.push_back(std::make_shared<HealItem>(pos, m_handleMap[ItemType::Heal]));
		break;
	default:
		break;
	}
	return m_items.back();
}

//全てのアイテムを取得する
std::list<std::shared_ptr<ItemBase>>& ItemFactory::GetItems()
{
	return m_items;
}
