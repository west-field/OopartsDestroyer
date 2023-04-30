#pragma once
#include "EnemyBase.h"

/// <summary>
/// ジャンプする敵　HP1,攻撃力2,接触2
/// (カマドーマー　ロックマンがいる方向にジャンプしている)　
/// </summary>
class EnemyJump : public EnemyBase
{
public:
	EnemyJump(std::shared_ptr<Player>player, const Position2 pos, int handle, int burstH, std::shared_ptr<ShotFactory> sFactory, std::shared_ptr<ItemFactory> itFactory);
	virtual ~EnemyJump();

	virtual void Update() override;
	virtual void Draw()override;

	//当たり判定対象かどうか
	virtual bool IsCollidable()const override;
	//ダメージを受けた
	virtual void Damage(int damage) override;
private:
	//通常
	void NormalUpdate();
	void NormalDraw();

	//爆発
	void BurstUpdate();
	void BurstDraw();

	//ジャンプする
	void JumpUpdate();
	//落下する
	void DownUpdate();

	void (EnemyJump::* m_updateFunc)();
	void (EnemyJump::* m_drawFunc)();

	int m_frame;		//ジャンプするまでの時間

	float m_posTemp;	//ジャンプ最高点
};

