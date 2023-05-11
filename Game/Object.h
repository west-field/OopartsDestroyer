#pragma once

#include "../Util/Geometry.h"
#include <memory>
#include "../ObjectInfo.h"

class HpBar;

class Object
{
public:
	Object();
	virtual ~Object();

	virtual void Update() = 0;
	virtual void Draw() = 0;

	/// <summary>
	/// �ړ�
	/// </summary>
	/// <param name="vec">�ړ���</param>
	void Movement(Vector2 vec);
	/// <summary>
	/// ��`���擾
	/// </summary>
	/// <returns>��`�\����</returns>
	const Rect& GetRect() const;

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
	/// �_���[�W���󂯂�
	/// </summary>
	/// <param name="damage">�_���[�W��</param>
	virtual void Damage(int damage) = 0;

	/// <summary>
	/// ���݂��Ă��邩�ǂ���
	/// </summary>
	/// <returns>true:���݂��� false:���݂��Ȃ�</returns>
	bool IsExist()const { return m_isExist; }

	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="isExist">���݂�����</param>
	virtual void EraseExist();

	/// <summary>
	/// �ړ��ʂ�Ԃ�
	/// </summary>
	/// <returns>�ړ�</returns>
	virtual Vector2 GetVec()const;

	/// <summary>
	/// �}�b�v�`�b�v����肷��
	/// </summary>
	/// <param name="chipId">�}�b�v�`�b�v</param>
	virtual void GetChip(int chipId) = 0;

	/// <summary>
	/// ����������̂�
	/// </summary>
	/// <returns>�I�u�W�F�N�g�̎��</returns>
	ObjectType Type()const { return m_type; }
protected:
	std::shared_ptr<HpBar> m_hp;

	Rect m_rect;//��`

	int m_handle;//�摜�n���h��

	int m_idx;//�摜�ύX

	bool m_isExist;//���݂��邩�ǂ���

	bool m_isLeft;//���������Ă��邩

	bool m_isJump;//�W�����v���Ă��邩

	ObjectType m_type;//����������̂�
};

