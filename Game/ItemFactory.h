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
	/// �X�V
	/// </summary>
	void Update();
	/// <summary>
	/// �\��
	/// </summary>
	/// <param name="vel">��ʈړ���</param>
	void Draw(Vector2 vel);

	/// <summary>
	/// �A�C�e�����쐬
	/// </summary>
	/// <param name="type">�A�C�e���̎��</param>
	/// <param name="pos">�����ʒu</param>
	/// <returns>�A�C�e��</returns>
	std::shared_ptr<ItemBase> Create(ItemType type, const Position2 pos);
	/// <summary>
	/// ���ׂẴA�C�e�����擾����
	/// </summary>
	/// <returns>�S�A�C�e��</returns>
	std::list<std::shared_ptr<ItemBase>>& GetItems();

private:
	std::list<std::shared_ptr<ItemBase>> m_items;//�A�C�e��

	std::map<ItemType, int>m_handleMap;//�A�C�e���摜�n���h��
};

