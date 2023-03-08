#pragma once
#include "EnemyBase.h"
#include "../Util/Geometry.h"

class Stage;

/// <summary>
/// �������X�[�W�[�@�ǁA�V��A�����㉺�A�܂��́A���E�ɉ����@HP5,�U����4
/// </summary>
class EnemyMoveLR : public EnemyBase
{
public:
	EnemyMoveLR(std::shared_ptr<Player>player, const Position2 pos, int handle, int burstH, std::shared_ptr<ShotFactory> sFactory);
	virtual ~EnemyMoveLR();

	virtual void Update()override;
	virtual void Draw()override;
	virtual void Movement(Vector2 vec)override;
	virtual int TouchAttackPower() const override;
	virtual void Damage(int damage) override;
	virtual bool IsCollidable()const override;
private:
	void NormalUpdate();
	void LeftUpdate();
	void RightUpdate();

	void NormalDraw();

	void BurstUpdate();
	void BurstDraw();

	void (EnemyMoveLR::* m_updateFunc)();
	void (EnemyMoveLR::* m_drawFunc)();

	int m_frame = 0;
};
