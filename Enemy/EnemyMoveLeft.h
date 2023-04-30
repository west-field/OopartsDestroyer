#pragma once
#include "EnemyBase.h"

/// <summary>
///　左に飛んでいく敵　HP1、攻撃力4
/// (ブンビーへりー　飛行しながら近付いて、プレイヤーの近くで上下に動き回る敵。)
/// </summary>
class EnemyMoveLeft : public EnemyBase
{
public:
	EnemyMoveLeft(std::shared_ptr<Player>player, const Position2 pos,int handle, int burstH, std::shared_ptr<ShotFactory> sFactory, std::shared_ptr<ItemFactory> itFactory);
	virtual ~EnemyMoveLeft();
	virtual void Update()override;
	virtual void Draw()override;

	//当たり判定対象かどうか
	virtual bool IsCollidable()const override;
	//ダメージを受けた
	virtual void Damage(int damage) override;
	//場所を決める(titleでのみ使用している)
	void SetPos(Position2 pos) { m_rect.center = pos; }
private:
	//通常
	void NormalUpdate();
	void NormalDraw();

	//爆発
	void BurstUpdate();
	void BurstDraw();

	//メンバ関数ポインタ
	void (EnemyMoveLeft::* m_updateFunc)();
	void (EnemyMoveLeft::* m_drawFunc)();
};

