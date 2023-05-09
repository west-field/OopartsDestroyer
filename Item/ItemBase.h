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
	//��`���擾
	const Rect& GetRect() const;
	//���݂��Ă��邩��Ԃ�
	bool IsExist()const;
	//���݂�����
	void EraseExist();
	
	//�񕜗�
	virtual int GetHeal()const = 0;

	//�A�C�e���̎�ނ�Ԃ�
	ItemType OnGet();

protected:
	
	int m_handle;//�摜�n���h��
	
	Rect m_rect;//��`

	bool m_isExist;	//���݂��Ă��邩�ǂ���

	int m_idx;//�摜�C���f�b�N�X

	ItemType m_itemType;//�ǂ̃A�C�e����

	int m_animSpeed;//�A�j���[�V�����X�s�[�h
	int m_animNum;//�A�j���[�V��������

	float m_drawScale;//�g�嗦
};

