#pragma once
#include "game.h"
#include "Util/Geometry.h"
#include <memory>
#include <vector>
#include "Stage.h"

class EnemyFactory;

enum MapLayer
{
	MapLayer_map,
	MapLayer_enemy,
	MapLayer_event
};

class Map
{
public:

	Map(std::shared_ptr<EnemyFactory> enemyFactory,int stage);
	virtual ~Map();
	void Update();
	void Draw();

	void Movement(Vector2 vec);
	const Position2 GetPos()const { return m_camera; }
	void SetPos(Position2 pos);

	//マップチップの値を取得する関数
	int GetMapChipParam(float X, float Y);
	int GetMapEventParam(float X, float Y);
private:

	Position2 m_camera;

	std::shared_ptr<Stage> m_stageMap;
	int m_stage;

	int m_handle;//グラフィック

	std::vector<int> m_enemyPos;
	std::shared_ptr<EnemyFactory> m_enemies;
	int m_frame = 0;
	int num = 0;
};

