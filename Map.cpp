#include "Map.h"
#include <DxLib.h>
#include <cassert>
#include "game.h"
#include "Util/DrawFunctions.h"
#include "Game/EnemyFactory.h"

namespace
{
	constexpr int kTrapImgWidth = 100;//画像サイズX
	constexpr int kTrapImgHeight = 100;//画像サイズY
	constexpr float kTrapDrawScale = 1.0f;//拡大率
	constexpr int kTrapAnimNum = 61;//アニメーション枚数
	constexpr int kTrapAnimSpeed = 1;//アニメーションスピード
}

Map::Map(std::shared_ptr<EnemyFactory> enemyFactory, int stage) :
	m_handle(-1), m_camera(), m_enemies(enemyFactory), m_stage(stage),
	m_mapWidth(0), m_mapHeight(0)
{
	m_handle = my::MyLoadGraph(L"Data/map/mapchip.png");
	m_trapH = my::MyLoadGraph(L"Data/trap.png");
}

Map::~Map()
{
	DeleteGraph(m_handle);
	DeleteGraph(m_trapH);
}

void Map::Update()
{
	for (int chipY = 0; chipY < m_mapHeight; chipY++)
	{
		for (int chipX = 0; chipX < m_mapWidth; chipX++)
		{
			//表示する位置
			int size = Game::kDrawSize / 2;
			//右座標、下座標
			Vector2 pos = { ((chipX * Game::kDrawSize + size) + m_camera.x) ,((chipY * Game::kDrawSize + size) + m_camera.y) };

			//表示画面内にいないときは生成しない
			if (pos.x < Game::kMapScreenLeftX - Game::kDrawSize)	continue;
			if (pos.x > Game::kMapScreenRightX + Game::kDrawSize)	continue;
			if (pos.y < Game::kMapScreenTopY - Game::kDrawSize)	continue;
			if (pos.y > Game::kMapScreenBottomY + Game::kDrawSize)continue;

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
				pos.y <= Game::kMapScreenTopY && pos.y >= Game::kMapScreenTopY - size && m_enemyPos[address] != 0 ||
				pos.y >= Game::kMapScreenBottomY && pos.y <= Game::kMapScreenBottomY + size && m_enemyPos[address] != 0)
			{
				switch (chipId)
				{
				case static_cast<int>(EnemyType::MoveLeft):
					m_enemies->Create(EnemyType::MoveLeft, Position2(pos.x, pos.y));
					break;
				case static_cast<int>(EnemyType::BatteryLeft):
					m_enemies->Create(EnemyType::BatteryLeft, Position2(pos.x, pos.y));
					break;
				case static_cast<int>(EnemyType::BatteryRight):
					m_enemies->Create(EnemyType::BatteryRight, Position2(pos.x, pos.y));
					break;
				case static_cast<int>(EnemyType::Jump):
					m_enemies->Create(EnemyType::Jump, Position2(pos.x, pos.y));
					break;
				case static_cast<int>(EnemyType::MoveLeftRight):
					m_enemies->Create(EnemyType::MoveLeftRight, Position2(pos.x, pos.y));
					break;
					/*case static_cast<int>(EnemyType::MoveShot):
						m_enemies->Create(EnemyType::MoveShot, Position2(pos.x, pos.y));
						break;*/
				case static_cast<int>(EnemyType::Boss):
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

	m_trapIdx++;
	if (m_trapIdx == kTrapAnimNum * kTrapAnimSpeed)
	{
		m_trapIdx = 0;
	}

}

void Map::Draw()
{
	for (int chipY = 0; chipY < m_mapHeight; chipY++)
	{
		for (int chipX = 0; chipX < m_mapWidth; chipX++)
		{
			int size = Game::kDrawSize / 2;
			int X = static_cast<int>((chipX * Game::kDrawSize + size) + m_camera.x);
			int Y = static_cast<int>((chipY * Game::kDrawSize + size) + m_camera.y);
			//画面外を表示しない
			if (X < Game::kMapScreenLeftX - size) continue;
			if (Y < Game::kMapScreenTopY - size)continue;
			if (X > Game::kMapScreenRightX + size) continue;
			if (Y > Game::kMapScreenBottomY + size) continue;
			//背景を表示
			auto bgChipId = GetChipId(static_cast<int>(MapLayer_bg + m_stage), chipX, chipY);
			if (bgChipId != 0)
			{
				DrawBox(X - Game::kDrawSize / 2, Y - Game::kDrawSize / 2, X - Game::kDrawSize / 2 + Game::kDrawSize, Y - Game::kDrawSize / 2 + Game::kDrawSize, 0xffffff, true);
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
				my::MyDrawRectRotaGraph(X, Y, (bgChipId % 16) * Game::ChipSize, (bgChipId / 16) * Game::ChipSize, Game::ChipSize, Game::ChipSize, Game::kScale, 0.0f, m_handle, true, false);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}

			auto chipId = GetChipId(static_cast<int>(MapLayer_map + m_stage), chipX, chipY);
			auto eventChipId = GetChipId(static_cast<int>(MapLayer_event + m_stage), chipX, chipY);
			if (eventChipId == 2)
			{
				int animNum = (m_trapIdx / kTrapAnimSpeed);
				if (animNum >= kTrapAnimNum)
				{
					animNum -= kTrapAnimNum;
				}
				int imgX = animNum % 8 * kTrapImgWidth;
				int imgY = animNum / 8 * kTrapImgHeight;
				my::MyDrawRectRotaGraph(X, Y, imgX, imgY, kTrapImgWidth, kTrapImgHeight, kTrapDrawScale * Game::kScale, 0.0f, m_trapH, true, false);
			}
			else if (chipId != 0)
			{
				//マップチップ表示
				my::MyDrawRectRotaGraph(X, Y, (chipId % 16) * Game::ChipSize, (chipId / 16) * Game::ChipSize, Game::ChipSize, Game::ChipSize, Game::kScale, 0.0f, m_handle, true, false);
			}

#ifdef _DEBUG
			auto enemyId = GetChipId(static_cast<int>(MapLayer_enemy + m_stage), chipX, chipY);
			if (enemyId != 0)
			{
				int size = Game::kDrawSize / 2;
				int X = static_cast<int>((chipX * Game::kDrawSize + size) + m_camera.x);
				int Y = static_cast<int>((chipY * Game::kDrawSize + size) + m_camera.y);
				//画面外を表示しない
				if (X < Game::kMapScreenLeftX - size) continue;
				if (Y < Game::kMapScreenTopY - size)continue;
				if (X > Game::kMapScreenRightX + size) continue;
				if (Y > Game::kMapScreenBottomY + size) continue;
				//敵の位置に敵の番号を表示
				DrawFormatString(X, Y, 0x000000, L"%d", enemyId);
			}
#endif
		}
	}
#ifdef _DEBUG
	DrawFormatString(0, 140, 0x000000, L"m_camera.x%3f,y%3f", m_camera.x, m_camera.y);
#endif
}

void Map::Movement(Vector2 vec)
{
	int size = Game::kDrawSize / 2;
	int X = static_cast<int>((m_mapHeight * Game::kDrawSize + size) + m_camera.x);
	int Y = static_cast<int>((m_mapWidth * Game::kDrawSize + size) + m_camera.y);
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

	EnemyPos();
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

void Map::EnemyPos()
{
	m_enemyPos.clear();
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

int Map::GetMapChipParam(float X, float Y)
{
	int x, y;

	// 整数値へ変換
	x = (int)(X) / Game::kDrawSize;
	y = (int)(Y) / Game::kDrawSize;

	// マップからはみ出ていたら 0 を返す
	if (x >= m_mapWidth || y >= m_mapHeight || x < 0 || y < 0) return 0;

	return GetChipId(static_cast<int>(MapLayer_map + m_stage), x, y);
}

int Map::GetMapEventParam(float X, float Y)
{
	int x, y;

	// 整数値へ変換
	x = (int)(X) / Game::kDrawSize;
	y = (int)(Y) / Game::kDrawSize;

	// マップからはみ出ていたら 0 を返す
	if (x >= m_mapWidth || y >= m_mapHeight || x < 0 || y < 0) return 0;

	return GetChipId(static_cast<int>(MapLayer_event + m_stage), x, y);
}

Vector2 Map::GetMapChipPos(float X, float Y)
{
	int x, y;

	// 整数値へ変換
	x = (int)(X) / Game::kDrawSize;
	y = (int)(Y) / Game::kDrawSize;

	// マップからはみ出ていたら 0 を返す
	if (x >= m_mapWidth || y >= m_mapHeight || x < 0 || y < 0)
	{
		x = 0;
		y = 0;
	}

	Vector2 aling = { static_cast<float>(x * Game::kDrawSize),static_cast<float>(y * Game::kDrawSize) };

	return aling;
}
