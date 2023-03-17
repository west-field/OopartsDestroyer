#pragma once
#include "EnemyBase.h"
#include "../Util/Geometry.h"

/// <summary>
/// ブラスター　壁にある赤い砲台。シャッターが開いたときに攻撃してくる。HP1、攻撃力（弾）2、（接触）1
/// </summary>
class EnemyBattery : public EnemyBase
{
public:
	EnemyBattery(std::shared_ptr<Player>player, const Position2 pos,int handle, int burstH,std::shared_ptr<ShotFactory> sFactory, std::shared_ptr<ItemFactory> itFactory,bool isLeft);
	virtual ~EnemyBattery();
	virtual void Update()override;		//更新
	virtual void Draw()override;		//表示

	/// <summary>
	/// 接触した時の攻撃力
	/// </summary>
	/// <returns>攻撃力</returns>
	virtual int TouchAttackPower()const override;
	/// <summary>
	/// ダメージを受けた
	/// </summary>
	/// <param name="damage">ダメージ量</param>
	virtual void Damage(int damage) override;
	/// <summary>
	/// あたり判定対象か
	/// </summary>
	/// <returns>true:当たる false:当たらない</returns>
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

