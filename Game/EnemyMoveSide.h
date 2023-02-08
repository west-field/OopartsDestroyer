#pragma once
#include "EnemyBase.h"
#include "../Util/Geometry.h"

class MoveSide : public EnemyBase
{
public:
	MoveSide(std::shared_ptr<Player>player, const Position2 pos,int handle);
	virtual ~MoveSide();
	virtual void Update();
	virtual void Draw();
	virtual void Movement(Vector2 vec);
	virtual void Damage(int damage);
private:
	int m_idx = 0;
	int m_handle = -1;
};

