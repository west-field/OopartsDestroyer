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

	//�_���[�W�v�Z
	void Damage(int damage);
	//�񕜌v�Z
	void Heal(const int heal);
private:
	virtual void SetHp(const int hp);

	static constexpr int kHpMax = 28;

	int m_MaxHp;
	//���݂�HP
	int m_Hp;
	//HP�̉摜
	int m_HpHandle;

	//HP�摜�\�����W
	Position2 m_pos;
	//�摜�̕��ƍ���
	Size m_size;

	//HP�摜�������\�����邩
	bool m_isHpGraph[kHpMax];
};

