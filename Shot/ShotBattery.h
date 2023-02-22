#pragma once
#include "ShotBase.h"
class ShotBattery : public ShotBase
{
public:
	ShotBattery(int handle);
	virtual ~ShotBattery();

	virtual void Start(Position2 pos, Vector2 vel,bool left)override;
	virtual void Update()override;
	virtual void Draw()override;
	virtual const int AttackPower()const override;
private:
	
};

