#pragma once
#include "../Util/Geometry.h"

enum class ShotType
{
	RockBuster,
};

/// <summary>
/// �e���N���X
/// </summary>
class ShotBase
{
public:
	ShotBase();
	virtual ~ShotBase();

	void SetHandle(int handle);
	//�V���b�g�J�n
	virtual void Start(Position2 pos, bool left) = 0;
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
	virtual bool GetPlayerShot()const { return m_isPlayerShot; }
	//�e���ړ�
	virtual void Movement(Vector2 vec);
	
	virtual const Rect& GetRect()const;

	virtual const int AttackPower()const = 0;
protected:
	
	int m_handle;//�O���t�B�b�N�n���h��
	
	Rect m_rect;

	Vector2 m_vel;	//�e�̃X�s�[�h
	
	bool m_isExist;//���݂��邩
	
	bool m_isLeft;//���Ɍ��������ǂ���
	
	bool m_isPlayerShot;//�v���C���[���������e��
};

