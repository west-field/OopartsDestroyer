#pragma once
#include "EnemyBase.h"
#include "../Util/Geometry.h"

/// <summary>
/// ブラスター　壁にある赤い砲台。シャッターが開いたときに攻撃してくる。HP1、攻撃力（弾）2、（接触）1
/// </summary>
class EnemyBattery : public EnemyBase
{
public:
	EnemyBattery(std::shared_ptr<Player>player, const Position2 pos,int handle,std::shared_ptr<ShotFactory> sFactory,bool isLeft);
	virtual ~EnemyBattery();
	virtual void Update()override;		//更新
	virtual void Draw()override;		//表示
	//移動させる
	virtual void Movement(Vector2 vec)override;
	//接触した時の攻撃力
	virtual int TouchAttackPower()const override;
private:
	int m_idx = 0;		//表示する画像の場所
	int m_fireFrame = 0;
	int num = 0;
};

