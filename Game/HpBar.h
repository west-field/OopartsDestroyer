#pragma once
#include "../Util/Geometry.h"

class HpBar
{
public:
	HpBar();
	virtual ~HpBar();

	void Init(int handle);
	
	void MaxHp(int maxHp);
	
	void Update();
	void Draw(bool isPlayer);

	//HP���擾����
	int GetHp() const { return m_Hp; }
	//�ő�HP���擾����
	int GetMaxHp()const { return m_MaxHp; }
	//�_���[�W�v�Z
	void Damage(int damage);
	//�񕜌v�Z
	void Heal(const int heal);
private:
	virtual void SetHp(const int hp);

	static constexpr int kHpMax = 20;

	int m_MaxHp;
	//���݂�HP
	int m_Hp;
	//HP�̉摜
	int m_HpHandle;

	//HP�摜�\�����W
	//�摜�̕��ƍ���
	Rect m_rect;

	int idxX = 0;
	int idxY = 0;
};

