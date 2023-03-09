#pragma once
#include "EnemyBase.h"
#include "../Util/Geometry.h"

/// <summary>
/// ブンビーへりー　飛行しながら近付いて、プレイヤーの近くで上下に動き回る敵。HP1、攻撃力4
/// </summary>
class EnemyMoveLeft : public EnemyBase
{
public:
	EnemyMoveLeft(std::shared_ptr<Player>player, const Position2 pos,int handle, int burstH, std::shared_ptr<ShotFactory> sFactory);
	virtual ~EnemyMoveLeft();
	virtual void Update()override;//更新
	virtual void Draw()override;//表示
	virtual void Movement(Vector2 vec)override;
	virtual int TouchAttackPower()const override;
	virtual void Damage(int damage) override;
	virtual bool IsCollidable()const override;

	void SetPos(Position2 pos) { m_rect.center = pos; }
private:
	void NormalUpdate();
	void NormalDraw();
	//爆発アニメーション
	void BurstUpdate();
	void BurstDraw();

	//メンバ関数ポインタ
	void (EnemyMoveLeft::* m_updateFunc)();
	void (EnemyMoveLeft::* m_drawFunc)();
};

