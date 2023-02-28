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

Map::Map(std::shared_ptr<EnemyFactory> enemyFactory,int stage) :
	m_handle(-1),m_camera(),m_enemies(enemyFactory),m_stage(stage),
	m_mapWidth(0),m_mapHeight(0)
{
	m_handle = my::MyLoadGraph(L"Data/mapchip.bmp");
}

Map::~Map()
{
	DeleteGraph(m_handle);
}

void Map::Update()
{
	for (int chipY = 0; chipY < m_mapHeight; chipY++)
	{
		for (int chipX = 0; chipX < m_mapWidth; chipX++)
		{
			//表示する位置
			int size = Game::ChipSize / 2;
			//右座標、下座標
			Vector2 pos = { ((chipX * Game::ChipSize + size) + m_camera.x) ,((chipY * Game::ChipSize + size) + m_camera.y) };

			//表示画面内にいないときは生成しない
			if (pos.x < Game::kMapScreenLeftX - Game::ChipSize)	continue;
			if (pos.x > Game::kMapScreenRightX + Game::ChipSize)	continue;
			if (pos.y < Game::kMapScreenTopY - Game::ChipSize)	continue;
			if (pos.y > Game::kMapScreenBottomY + Game::ChipSize)continue;

			auto chipId = GetChipId(MapLayer_enemy + m_stage, chipX, chipY);
			//何もないときはループを抜ける
			if (chipId == 0)	continue;

			//一度生成したら次は生成しない

			//画面内に入った時生成
			//mapXがGame::kMapScreenRightXからGame::kMapScreenRightX + Game::ChipSizeの中に入った時
			
			auto address = chipY * m_mapWidth + chipX;
#ifdef _DEBUG
			DrawFormatString(10, 800, 0x000000, L"マップ%d,エネミー位置%d", chipId, m_enemyPos[address]);
#endif
			//画面に入ったら一度だけ生成
			if (pos.x >= Game::kMapScreenRightX && pos.x <= Game::kMapScreenRightX + size && m_enemyPos[address] != 0 ||
				pos.y <= Game::kMapScreenTopY && pos.y >= Game::kMapScreenTopY - size && m_enemyPos[address] != 0)
			{
				switch (chipId)
				{
				case 1:
					m_enemies->Create(EnemyType::MoveUpDown, Position2(pos.x, pos.y));
					break;
				case 2:
					m_enemies->Create(EnemyType::BatteryRight, Position2(pos.x, pos.y));
					break;
				case 3:
					m_enemies->Create(EnemyType::BatteryLeft, Position2(pos.x, pos.y));
					break;
				case 4:
					m_enemies->Create(EnemyType::Jump, Position2(pos.x, pos.y));
					break;
				case 5:
					m_enemies->Create(EnemyType::MoveLeftRight, Position2(pos.x, pos.y));
					break;
				/*case 6:
					m_enemies->Create(EnemyType::MoveShot, Position2(pos.x, pos.y));
					break;*/
				case 7:
					m_enemies->Create(EnemyType::Boss, Position2(pos.x, pos.y));
					break;
				default:
					break;
				}
				m_enemyPos[address] = 0;
				break;
			}
		}
	}
}

void Map::Draw()
{
	for (int chipY = 0; chipY < m_mapHeight; chipY++)
	{
		for (int chipX = 0; chipX < m_mapWidth; chipX++)
		{
			auto chipId = GetChipId(m_stage, chipX, chipY);
			if (chipId != 0)
			{
				int size = Game::ChipSize / 2;

				int X = static_cast<int>((chipX * Game::ChipSize + size) + m_camera.x);
				int Y = static_cast<int>((chipY * Game::ChipSize + size) + m_camera.y);

				//画面外を表示しない
				if (X < Game::kMapScreenLeftX - size) continue;
				if (Y < Game::kMapScreenTopY - size)continue;
				if (X > Game::kMapScreenRightX + size) continue;
				if (Y > Game::kMapScreenBottomY + size) continue;

				my::MyDrawRectRotaGraph(X, Y, (chipId % 16) * Game::ChipSize, (chipId / 16) * Game::ChipSize, Game::ChipSize, Game::ChipSize, kScale, 0.0f, m_handle, false, false);
			}
#ifdef _DEBUG
			auto enemyId = GetChipId(MapLayer_enemy + m_stage, chipX, chipY);
			if (enemyId != 0)
			{
				int size = Game::ChipSize / 2;
				int X = static_cast<int>((chipX * Game::ChipSize + size) + m_camera.x);
				int Y = static_cast<int>((chipY * Game::ChipSize + size) + m_camera.y);
				//画面外を表示しない
				if (X < Game::kMapScreenLeftX - size) continue;
				if (Y < Game::kMapScreenTopY - size)continue;
				if (X > Game::kMapScreenRightX + size) continue;
				if (Y > Game::kMapScreenBottomY + size) continue;
				DrawFormatString(X, Y, 0x000000, L"%d", enemyId);
			}
#endif
		}
	}

#ifdef _DEBUG
	DrawFormatString(0,140, 0x000000, L"m_camera.x%3f,y%3f", m_camera.x, m_camera.y);
#endif
}

void Map::Movement(Vector2 vec)
{
	int size = Game::ChipSize / 2;
	int X = static_cast<int>((m_mapHeight * Game::ChipSize + size) + m_camera.x);
	int Y = static_cast<int>((m_mapWidth * Game::ChipSize + size) + m_camera.y);
	//画面外を表示しない
	if (Y < Game::kMapScreenTopY - size)return;
	m_camera += vec;
}

void Map::SetPos(Position2 pos)
{
	m_camera.x = pos.x;
}

void Map::Load(const wchar_t* filePath)
{
	struct Header
	{
		char id[4];					//　ファイル識別子(FMF_)
		unsigned int size;			//　ヘッダをのぞいたデータサイズ
		unsigned int mapWidth;		//　マップの横幅
		unsigned int mapHeight;		//　マップの縦幅
		unsigned char chipWidth;	//　パーツ(セル1個)の横幅
		unsigned char chipHeight;	//　パーツ(セル1個)の縦幅
		unsigned char layerCount;	//　レイヤー数
		unsigned char bitCount;		//　1セル当たりのビット数(÷8でバイト数)

	};
	Header header;
	int handle = FileRead_open(filePath);
	FileRead_read(&header, sizeof(header), handle);
	m_mapWidth = header.mapWidth;
	m_mapHeight = header.mapHeight;
	int layerCount = header.layerCount;
	//レイヤー1個当たりのサイズを計算する
	//マップの幅×マップの高さ×（マップ一個当たりのバイト数
	int layerDataSize = header.mapWidth * header.mapHeight * (header.bitCount / 8);

	m_mapData.resize(layerCount);//  resizeデータを大きくしたり小さくしたり
	for (auto& layer : m_mapData)
	{
		layer.resize(layerDataSize);
		FileRead_read(layer.data(), layerDataSize, handle);
	}

	FileRead_close(handle);

	//敵を表示できるかどうか
	for (int chipY = 0; chipY < m_mapHeight; chipY++)
	{
		for (int chipX = 0; chipX < m_mapWidth; chipX++)
		{
			int chipId = GetChipId(MapLayer_enemy + m_stage, chipX, chipY);
			m_enemyPos.push_back(chipId);
		}
	}
}

const MapData_t& Map::GetMapData() const
{
	return m_mapData;
}

const int Map::GetChipId(int layerId, int chipX, int chipY) const
{
	auto address = chipY * m_mapWidth + chipX;
	return m_mapData[layerId][address];
}

void Map::GetMapSize(int& width, int& height)
{
	width = m_mapWidth;
	height = m_mapHeight;
}

int Map::GetMapChipParam(float X, float Y)
{
	int x, y;

	// 整数値へ変換
	x = (int)(X) / Game::ChipSize;
	y = (int)(Y) / Game::ChipSize;

	// マップからはみ出ていたら 0 を返す
	if (x >= m_mapWidth || y >= m_mapHeight || x < 0 || y < 0) return 0;

	return GetChipId(MapLayer_map + m_stage, x, y);
}

int Map::GetMapEventParam(float X, float Y)
{
	int x, y;

	// 整数値へ変換
	x = (int)(X) / Game::ChipSize;
	y = (int)(Y) / Game::ChipSize;

	// マップからはみ出ていたら 0 を返す
	if (x >= m_mapWidth || y >= m_mapHeight || x < 0 || y < 0) return 0;

	return GetChipId(MapLayer_event + m_stage, x, y);
}
