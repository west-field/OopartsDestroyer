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
	virtual void Update()override;		//更新
	virtual void Draw()override;		//表示

	// ダメージを受けた
	virtual void Damage(int damage) override;
	// あたり判定対象か
	virtual bool IsCollidable()const override;
private:
	//通常
	void NormalUpdate();
	void NormalDraw();
	//爆発アニメーション
	void BurstUpdate();
	void BurstDraw();

	void (EnemyBattery::* m_updateFunc)();
	void (EnemyBattery::* m_drawFunc)();

	int m_num = 0;//攻撃絵の時一度だけ弾を作成
};

