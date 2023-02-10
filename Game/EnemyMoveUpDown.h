#pragma once
#include "EnemyBase.h"
#include "../Util/Geometry.h"

/// <summary>
/// ブンビーへりー　飛行しながら近付いて、プレイヤーの近くで上下に動き回る敵。HP1、攻撃力4
/// </summary>
class EnemyMoveUpDown : public EnemyBase
{
public:
	EnemyMoveUpDown(std::shared_ptr<Player>player, const Position2 pos,int handle);
	virtual ~EnemyMoveUpDown();
	virtual void Update();
	virtual void Draw();
	virtual void Movement(Vector2 vec);
	virtual int TouchAttackPower()const;
	virtual void Damage(int damage);
private:
	int m_idx = 0;
	int m_handle = -1;
};

