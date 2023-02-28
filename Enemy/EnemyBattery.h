#pragma once
#include "EnemyBase.h"
#include "../Util/Geometry.h"

/// <summary>
/// �u���X�^�[�@�ǂɂ���Ԃ��C��B�V���b�^�[���J�����Ƃ��ɍU�����Ă���BHP1�A�U���́i�e�j2�A�i�ڐG�j1
/// </summary>
class EnemyBattery : public EnemyBase
{
public:
	EnemyBattery(std::shared_ptr<Player>player, const Position2 pos,int handle,std::shared_ptr<ShotFactory> sFactory,bool isLeft);
	virtual ~EnemyBattery();
	virtual void Update()override;		//�X�V
	virtual void Draw()override;		//�\��
	//�ړ�������
	virtual void Movement(Vector2 vec)override;
	//�ڐG�������̍U����
	virtual int TouchAttackPower()const override;
	virtual bool IsCollidable()const override;
private:
	int m_idx = 0;		//�\������摜�̏ꏊ
	int m_fireFrame = 0;
	int num = 0;

	struct Shot
	{
		Position2 pos = { 0.0f,0.0f };//�ʒu
		float angle = 0.0f;//����
		Vector2 vel = { -2.0f,0.0f };//���x
		bool isExist = false;//���݂��Ă��邩
	};
	static constexpr int kShotNum = 4;
	Shot m_shot[kShotNum];//�l�񌂂�
};

