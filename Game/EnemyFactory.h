#pragma once
#include <memory>
#include <list>
#include <map>
#include "../Util/Geometry.h"

class Player;
class HpBar;
class EnemyBase;
class ShotFactory;
class ItemFactory;

//敵種類
enum class EnemyType
{
	no,
	MoveLeft,		//自機とX軸が同じとき上下に移動する敵
	BatteryRight,	//固定砲台
	BatteryLeft,	//固定砲台
	Jump,			//ジャンプする敵
	MoveLeftRight,	//横移動する四角い敵
	no2,

	Boss,			//ボス
};

class EnemyFactory
{
public:
	EnemyFactory(std::shared_ptr<Player>player, std::shared_ptr<ShotFactory> sFactory, std::shared_ptr<ItemFactory> itFactory, std::shared_ptr<HpBar> hp);
	virtual ~EnemyFactory();

	void Update();//更新
	void Draw();//表示

	/// <summary>
	/// 敵オブジェクトの生成
	/// </summary>
	/// <param name="type">敵種類</param>
	/// <param name="pos">生成位置</param>
	/// <returns>敵オブジェクト</returns>
	std::shared_ptr<EnemyBase> Create(EnemyType type, const Position2 pos);

	/// <summary>
	/// エネミーをすべて取得
	/// </summary>
	/// <returns>エネミーすべて</returns>
	std::list<std::shared_ptr<EnemyBase>>& GetEnemies();
private:
	std::list<std::shared_ptr<EnemyBase>> m_enemies;	//エネミー
	std::shared_ptr<Player> m_player;					//プレイヤー
	std::shared_ptr<HpBar> m_hp;						//HP
	std::shared_ptr<ShotFactory> m_shotFactory;			//ショット
	std::shared_ptr<ItemFactory> m_itemFactory;			//アイテム

	std::map<EnemyType, int>m_handleMap;//敵の画像ハンドル
	int m_burstHandle;					//爆発グラフィック
	int m_bossBurstHandle;				//ボス爆発グラフィック
};

