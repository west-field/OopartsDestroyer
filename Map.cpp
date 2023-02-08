#include "Map.h"
#include <DxLib.h>
#include <cassert>
#include "game.h"
#include "Util/DrawFunctions.h"

Map::Map() :m_handle(-1), m_graphSize(),m_camera()
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

			int X =  static_cast<int>((x * Game::ChipSize + Game::ChipSize / 2) + m_camera.x);
			int Y = static_cast<int>((y * Game::ChipSize + Game::ChipSize / 2) + m_camera.y);

			if (chipNo == 0) continue;
			DrawRectRotaGraph(X, Y , graphX, graphY, Game::ChipSize, Game::ChipSize, 1.0f, 0.0f, m_handle, false);
		}
	}

	DrawFormatString(0,140, 0xffffff, L"m_camera.x%3f,y%3f", m_camera.x, m_camera.y);
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

