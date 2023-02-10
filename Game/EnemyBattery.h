#pragma once
#include "EnemyBase.h"
#include "../Util/Geometry.h"

/// <summary>
/// ブラスター　壁にある赤い砲台。シャッターが開いたときに攻撃してくる。HP1、攻撃力（弾）2、（接触）1
/// </summary>
class EnemyBattery : public EnemyBase
{
public:
	EnemyBattery(std::shared_ptr<Player>player, const Position2 pos,int handle,std::shared_ptr<ShotFactory> sFactory);
	virtual ~EnemyBattery();
	virtual void Update();
	virtual void Draw();
	virtual void Movement(Vector2 vec);
	//接触した時の攻撃力
	virtual int TouchAttackPower()const;
	//ダメージ
	virtual void Damage(int damage);
private:
	int m_idx = 0;
	int m_handle = -1;
};

