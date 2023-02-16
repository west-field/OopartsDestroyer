#pragma once
#include <vector>

//マップの広さ×レイヤー数なので、2つのベクターを入れ子にしている
using MapData_t = std::vector<std::vector<unsigned char>>;

/// <summary>
/// ステージ情報を管理するクラス
/// </summary>
class Stage
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
	MapData_t m_mapData;
	int m_mapWidth;
	int m_mapHeight;
public:
	Stage();
	virtual ~Stage();

	/// <summary>
	/// ヘッダーを読み込む
	/// </summary>
	/// <param name="filePath">読み込むファイル名</param>
	void Load(const wchar_t* filePath);

	/// <summary>
	/// マップデータの参照を返す
	/// </summary>
	/// <returns>マップデータ</returns>
	const MapData_t& GetMapData()const;

	/// <summary>
	/// マップのIDを取得する関数
	/// </summary>
	/// <param name="layerId">レイヤー</param>
	/// <param name="chipX">チップの場所横</param>
	/// <param name="chipY">チップの場所縦</param>
	/// <returns>マップのID</returns>
	const int GetChipId(int layerId,int chipX,int chipY)const;

	/// <summary>
	/// マップのサイズを入手する
	/// </summary>
	/// <param name="width"></param>
	/// <param name="height"></param>
	void GetMapSize(int& width, int& height);
};

