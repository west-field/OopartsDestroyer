#pragma once
#include "EnemyBase.h"

/// <summary>
/// 壁に張り付いて、弾を撃ってくる敵　HP1、攻撃力（弾）2、（接触）1
/// (ブラスター　壁にある赤い砲台。シャッターが開いたときに攻撃してくる。)
/// </summary>
class EnemyBattery : public EnemyBase
{
public:
	EnemyBattery(std::shared_ptr<Player>player, const Position2 pos,int handle, int burstH,std::shared_ptr<ShotFactory> sFactory, std::shared_ptr<ItemFactory> itFactory,bool isLeft);
	virtual ~EnemyBattery();

	virtual void Update()override;
	virtual void Draw()override;

	// 当たり判定対象かどうか
	virtual bool IsCollidable()const override;
	// ダメージを受けた
	virtual void Damage(int damage) override;
private:
	//通常
	void NormalUpdate();
	void NormalDraw();

	//爆発
	void BurstUpdate();
	void BurstDraw();

	void (EnemyBattery::* m_updateFunc)();
	void (EnemyBattery::* m_drawFunc)();

	int m_createShot;//攻撃絵の時一度だけ弾を作成
};

