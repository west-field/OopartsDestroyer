#pragma once
#include "EnemyBase.h"
#include "../Util/Geometry.h"

/// <summary>
/// ブラスター　壁にある赤い砲台。シャッターが開いたときに攻撃してくる。HP1、攻撃力（弾）2、（接触）1
/// </summary>
class EnemyBattery : public EnemyBase
{
public:
	EnemyBattery(std::shared_ptr<Player>player, const Position2 pos,int handle, int burstH,std::shared_ptr<ShotFactory> sFactory,bool isLeft);
	virtual ~EnemyBattery();
	virtual void Update()override;		//更新
	virtual void Draw()override;		//表示
	//移動させる
	virtual void Movement(Vector2 vec)override;
	//接触した時の攻撃力
	virtual int TouchAttackPower()const override;
	virtual void Damage(int damage) override;
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

	int m_fireFrame = 0;
	int num = 0;

	struct Shot
	{
		Position2 pos = { 0.0f,0.0f };//位置
		float angle = 0.0f;//向き
		Vector2 vel = { -2.0f,0.0f };//速度
		bool isExist = false;//存在しているか
	};
	static constexpr int kShotNum = 4;
	Shot m_shot[kShotNum];//四回撃つ
};

