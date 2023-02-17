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
	virtual void Update()override;		//�X�V
	virtual void Draw()override;		//�\��
	//�ړ�������
	virtual void Movement(Vector2 vec)override;
	//�ڐG�������̍U����
	virtual int TouchAttackPower()const override;
private:
	int m_idx = 0;		//�\������摜�̏ꏊ
	int m_handle = -1;	//�摜�n���h��
	int m_fireFrame;
	int num = 0;
};

