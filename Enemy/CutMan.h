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
	virtual int TouchAttackPower() const override;
private:
	virtual void MoveUpdate();//�{�X���ړ�������
	virtual void StopUpdate();//���̎w�������߂�
	virtual void JumpUpdate();//�{�X���W�����v������
	virtual void OneShotUpdate();//�{�X��1��e������
	virtual void TwoShotUpdate();//�{�X��2��e������
	void (CutMan::* updateFunc)();

	int m_idx = 0;//�摜�̕\���͈�
	int m_shotFrame;//���̒e�����܂�
	int m_JumpFrame;//���̃W�����v�܂�
};

