#pragma once
#include "EnemyBase.h"

/// <summary>
/// �ǂɒ���t���āA�e�������Ă���G�@HP1�A�U���́i�e�j2�A�i�ڐG�j1
/// (�u���X�^�[�@�ǂɂ���Ԃ��C��B�V���b�^�[���J�����Ƃ��ɍU�����Ă���B)
/// </summary>
class EnemyBattery : public EnemyBase
{
public:
	EnemyBattery(std::shared_ptr<Player>player, const Position2 pos,int handle, int burstH,std::shared_ptr<ShotFactory> sFactory, std::shared_ptr<ItemFactory> itFactory,bool isLeft);
	virtual ~EnemyBattery();
	virtual void Update()override;		//�X�V
	virtual void Draw()override;		//�\��

	// �_���[�W���󂯂�
	virtual void Damage(int damage) override;
	// �����蔻��Ώۂ�
	virtual bool IsCollidable()const override;
private:
	//�ʏ�
	void NormalUpdate();
	void NormalDraw();
	//�����A�j���[�V����
	void BurstUpdate();
	void BurstDraw();

	void (EnemyBattery::* m_updateFunc)();
	void (EnemyBattery::* m_drawFunc)();

	int m_num = 0;//�U���G�̎���x�����e���쐬
};

