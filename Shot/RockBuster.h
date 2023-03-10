#pragma once
#include "ShotBase.h"
#include "../Util/Geometry.h"

class RockBuster : public ShotBase
{
public:
	RockBuster(int handle);
	virtual ~RockBuster();

	virtual void Start(Position2 pos, Vector2 vel, bool left, bool isPlayer) override;
	virtual void Update();
	virtual void Draw();
	virtual const int AttackPower()const;
private:
};

