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
