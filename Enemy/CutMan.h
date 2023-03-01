#pragma once
#include "EnemyBase.h"
#include "../Util/Geometry.h"

/// <summary>
/// �J�b�g�}���i�{�X�j �U����,�ڐG:4
/// </summary>
class CutMan :public EnemyBase
{
public :
	CutMan(std::shared_ptr<Player>player, const Position2& pos,int handle, std::shared_ptr<ShotFactory> sFactory);
	virtual ~CutMan();
	virtual void Update();
	virtual void Draw();

	virtual void Movement(Vector2 vec)override;
	virtual void EnemyMovement(Vector2 vec)override;
	virtual int TouchAttackPower() const override;
	virtual bool IsCollidable()const override;
private:
	void MoveUpdate();//�{�X���ړ�������
	void StopUpdate();//���̎w�������߂�
	void JumpUpdate();//�{�X���W�����v������
	void DownUpdate();//�{�X���W�����v������
	void OneShotUpdate();//�{�X��1��e������
	void TwoShotUpdate();//�{�X��2��e������
	
	void (CutMan::* updateFunc)();

	int m_idx = 0;//�摜�̕\���͈�
	int m_frame = 0;//���̉摜�Ɉړ�
	int m_shotFrame;//���̒e�����܂�
	int m_JumpFrame;//���̃W�����v�܂�

	float m_posTemp = 0.0f;//������

};

