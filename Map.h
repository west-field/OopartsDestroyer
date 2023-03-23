#pragma once
#include "game.h"
#include "Util/Geometry.h"
#include <memory>
#include <vector>

class EnemyFactory;

enum MapLayer
{
	MapLayer_bg,	//背景
	MapLayer_map,	//マップ
	MapLayer_enemy,	//エネミー
	MapLayer_event	//当たり判定
};

//マップの広さ×レイヤー数なので、2つのベクターを入れ子にしている
using MapData_t = std::vector<std::vector<unsigned char>>;

class Map
{
public:
	Map(std::shared_ptr<EnemyFactory> enemyFactory, int stage);
	virtual ~Map();
	void Update();
	void Draw();

	/// <summary>
	/// 画面移動
	/// </summary>
	/// <param name="vec">画面移動</param>
	void Movement(Vector2 vec);
	const Position2 GetPos()const { return m_camera; }
	void SetPos(Position2 pos);

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
	const int GetChipId(int layerId, int chipX, int chipY)const;

	/// <summary>
	/// マップのサイズを入手する
	/// </summary>
	/// <param name="width"></param>
	/// <param name="height"></param>
	void GetMapSize(int& width, int& height);

	/// <summary>
	/// 画面スクロールの後もう一度敵を表示させる
	/// </summary>
	void EnemyPos();

	//マップチップの値を取得する関数
	int GetMapChipParam(float X, float Y);
	//イベントチップの値を取得する関数
	int GetMapEventParam(float X, float Y);
	//マップチップの値がある位置を返す
	Vector2 GetMapChipPos(float X, float Y);
private:
	MapData_t m_mapData;//マップデータ
	int m_mapWidth;//マップの横幅
	int m_mapHeight;//マップの縦幅

	Position2 m_camera;//移動

	int m_stage;//

	int m_handle;//グラフィック

	int m_trapH = -1;
	int m_trapIdx = 0;

	std::vector<int> m_enemyPos;
	//std::vector<bool> m_enemyPos;
	std::shared_ptr<EnemyFactory> m_enemies;
	int m_frame = 0;
	int num = 0;
};


