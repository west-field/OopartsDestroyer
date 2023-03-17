#pragma once
#include "EnemyBase.h"
#include "../Util/Geometry.h"

class Stage;

/// <summary>
/// くっつきスージー　壁、天井、床を上下、または、左右に往復　HP5,攻撃力4
/// </summary>
class EnemyMoveLR : public EnemyBase
{
public:
	EnemyMoveLR(std::shared_ptr<Player>player, const Position2 pos, int handle, int burstH, std::shared_ptr<ShotFactory> sFactory, std::shared_ptr<ItemFactory> itFactory);
	virtual ~EnemyMoveLR();

	virtual void Update()override;
	virtual void Draw()override;
	//接触した時の攻撃力
	virtual int TouchAttackPower() const override;
	//ダメージを受けた
	virtual void Damage(int damage) override;
	//当たり判定対象か
	virtual bool IsCollidable()const override;
private:
	//通常
	void NormalUpdate();
	void NormalDraw();

	//爆発
	void BurstUpdate();
	void BurstDraw();

	//左右に動く
	void MoveUpdate();

	void (EnemyMoveLR::* m_updateFunc)();
	void (EnemyMoveLR::* m_drawFunc)();

	int m_frame = 0;
};

