#pragma once
#include "EnemyBase.h"
#include "../Util/Geometry.h"

/// <summary>
/// �u���X�^�[�@�ǂɂ���Ԃ��C��B�V���b�^�[���J�����Ƃ��ɍU�����Ă���BHP1�A�U���́i�e�j2�A�i�ڐG�j1
/// </summary>
class EnemyBattery : public EnemyBase
{
public:
	EnemyBattery(std::shared_ptr<Player>player, const Position2 pos,int handle,std::shared_ptr<ShotFactory> sFactory);
	virtual ~EnemyBattery();
	virtual void Update();		//�X�V
	virtual void Draw();		//�\��
	//�ړ�������
	virtual void Movement(Vector2 vec);
	//�ڐG�������̍U����
	virtual int TouchAttackPower()const;
	//�_���[�W���󂯂�
	virtual void Damage(int damage);
private:
	int m_idx = 0;		//�\������摜�̏ꏊ
	int m_handle = -1;	//�摜�n���h��
};

