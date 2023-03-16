#include "ItemFactory.h"
#include <DxLib.h>
#include <algorithm>
#include "../game.h"
#include "../Util/DrawFunctions.h"
#include "../Item/ItemBase.h"
#include "../Item/HealItem.h"

ItemFactory::ItemFactory()
{
	//m_handleMap[ItemType::Heal] = my::MyLoadGraph(L"Data/heal.png");
	m_handleMap[ItemType::Heal] = my::MyLoadGraph(L"Data/Card.png");
}

ItemFactory::~ItemFactory()
{
}

void ItemFactory::Update()
{
	//���Ȃ��Ȃ����G�͏�����
	//�����ɓ��Ă͂܂�G�����ɂ悯�Ă��� 
	//remove_if�����ɍ��v�������̂����� begin,end �Ώۂ�enemise_�̍ŏ�����Ō�܂� �����Ă��炤������\�������_�� true������false���Ə����Ȃ�
	auto rmIt = std::remove_if(m_items.begin(), m_items.end(),
		[](const std::shared_ptr<ItemBase>& item) {
			return !item->IsExist();
		});
	//remove�n�̊֐��͕s�v�Ȃ��̂����ɂ悯�āA�悯���ꏊ���C�e���[�^�Ƃ��ĕԂ��B

	//���ۂɔ͈͂��w�肵�ď����Ă���
	m_items.erase(rmIt, m_items.end());

	for (auto& item : m_items)
	{
		if (item->IsExist() && item->GetRect().GetCenter().x < Game::kMapScreenRightX + item->GetRect().GetSize().w / 2)
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

std::list<std::shared_ptr<ItemBase>>& ItemFactory::GetItems()
{
	return m_items;
}
