#pragma once
#include "EnemyBase.h"

/// <summary>
///　左右に移動する敵　HP5,攻撃力4
/// (くっつきスージー　壁、天井、床を上下、または、左右に往復)　
/// </summary>
class EnemyMoveLR : public EnemyBase
{
public:
	EnemyMoveLR(std::shared_ptr<Player>player, const Position2 pos, int handle, int burstH, std::shared_ptr<ShotFactory> sFactory, std::shared_ptr<ItemFactory> itFactory);
	virtual ~EnemyMoveLR();

	virtual void Update()override;		//更新
	virtual void Draw()override;		//表示

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

	bool m_isOnDamage = false;
};

