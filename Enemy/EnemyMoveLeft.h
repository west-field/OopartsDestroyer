#pragma once
#include "EnemyBase.h"
#include "../Util/Geometry.h"

/// <summary>
/// �u���r�[�ւ�[�@��s���Ȃ���ߕt���āA�v���C���[�̋߂��ŏ㉺�ɓ������G�BHP1�A�U����4
/// </summary>
class EnemyMoveLeft : public EnemyBase
{
public:
	EnemyMoveLeft(std::shared_ptr<Player>player, const Position2 pos,int handle, int burstH, std::shared_ptr<ShotFactory> sFactory);
	virtual ~EnemyMoveLeft();
	virtual void Update()override;//�X�V
	virtual void Draw()override;//�\��
	virtual void Movement(Vector2 vec)override;
	virtual int TouchAttackPower()const override;
	virtual void Damage(int damage) override;
	virtual bool IsCollidable()const override;

	void SetPos(Position2 pos) { m_rect.center = pos; }
private:
	void NormalUpdate();
	void NormalDraw();
	//�����A�j���[�V����
	void BurstUpdate();
	void BurstDraw();

	//�����o�֐��|�C���^
	void (EnemyMoveLeft::* m_updateFunc)();
	void (EnemyMoveLeft::* m_drawFunc)();
};

