#pragma once
#include <memory>
#include "../Util/Geometry.h"

class Player;			//�v���C���[
class HpBar;			//HP�o�[
class ShotFactory;		//�V���b�g

/// <summary>
/// �G���N���X
/// </summary>
class EnemyBase
{
public:
	EnemyBase(std::shared_ptr<Player>player,const Position2& pos,std::shared_ptr<ShotFactory> sFactory);
	virtual ~EnemyBase();

	virtual void Update() = 0;//�X�V
	virtual void Draw() = 0;//�\��

	//�G�l�~�[�ړ�
	virtual void Movement(Vector2 vec) = 0;
	//��`���擾
	const Rect& GetRect() const;

	/// <summary>
	/// �ڐG�������̍U��
	/// </summary>
	/// <returns>�ڐG�������̍U����</returns>
	virtual int TouchAttackPower()const = 0;
	/// <summary>
	/// �_���[�W���󂯂�
	/// </summary>
	/// <param name="damage">�_���[�W��</param>
	void Damage(int damage);
	/// <summary>
	/// ���̓G�͐����Ă��邩
	/// </summary>
	/// <returns>true:�����Ă���@false:�����Ă��Ȃ�</returns>
	bool IsExist()const;
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="isExist">���݂�����</param>
	void SetExist(bool isExist) { m_isExist = isExist; }

	/// <summary>
	/// ���������Ă��邩�ǂ�����Ԃ�
	/// </summary>
	/// <returns>true:���������Ă���@false:�E�������Ă���</returns>
	bool IsLeft()const { return m_isLeft; }
	/// <summary>
	/// ���������Ă��邩�ǂ��������
	/// </summary>
	/// <param name="isLeft">true:���������Ă���@false:�E�������Ă���</param>
	void SetLeft(bool isLeft) { m_isLeft = isLeft; }

	/// <summary>
	/// �W�����v���Ă��邩�ǂ�����Ԃ�
	/// </summary>
	/// <returns>true:�W�����v���Ă���@false:�W�����v���Ă��Ȃ�</returns>
	bool IsJump()const { return m_isJump; }
	/// <summary>
	/// �W�����v�����Ă��邩���Ă��Ȃ��������߂�
	/// </summary>
	/// <param name="isJump">true:�W�����v���Ă���@false:�W�����v���Ă��Ȃ�</param>
	void SetJump(bool isJump) { m_isJump = isJump; }

	Vector2 GetVec()const { return m_vec; }

	void OnDamage();

	/// <summary>
	/// �}�b�v�`�b�v����肷��
	/// </summary>
	/// <param name="chipId">�}�b�v�`�b�v</param>
	void GetChip(int chipId) { m_chipId = chipId; }
protected:
	std::shared_ptr<Player> m_player = nullptr;				//�v���C���[
	std::shared_ptr<HpBar> m_hp = nullptr;					//Hp�o�[
	std::shared_ptr<ShotFactory> m_shotFactory = nullptr;	//�V���b�g

	bool m_isExist = true;	//���݂��邩
	Rect m_rect;	//�G�̋�`

	Vector2 m_vec;//�ړ����x

	int m_handle = -1;

	bool m_isLeft = false;	//���������Ă���

	bool m_isJump = false;	//�W�����v�����Ă��邩

	int m_chipId;//�}�b�v�`�b�v
	bool m_isOnDamage = false;//�_���[�W���󂯂����ǂ���
};

