#include "Map.h"
#include <DxLib.h>
#include <cassert>
#include "game.h"
#include "Util/DrawFunctions.h"
#include "Game/EnemyFactory.h"
#include "Stage.h"

namespace
{
	constexpr float kScale = 1.0f;
}

Map::Map(std::shared_ptr<EnemyFactory> enemyFactory,int stage) :m_handle(-1),m_camera(),m_enemies(enemyFactory),m_stage(stage)
{
	m_stageMap = std::make_shared<Stage>();
	m_stageMap->Load(L"Data/maptest.fmf");

	m_handle = my::MyLoadGraph(L"Data/mapchip.bmp");
	//m_camera.y = ((Game::kMapChipNumY * Game::ChipSize) - (Game::kNumY * Game::ChipSize)) * -1.0f;

	int mW, mH;
	m_stageMap->GetMapSize(mW, mH);
	for (int chipY = 0; chipY < mH; chipY++)
	{
		for (int chipX = 0; chipX < mW; chipX++)
		{
			int chipId = m_stageMap->GetChipId(MapLayer_enemy + m_stage, chipX, chipY);
			if (chipId == 0)
			{
				m_enemyPos.push_back(0);
			}
			else
			{
				m_enemyPos.push_back(1);
			}
		}
	}
}

Map::~Map()
{
	DeleteGraph(m_handle);
}

void Map::Update()
{
	int mW, mH;
	m_stageMap->GetMapSize(mW, mH);
#if true
	for (int chipY = 0; chipY < mH; chipY++)
	{
		for (int chipX = 0; chipX < mW; chipX++)
		{
			//表示する位置
			int size = Game::ChipSize / 2;
			Vector2 pos = { ((chipX * Game::ChipSize + size) + m_camera.x) ,((chipY * Game::ChipSize + size) + m_camera.y) };

			//表示画面内にいないときは生成しない
			if (pos.x < Game::kMapScreenLeftX - Game::ChipSize)	continue;
			if (pos.x > Game::kMapScreenRightX + Game::ChipSize)	continue;
			if (pos.y < Game::kMapScreenTopY - Game::ChipSize)	continue;
			if (pos.y > Game::kMapScreenBottomY + Game::ChipSize)continue;

			auto chipId = m_stageMap->GetChipId(MapLayer_enemy + m_stage, chipX, chipY);
			//何もないときはループを抜ける
			if (chipId == 0)	continue;

			//一度生成したら次は生成しない

			//画面内に入った時生成
			//mapXがGame::kMapScreenRightXからGame::kMapScreenRightX + Game::ChipSizeの中に入った時
			auto address = chipY * mW + chipX;
#ifdef _DEBUG
			DrawFormatString(10, 800, 0xffffff, L"マップ%d,エネミー位置%d", chipId, m_enemyPos[address]);
#endif
			if (pos.x >= Game::kMapScreenRightX && pos.x <= Game::kMapScreenRightX + size && m_enemyPos[address] == 1 ||
				pos.y <= Game::kMapScreenTopY && pos.y >= Game::kMapScreenTopY - size && m_enemyPos[address] == 1)
			{
				switch (chipId)
				{
				case 1:
					m_enemies->Create(EnemyType::MoveUpDown, Position2(pos.x, pos.y));
					break;
				case 2:
					m_enemies->Create(EnemyType::Battery, Position2(pos.x, pos.y));
					break;
				/*case 3:
					m_enemies->Create(EnemyType::Jump, Position2(pos.x, pos.y));
					break;
				case 4:
					m_enemies->Create(EnemyType::MoveLeftRight, Position2(pos.x, pos.y));
					break;
				case 5:
					m_enemies->Create(EnemyType::MoveShot, Position2(pos.x, pos.y));
					break;*/
				default:
					break;
				}
				m_enemyPos[address] = 0;
				break;
			}
		}
	}
#else
	for (int chipY = 0; chipY < mH; chipY++)
	{
		for (int chipX = 0; chipX < mW; chipX++)
		{
			if (m_enemyPos[chipY * mH + chipX] == 0)	continue;
			//表示する位置
			int size = Game::ChipSize / 2;
			Vector2 pos = { ((chipX * Game::ChipSize + size) + m_camera.x) ,((chipY * Game::ChipSize + size) + m_camera.y) };
			//表示画面内にいないときは生成しない
			if (pos.x < Game::kMapScreenLeftX - Game::ChipSize)	continue;
			if (pos.x > Game::kMapScreenRightX + Game::ChipSize)	continue;
			if (pos.y < Game::kMapScreenTopY - Game::ChipSize)	continue;
			if (pos.y > Game::kMapScreenBottomY + Game::ChipSize)continue;

			auto chipId = m_enemyPos[chipY * mH + chipX];
			switch (chipId)
			{
			case 1:
				DrawString(0, 300, L"出現", 0xffffff);
				m_enemies->Create(EnemyType::MoveUpDown, Position2(pos.x, pos.y));
				break;
			case 2:
				DrawString(0, 300, L"出現", 0xffffff);
				m_enemies->Create(EnemyType::Battery, Position2(pos.x, pos.y));
				break;
				/*case 3:
					m_enemies->Create(EnemyType::Jump, Position2(pos.x, pos.y));
					break;
				case 4:
					m_enemies->Create(EnemyType::MoveLeftRight, Position2(pos.x, pos.y));
					break;
				case 5:
					m_enemies->Create(EnemyType::MoveShot, Position2(pos.x, pos.y));
					break;*/
			default:
				break;
			}
			m_enemyPos[chipY * mH + chipX] = 0;
		}
	}

#endif
}

void Map::Draw()
{
	int mW, mH;
	m_stageMap->GetMapSize(mW, mH);
	const auto& mapData = m_stageMap->GetMapData();
	for (int chipY = 0; chipY < mH; chipY++)
	{
		for (int chipX = 0; chipX < mW; chipX++)
		{
			auto chipId = m_stageMap->GetChipId(m_stage, chipX, chipY);
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
#if _DEBUG
			auto enemyId = m_stageMap->GetChipId(MapLayer_enemy + m_stage, chipX, chipY);
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
				DrawFormatString(X, Y, 0xffffff, L"%d", enemyId);
			}
#endif
		}
	}

#ifdef _DEBUG
	DrawFormatString(0,140, 0xffffff, L"m_camera.x%3f,y%3f", m_camera.x, m_camera.y);
#endif
}

void Map::Movement(Vector2 vec)
{
	int mW, mH;
	m_stageMap->GetMapSize(mW, mH);
	int size = Game::ChipSize / 2;
	int X = static_cast<int>((mW * Game::ChipSize + size) + m_camera.x);
	int Y = static_cast<int>((mH * Game::ChipSize + size) + m_camera.y);
	//画面外を表示しない
	if (Y < Game::kMapScreenTopY - size)return;
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

	int mW, mH;
	m_stageMap->GetMapSize(mW, mH);
	// マップからはみ出ていたら 0 を返す
	if (x >= mW || y >= mH || x < 0 || y < 0) return 0;

	return m_stageMap->GetChipId(MapLayer_map + m_stage, x, y);
}

int Map::GetMapEventParam(float X, float Y)
{
	int x, y;

	// 整数値へ変換
	x = (int)(X) / Game::ChipSize;
	y = (int)(Y) / Game::ChipSize;

	int mW, mH;
	m_stageMap->GetMapSize(mW, mH);
	// マップからはみ出ていたら 0 を返す
	if (x >= mW || y >= mH || x < 0 || y < 0) return 0;

	return m_stageMap->GetChipId(MapLayer_event + m_stage, x, y);
}
