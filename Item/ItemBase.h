#pragma once
#include "../Util/Geometry.h"

class ItemBase
{
public:
	ItemBase(const Position2& pos);
	~ItemBase();
	virtual void Update() = 0;
	virtual void Draw(Vector2 vel) = 0;
	//��`���擾
	const Rect& GetRect() const;
	//���݂��Ă��邩��Ԃ�
	bool IsExist()const;
	//���������ǂ���
	void SetExist(bool isExist);
	//�񕜗�
	virtual int GetHeal()const = 0;
protected:
	//�摜�n���h��
	int m_handle = 1;
	//��`
	Rect m_rect;
	//���݂��Ă��邩�ǂ���
	bool m_isExist = true;
	//���Z�����p
	int m_addBlend = 0;
};

