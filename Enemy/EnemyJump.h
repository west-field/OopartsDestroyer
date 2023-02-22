#pragma once
#include "EnemyBase.h"

/// <summary>
/// カマドーマー　ロックマンがいる方向にジャンプしている　HP1,攻撃力2,接触2
/// </summary>
class EnemyJump : public EnemyBase
{
public:
	EnemyJump(std::shared_ptr<Player>player, const Position2 pos, int handle, std::shared_ptr<ShotFactory> sFactory);
	virtual ~EnemyJump();

	virtual void Update() override;
	virtual void Draw()override;
	virtual void Movement(Vector2 vec)override;
	virtual int TouchAttackPower() const override;
private:
	void MoveUpdate();
	void JumpUpdate();
	void DownUpdate();

	void (EnemyJump::* m_updateFunc)();

	int m_idx = 0;//画像変更
	int m_frame = 0;//ジャンプするまでの時間
};

