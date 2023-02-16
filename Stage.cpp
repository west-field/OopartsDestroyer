#include "Stage.h"
#include <DxLib.h>

Stage::Stage():m_mapWidth(0),m_mapHeight(0)
{
}

Stage::~Stage()
{
}

void Stage::Load(const wchar_t* filePath)
{
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
}

const MapData_t& Stage::GetMapData() const
{
	return m_mapData;
}

const int Stage::GetChipId(int layerId, int chipX, int chipY) const
{
	auto address = chipY * m_mapWidth + chipX;
	return m_mapData[layerId][address];
}

void Stage::GetMapSize(int& width, int& height)
{
	width = m_mapWidth;
	height = m_mapHeight;
}
