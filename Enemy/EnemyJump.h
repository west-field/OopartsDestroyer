#pragma once
#include "EnemyBase.h"

/// <summary>
/// カマドーマー　ロックマンがいる方向にジャンプしている　HP1,攻撃力2,接触2
/// </summary>
class EnemyJump : public EnemyBase
{
public:
	EnemyJump(std::shared_ptr<Player>player, const Position2 pos, int handle, int burstH, std::shared_ptr<ShotFactory> sFactory, std::shared_ptr<ItemFactory> itFactory);
	virtual ~EnemyJump();

	virtual void Update() override;
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

	//爆発アニメーション
	void BurstUpdate();
	void BurstDraw();

	//ジャンプする
	void JumpUpdate();
	//落下する
	void DownUpdate();

	void (EnemyJump::* m_updateFunc)();
	void (EnemyJump::* m_drawFunc)();

	int m_frame = 0;//ジャンプするまでの時間

	float m_posTemp = 0.0f;//ジャンプ最高点
};

