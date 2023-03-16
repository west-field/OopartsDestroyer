#pragma once
#include "EnemyBase.h"
#include "../Util/Geometry.h"

/// <summary>
/// �J�b�g�}���i�{�X�j �U����,�ڐG:4
/// </summary>
class Boss :public EnemyBase
{
public :
	Boss(std::shared_ptr<Player>player, const Position2& pos,int handle, int bossBurstH, int burstH, std::shared_ptr<ShotFactory> sFactory, std::shared_ptr<ItemFactory> itFactory, std::shared_ptr<HpBar>hp);
	virtual ~Boss();
	virtual void Update()override;
	virtual void Draw()override;

	virtual void Movement(Vector2 vec)override;
	virtual void EnemyMovement(Vector2 vec)override;
	virtual int TouchAttackPower() const override;
	virtual void Damage(int damage) override;
	virtual bool IsCollidable()const override;
private:
	void MoveUpdate();//�{�X���ړ�������
	void StopUpdate();//���̎w�������߂�
	void JumpUpdate();//�{�X���W�����v������
	void DownUpdate();//�{�X���W�����v������
	void OneShotUpdate();//�{�X��1��e������
	void TwoShotUpdate();//�{�X��2��e������
	
	void NormalDraw();

	void BurstUpdate();
	void BurstDraw();

	void (Boss::* updateFunc)();
	void (Boss::* m_drawFunc)();

	int m_bossBurstH = -1;//�{�X�����摜

	int m_idx = 0;//�摜�̕\���͈�
	
	int m_animFrame = 0;//���̉摜�Ɉړ�(�{�X
	
	int m_frame = 0;//���̍U��������܂ł̎���
	
	int m_shotFrame = 20;//���U������Ƃ����̒e�����܂�
	int m_JumpFrame;//���̃W�����v�܂�

	float m_posTemp = 0.0f;//������

};

