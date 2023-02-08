#pragma once
#include "ShotBase.h"
#include "../Util/Geometry.h"

class Player;
class EnemyBase;

class RockBuster : public ShotBase
{
public:
	RockBuster();
	virtual ~RockBuster();

	virtual void Start(Position2 pos,bool left);
	virtual void Update();
	virtual void Draw();
private:
	int m_handle;
};

