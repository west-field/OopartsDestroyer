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

//敵種類
enum class EnemyType
{
	MoveUpDown,//横移動する敵
	Battery,//固定砲台
};

class EnemyFactory
{
public:
	EnemyFactory(std::shared_ptr<Player>player, std::shared_ptr<ShotFactory> sFactory);
	virtual ~EnemyFactory();

	void Update();
	void Draw();

	/// <summary>
	/// 敵オブジェクトの生成
	/// </summary>
	/// <param name="type">敵種類</param>
	/// <param name="pos">生成位置</param>
	/// <returns>敵オブジェクト</returns>
	std::shared_ptr<EnemyBase> Create(EnemyType type, const Position2 pos);

	std::list<std::shared_ptr<EnemyBase>>& GetEnemies();
private:
	std::list<std::shared_ptr<EnemyBase>> m_enemies;
	std::shared_ptr<Player> m_player;
	std::shared_ptr<ShotFactory> m_shotFactory;

	std::map<EnemyType, int>m_handleMap;//敵のハンドル
	int m_frame = 0;
};

