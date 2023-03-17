#pragma once
#include "../Util/Geometry.h"

enum class ShotType
{
	RockBuster,
	ShotBattery,
};

/// <summary>
/// �e���N���X
/// </summary>
class ShotBase
{
public:
	ShotBase(int handle);
	virtual ~ShotBase();

	//�V���b�g�J�n
	virtual void Start(Position2 pos, Vector2 vel, bool left, bool isPlayer) = 0;
	//�X�V
	virtual void Update() = 0;
	//�`��
	virtual void Draw() = 0;
	//���Ɍ��������ǂ���
	virtual bool IsLeft() const { return m_isLeft; }
	//���݂��邩
	virtual bool IsExist() const { return m_isExist; }
	//��������
	void SetExist(bool isExist) { m_isExist = isExist; }
	//�v���C���[���������e��
	virtual void PlayerShot(bool isPlayerShot) { m_isPlayerShot = isPlayerShot; }
	/// <summary>
	/// �v���C���[���������e��
	/// </summary>
	/// <returns>true:�v���C���[�������� false:�G��������</returns>
	virtual bool IsPlayerShot()const { return m_isPlayerShot; }
	//�e���ړ�
	virtual void Movement(Vector2 vec) = 0;
	//��`���擾
	virtual const Rect& GetRect()const;
	//�U����
	virtual const int AttackPower()const = 0;
protected:
	
	int m_handle;//�O���t�B�b�N�n���h��
	
	Rect m_rect;//��`

	Vector2 m_vel;	//�e�̃X�s�[�h
	
	bool m_isExist;//���݂��邩
	
	bool m_isLeft;//���Ɍ��������ǂ���
	
	bool m_isPlayerShot;//�v���C���[���������e��
};

