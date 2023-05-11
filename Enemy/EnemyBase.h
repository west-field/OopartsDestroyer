#pragma once
#include "../Game/Object.h"
#include <memory>
#include "../Util/Geometry.h"

class Player;			//�v���C���[
class HpBar;			//HP�o�[
class ShotFactory;		//�V���b�g
class ItemFactory;		//�A�C�e��

/// <summary>
/// �G���N���X
/// </summary>
class EnemyBase : public Object
{
public:
	EnemyBase(std::shared_ptr<Player>player,const Position2 pos, int handle, int burstH,
		std::shared_ptr<ShotFactory> sFactory, std::shared_ptr<ItemFactory> itFactory);
	virtual ~EnemyBase();

	virtual void Update() = 0;	//�X�V
	virtual void Draw() = 0;	//�\��

	/// <summary>
	/// �����蔻��Ώۂ��ǂ���
	/// </summary>
	/// <returns>true:������ false:������Ȃ�</returns>
	virtual bool IsCollidable()const = 0;

	/// <summary>
	/// �_���[�W���󂯂�
	/// </summary>
	/// <param name="damage">�_���[�W��</param>
	virtual void Damage(int damage) = 0;

	/// <summary>
	/// �ڐG�������̍U��
	/// </summary>
	/// <returns>�ڐG�������̍U����</returns>
	virtual int TouchAttackPower()const;


	/// <summary>
	/// �ړ��ʂ�Ԃ�
	/// </summary>
	/// <returns>�ړ�</returns>
	Vector2 GetVec()const;

	/// <summary>
	/// ���݂�HP��Ԃ�
	/// </summary>
	/// <returns>���݂�HP</returns>
	int GetHp() const;

	/// <summary>
	/// �}�b�v�`�b�v����肷��
	/// </summary>
	/// <param name="chipId">�}�b�v�`�b�v</param>
	void GetChip(int chipId);
protected:
	std::shared_ptr<Player> m_player;				//�v���C���[
	std::shared_ptr<ShotFactory> m_shotFactory;		//�V���b�g
	std::shared_ptr<ItemFactory> m_itemFactory;		//�A�C�e��

	Vector2 m_vec;			//�ړ����x

	int m_burstHandle;		//�����摜�n���h��

	int m_chipId;			//�}�b�v�`�b�vID
	bool m_isOnDamage;		//�_���[�W���󂯂����ǂ���

	int m_touchDamagePower;	//�����������̍U����
};

