#include "Map.h"
#include <DxLib.h>
#include <cassert>
#include "game.h"
#include "Util/DrawFunctions.h"
#include "Game/EnemyFactory.h"

namespace
{
	constexpr float kScale = 1.0f;
}

Map::Map(std::shared_ptr<EnemyFactory> enemyFactory) :m_handle(-1), m_graphSize(),m_camera(),m_enemies(enemyFactory)
{
	m_handle = my::MyLoadGraph(L"Data/mapchip.bmp");
	GetGraphSize(m_handle, &m_graphSize.w, &m_graphSize.h);
	//m_camera.y = ((Game::kMapChipNumY * Game::ChipSize) - (Game::kNumY * Game::ChipSize)) * -1.0f;

}

Map::~Map()
{
	DeleteGraph(m_handle);
}

void Map::Update()
{
	m_frame++;
	if (m_frame % 60 == 0)
	{
		m_frame = 0;
		
	}

	if ((59 * Game::ChipSize + Game::ChipSize / 2 /*- Game::ChipSize*/) + m_camera.x <= Game::kMapScreenRightX && num == 0)
	{
		DrawString(0, 300, L"出現", 0xffffff);
		num++;
		
		m_enemies->Create(EnemyType::MoveUpDown, { (59 * Game::ChipSize + Game::ChipSize / 2) + m_camera.x,((Game::kMapChipNumY - 9) * Game::ChipSize + Game::ChipSize / 2) + m_camera.y });
		m_enemies->Create(EnemyType::Battery, { (59 * Game::ChipSize + Game::ChipSize / 2) + m_camera.x,((Game::kMapChipNumY - 9) * Game::ChipSize + Game::ChipSize / 2) + m_camera.y });
	}

	//固定砲台
	//m_enemies->Create(EnemyType::Battery, { (59 * Game::ChipSize + Game::ChipSize / 2) + m_camera.x,((Game::kMapChipNumY - 9) * Game::ChipSize + Game::ChipSize / 2) + m_camera.y });
	//横移動する敵を作成する
	//m_enemies->Create(EnemyType::MoveUpDown, { (59 * Game::ChipSize + Game::ChipSize / 2) + m_camera.x,((Game::kMapChipNumY - 9) * Game::ChipSize + Game::ChipSize / 2) + m_camera.y });
}

void Map::Draw()
{
	for (int y = 0; y < Game::kMapChipNumY; y++)
	{
		for (int x = 0; x < Game::kMapChipNumX; x++)
		{
			const int chipNo = m_map[y][x];
			assert(chipNo >= 0);
			assert(chipNo < ChipNum());

			int graphX = (chipNo % ChipNumX()) * Game::ChipSize;
			int graphY = 0;

			int size = Game::ChipSize / 2;

			int X =  static_cast<int>((x * Game::ChipSize + size) + m_camera.x);
			int Y =  static_cast<int>((y * Game::ChipSize + size) + m_camera.y);

			if (chipNo == 0) continue;

			//画面外を表示しない
			if (X < Game::kMapScreenLeftX - size) continue;
			if (Y < Game::kMapScreenTopY - size)continue;
			if (X > Game::kMapScreenRightX + size) continue;
			if (Y > Game::kMapScreenBottomY + size) continue;

			my::MyDrawRectRotaGraph(X, Y , graphX, graphY, Game::ChipSize, Game::ChipSize, kScale, 0.0f, m_handle, false,false);
		}
	}
#ifdef _DEBUG
	DrawFormatString(0,140, 0xffffff, L"m_camera.x%3f,y%3f", m_camera.x, m_camera.y);
#endif
}

void Map::Movement(Vector2 vec)
{
	m_camera += vec;
}

void Map::SetPos(Position2 pos)
{
	m_camera.x = pos.x;
}

int Map::GetMapChipParam(float X, float Y)
{
	int x, y;

	// 整数値へ変換
	x = (int)(X) / Game::ChipSize;
	y = (int)(Y) / Game::ChipSize;

	// マップからはみ出ていたら 0 を返す
	if (x >= Game::kMapChipNumX || y >= Game::kMapChipNumY || x < 0 || y < 0) return 0;

	// 指定の座標に該当するマップの情報を返す (vector)
	/*const int chipNo = m_map[y * kBgChipNumX + x];
	assert(chipNo >= 0);
	assert(chipNo < ChipNum());*/

	const int chipNo = m_map[y][x];

	return chipNo;
}

int Map::ChipNumX()const
{
	return (m_graphSize.w / Game::ChipSize);
}
int Map::ChipNumY()const
{
	return (m_graphSize.h / Game::ChipSize);
}
int Map::ChipNum()const
{
	return (ChipNumX() * ChipNumY());
}

