#pragma once
#include "EnemyBase.h"
#include "../Util/Geometry.h"

class Stage;

/// <summary>
/// �������X�[�W�[�@�ǁA�V��A�����㉺�A�܂��́A���E�ɉ����@HP5,�U����4
/// </summary>
class EnemyMoveUD : public EnemyBase
{
public:
	EnemyMoveUD(std::shared_ptr<Player>player, const Position2 pos, int handle, int burstH, std::shared_ptr<ShotFactory> sFactory);
	virtual ~EnemyMoveUD();

	virtual void Update()override;
	virtual void Draw()override;
	virtual void Movement(Vector2 vec)override;
	virtual int TouchAttackPower() const override;
	virtual void Damage(int damage) override;
	virtual bool IsCollidable()const override;
private:
	void NormalUpdate();
	void UpUpdate();
	void DownUpdate();

	void NormalDraw();

	void BurstUpdate();
	void BurstDraw();

	void (EnemyMoveUD::* m_updateFunc)();
	void (EnemyMoveUD::* m_drawFunc)();

	int m_frame = 0;
};

