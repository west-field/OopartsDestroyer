#pragma once
#include "EnemyBase.h"
#include "../Util/Geometry.h"

/// <summary>
/// �u���X�^�[�@�ǂɂ���Ԃ��C��B�V���b�^�[���J�����Ƃ��ɍU�����Ă���BHP1�A�U���́i�e�j2�A�i�ڐG�j1
/// </summary>
class EnemyBattery : public EnemyBase
{
public:
	EnemyBattery(std::shared_ptr<Player>player, const Position2 pos,int handle, int burstH,std::shared_ptr<ShotFactory> sFactory, std::shared_ptr<ItemFactory> itFactory,bool isLeft);
	virtual ~EnemyBattery();
	virtual void Update()override;		//�X�V
	virtual void Draw()override;		//�\��

	/// <summary>
	/// �ڐG�������̍U����
	/// </summary>
	/// <returns>�U����</returns>
	virtual int TouchAttackPower()const override;
	/// <summary>
	/// �_���[�W���󂯂�
	/// </summary>
	/// <param name="damage">�_���[�W��</param>
	virtual void Damage(int damage) override;
	/// <summary>
	/// �����蔻��Ώۂ�
	/// </summary>
	/// <returns>true:������ false:������Ȃ�</returns>
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

