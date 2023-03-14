#pragma once
#include "ShotBase.h"

class RockBuster : public ShotBase
{
public:
	RockBuster(int handle);
	virtual ~RockBuster();

	virtual void Start(Position2 pos, Vector2 vel, bool left, bool isPlayer) override;
	virtual void Update()override;
	virtual void Draw()override;
	virtual void Movement(Vector2 vec)override;
	virtual const int AttackPower()const;
private:
};

