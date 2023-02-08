#pragma once
#include <memory>
#include <list>
#include <map>
#include "../Util/Geometry.h"

class Player;
class EnemyBase;

//敵種類
enum class EnemyType
{
	widthMove,//横移動する敵
};

class EnemyFactory
{
public:
	EnemyFactory(std::shared_ptr<Player>player);
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

	std::map<EnemyType, int>m_handleMap;//敵のハンドル
	int m_frame = 0;
};

