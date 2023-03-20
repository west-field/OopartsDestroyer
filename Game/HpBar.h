#pragma once
#include "../Util/Geometry.h"

class HpBar
{
public:
	HpBar(Position2 pos);
	virtual ~HpBar();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="handle">�摜�n���h��</param>
	void Init(int handle);
	
	/// <summary>
	/// �ő�HP���w�肷��
	/// </summary>
	/// <param name="maxHp">�ő�HP</param>
	void MaxHp(int maxHp);
	/// <summary>
	/// �X�V�v���C���[
	/// </summary>
	void UpdatePlayer();
	/// <summary>
	/// �X�V�G�l�~�[
	/// </summary>
	void UpdateBoss();
	/// <summary>
	/// �\��
	/// </summary>
	void Draw();

	/// <summary>
	/// HP���擾����
	/// </summary>
	/// <returns>���݂�HP</returns>
	int GetHp() const { return m_Hp; }
	/// <summary>
	/// �ő�HP���擾����
	/// </summary>
	/// <returns>�ő�HP</returns>
	int GetMaxHp()const { return m_MaxHp; }
	/// <summary>
	/// �_���[�W�v�Z
	/// </summary>
	/// <param name="damage">�󂯂��_���[�W��</param>
	void Damage(int damage);
	/// <summary>
	/// �񕜌v�Z
	/// </summary>
	/// <param name="heal">�񕜗�</param>
	void Heal(const int heal);
private:
	/// <summary>
	/// HP���ő�l�܂���0�𒴂��Ă��Ȃ���
	/// </summary>
	/// <param name="hp">�v�Z����HP</param>
	virtual void SetHp(const int hp);

	int m_MaxHp;//�ő�HP
	int m_Hp;//���݂�HP
	int m_HpHandle;	//HP�̉摜

	Rect m_rect;

	int idxX = 0;
	int idxY = 0;
};

