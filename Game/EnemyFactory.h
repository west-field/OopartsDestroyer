#pragma once
#include <memory>
#include <list>
#include <map>
#include "../Util/Geometry.h"

class Player;
class EnemyBase;
class ShotFactory;

/// ブンビーへりー　飛行しながら近付いて、プレイヤーの近くで上下に動き回る敵。HP1、攻撃力4
/// ブラスター　壁にある赤い砲台。シャッターが開いたときに攻撃してくる。HP1、攻撃力（弾）2、（接触）1
/// カマドーマー　ロックマンがいる方向にジャンプしている　HP1,攻撃力2,接触2
///	くっつきスージー　壁、天井、床を上下、または、左右に往復　HP5,攻撃力4,接触4
///	マンブー　まっすぐ飛んで、時々顔を出し、８方向同時に弾を打つ　HP：1、攻撃力：2（弾）、1（接触）

//敵種類
enum class EnemyType
{
	MoveUpDown,//自機とX軸が同じとき上下に移動する敵
	BatteryRight,//固定砲台
	BatteryLeft,//固定砲台
	Jump,//ジャンプする敵
	MoveLeftRight,//横移動する四角い敵
	MoveShot,//移動しながら8方向に撃つ敵

	Boss,//ボス
};

class EnemyFactory
{
public:
	EnemyFactory(std::shared_ptr<Player>player, std::shared_ptr<ShotFactory> sFactory);
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
	std::list<std::shared_ptr<EnemyBase>> m_enemies;	//エネミーを作成
	std::shared_ptr<Player> m_player;					//プレイヤー
	std::shared_ptr<ShotFactory> m_shotFactory;			//ショット

	std::map<EnemyType, int>m_handleMap;//敵の画像ハンドル
	int m_burstHandle;

	int m_frame = 0;//敵作成までのフレーム数
};

