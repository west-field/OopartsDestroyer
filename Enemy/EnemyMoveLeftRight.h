#pragma once
#include "EnemyBase.h"
#include "../Util/Geometry.h"
/// <summary>
/// くっつきスージー　壁、天井、床を上下、または、左右に往復　HP5,攻撃力4
/// </summary>
class EnemyMoveLeftRight : public EnemyBase
{
public:
	EnemyMoveLeftRight(std::shared_ptr<Player>player, const Position2 pos, int handle, std::shared_ptr<ShotFactory> sFactory);
	virtual ~EnemyMoveLeftRight();

	virtual void Update();
	virtual void Draw();
	virtual void Movement(Vector2 vec);
	virtual int TouchAttackPower() const;
	virtual void Damage(int damage);
private:
	int m_idx = 0;
	int m_handle = -1;
};

