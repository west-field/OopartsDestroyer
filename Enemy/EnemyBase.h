#pragma once
#include <memory>
#include "../Util/Geometry.h"

class Player;			//�v���C���[
class HpBar;			//HP�o�[
class ShotFactory;		//�V���b�g
class ItemFactory;		//�A�C�e��

/// <summary>
/// �G���N���X
/// </summary>
class EnemyBase
{
public:
	EnemyBase(std::shared_ptr<Player>player,const Position2& pos, int handle, int burstH,
		std::shared_ptr<ShotFactory> sFactory, std::shared_ptr<ItemFactory> itFactory);
	virtual ~EnemyBase();

	virtual void Update() = 0;	//�X�V
	virtual void Draw() = 0;	//�\��

	/// <summary>
	/// �G�l�~�[�ړ�
	/// </summary>
	/// <param name="vec">�ړ���</param>
	void Movement(Vector2 vec);

	/// <summary>
	/// ��`���擾
	/// </summary>
	/// <returns>��`�\����</returns>
	const Rect& GetRect() const;

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
	/// ���̓G�͐����Ă��邩
	/// </summary>
	/// <returns>true:�����Ă���@false:�����Ă��Ȃ�</returns>
	bool IsExist()const;

	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="isExist">���݂�����</param>
	void EraseExist();

	/// <summary>
	/// ���������Ă��邩�ǂ�����Ԃ�
	/// </summary>
	/// <returns>true:���������Ă���@false:�E�������Ă���</returns>
	bool IsLeft()const;

	/// <summary>
	/// ���������Ă��邩�ǂ��������
	/// </summary>
	/// <param name="isLeft">true:���������Ă���@false:�E�������Ă���</param>
	void SetLeft(bool isLeft);

	/// <summary>
	/// �W�����v���Ă��邩�ǂ�����Ԃ�
	/// </summary>
	/// <returns>true:�W�����v���Ă���@false:�W�����v���Ă��Ȃ�</returns>
	bool IsJump()const;

	/// <summary>
	/// �W�����v�����Ă��邩���Ă��Ȃ��������߂�
	/// </summary>
	/// <param name="isJump">true:�W�����v���Ă���@false:�W�����v���Ă��Ȃ�</param>
	void SetJump(bool isJump);

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
	std::shared_ptr<HpBar> m_hp;					//Hp�o�[
	std::shared_ptr<ShotFactory> m_shotFactory;		//�V���b�g
	std::shared_ptr<ItemFactory> m_itemFactory;		//�A�C�e��

	bool m_isExist;			//���݂��邩
	Rect m_rect;			//�G�̋�`

	Vector2 m_vec;			//�ړ����x

	int m_handle;			//�摜�n���h��
	int m_burstHandle;		//�����摜�n���h��

	int m_idx;				//�摜�ύX

	bool m_isLeft;			//���������Ă���

	bool m_isJump;			//�W�����v�����Ă��邩

	int m_chipId;			//�}�b�v�`�b�vID
	bool m_isOnDamage;		//�_���[�W���󂯂����ǂ���

	int m_touchDamagePower;	//�������������̍U����
};

