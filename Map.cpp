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
			//�\������ʒu
			int size = Game::ChipSize / 2;
			//�E���W�A�����W
			Vector2 pos = { ((chipX * Game::ChipSize + size) + m_camera.x) ,((chipY * Game::ChipSize + size) + m_camera.y) };

			//�\����ʓ��ɂ��Ȃ��Ƃ��͐������Ȃ�
			if (pos.x < Game::kMapScreenLeftX - Game::ChipSize)	continue;
			if (pos.x > Game::kMapScreenRightX + Game::ChipSize)	continue;
			if (pos.y < Game::kMapScreenTopY - Game::ChipSize)	continue;
			if (pos.y > Game::kMapScreenBottomY + Game::ChipSize)continue;

			auto chipId = GetChipId(MapLayer_enemy + m_stage, chipX, chipY);
			//�����Ȃ��Ƃ��̓��[�v�𔲂���
			if (chipId == 0)	continue;

			//��x���������玟�͐������Ȃ�

			//��ʓ��ɓ�����������
			//mapX��Game::kMapScreenRightX����Game::kMapScreenRightX + Game::ChipSize�̒��ɓ�������
			
			auto address = chipY * m_mapWidth + chipX;
#ifdef _DEBUG
			DrawFormatString(10, 800, 0x000000, L"�}�b�v%d,�G�l�~�[�ʒu%d", chipId, m_enemyPos[address]);
#endif
			//��ʂɓ��������x��������
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

				//��ʊO��\�����Ȃ�
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
				//��ʊO��\�����Ȃ�
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
	//��ʊO��\�����Ȃ�
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
		char id[4];					//�@�t�@�C�����ʎq(FMF_)
		unsigned int size;			//�@�w�b�_���̂������f�[�^�T�C�Y
		unsigned int mapWidth;		//�@�}�b�v�̉���
		unsigned int mapHeight;		//�@�}�b�v�̏c��
		unsigned char chipWidth;	//�@�p�[�c(�Z��1��)�̉���
		unsigned char chipHeight;	//�@�p�[�c(�Z��1��)�̏c��
		unsigned char layerCount;	//�@���C���[��
		unsigned char bitCount;		//�@1�Z��������̃r�b�g��(��8�Ńo�C�g��)

	};
	Header header;
	int handle = FileRead_open(filePath);
	FileRead_read(&header, sizeof(header), handle);
	m_mapWidth = header.mapWidth;
	m_mapHeight = header.mapHeight;
	int layerCount = header.layerCount;
	//���C���[1������̃T�C�Y���v�Z����
	//�}�b�v�̕��~�}�b�v�̍����~�i�}�b�v�������̃o�C�g��
	int layerDataSize = header.mapWidth * header.mapHeight * (header.bitCount / 8);

	m_mapData.resize(layerCount);//  resize�f�[�^��傫�������菬����������
	for (auto& layer : m_mapData)
	{
		layer.resize(layerDataSize);
		FileRead_read(layer.data(), layerDataSize, handle);
	}

	FileRead_close(handle);

	//�G��\���ł��邩�ǂ���
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

	// �����l�֕ϊ�
	x = (int)(X) / Game::ChipSize;
	y = (int)(Y) / Game::ChipSize;

	// �}�b�v����͂ݏo�Ă����� 0 ��Ԃ�
	if (x >= m_mapWidth || y >= m_mapHeight || x < 0 || y < 0) return 0;

	return GetChipId(MapLayer_map + m_stage, x, y);
}

int Map::GetMapEventParam(float X, float Y)
{
	int x, y;

	// �����l�֕ϊ�
	x = (int)(X) / Game::ChipSize;
	y = (int)(Y) / Game::ChipSize;

	// �}�b�v����͂ݏo�Ă����� 0 ��Ԃ�
	if (x >= m_mapWidth || y >= m_mapHeight || x < 0 || y < 0) return 0;

	return GetChipId(MapLayer_event + m_stage, x, y);
}
