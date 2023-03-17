#pragma once
#include "EnemyBase.h"

/// <summary>
/// �J�}�h�[�}�[�@���b�N�}������������ɃW�����v���Ă���@HP1,�U����2,�ڐG2
/// </summary>
class EnemyJump : public EnemyBase
{
public:
	EnemyJump(std::shared_ptr<Player>player, const Position2 pos, int handle, int burstH, std::shared_ptr<ShotFactory> sFactory, std::shared_ptr<ItemFactory> itFactory);
	virtual ~EnemyJump();

	virtual void Update() override;
	virtual void Draw()override;
	//�ڐG�������̍U����
	virtual int TouchAttackPower() const override;
	//�_���[�W���󂯂�
	virtual void Damage(int damage) override;
	//�����蔻��Ώۂ�
	virtual bool IsCollidable()const override;
private:
	//�ʏ�
	void NormalUpdate();
	void NormalDraw();

	//�����A�j���[�V����
	void BurstUpdate();
	void BurstDraw();

	//�W�����v����
	void JumpUpdate();
	//��������
	void DownUpdate();

	void (EnemyJump::* m_updateFunc)();
	void (EnemyJump::* m_drawFunc)();

	int m_frame = 0;//�W�����v����܂ł̎���

	float m_posTemp = 0.0f;//�W�����v�ō��_
};

