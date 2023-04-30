#pragma once
#include "EnemyBase.h"

/// <summary>
/// �W�����v����G�@HP1,�U����2,�ڐG2
/// (�J�}�h�[�}�[�@���b�N�}������������ɃW�����v���Ă���)�@
/// </summary>
class EnemyJump : public EnemyBase
{
public:
	EnemyJump(std::shared_ptr<Player>player, const Position2 pos, int handle, int burstH, std::shared_ptr<ShotFactory> sFactory, std::shared_ptr<ItemFactory> itFactory);
	virtual ~EnemyJump();

	virtual void Update() override;
	virtual void Draw()override;

	//�����蔻��Ώۂ��ǂ���
	virtual bool IsCollidable()const override;
	//�_���[�W���󂯂�
	virtual void Damage(int damage) override;
private:
	//�ʏ�
	void NormalUpdate();
	void NormalDraw();

	//����
	void BurstUpdate();
	void BurstDraw();

	//�W�����v����
	void JumpUpdate();
	//��������
	void DownUpdate();

	void (EnemyJump::* m_updateFunc)();
	void (EnemyJump::* m_drawFunc)();

	int m_frame;		//�W�����v����܂ł̎���

	float m_posTemp;	//�W�����v�ō��_
};

