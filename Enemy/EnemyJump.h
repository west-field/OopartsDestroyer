#pragma once
#include "EnemyBase.h"

/// <summary>
/// �J�}�h�[�}�[�@���b�N�}������������ɃW�����v���Ă���@HP1,�U����2,�ڐG2
/// </summary>
class EnemyJump : public EnemyBase
{
public:
	EnemyJump(std::shared_ptr<Player>player, const Position2 pos, int handle, std::shared_ptr<ShotFactory> sFactory);
	virtual ~EnemyJump();

	virtual void Update() override;
	virtual void Draw()override;
	virtual void Movement(Vector2 vec)override;
	virtual int TouchAttackPower() const override;
	virtual void Damage(int damage) override;
	virtual bool IsCollidable()const override;
private:
	void NormalUpdate();
	void JumpUpdate();
	void DownUpdate();

	void NormalDraw();

	//�����A�j���[�V����
	void BurstUpdate();
	void BurstDraw();

	void (EnemyJump::* m_updateFunc)();
	void (EnemyJump::* m_drawFunc)();

	int m_idx = 0;//�摜�ύX
	int m_frame = 0;//�W�����v����܂ł̎���

	float m_posTemp = 0.0f;
};

