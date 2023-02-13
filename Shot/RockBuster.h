#pragma once
#include "ShotBase.h"
#include "../Util/Geometry.h"

class RockBuster : public ShotBase
{
public:
	RockBuster(int handle);
	virtual ~RockBuster();

	virtual void Start(Position2 pos,bool left);
	virtual void Update();
	virtual void Draw();
	virtual const int AttackPower()const;
private:
};

