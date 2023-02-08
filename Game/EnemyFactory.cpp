#include "EnemyFactory.h"
#include <DxLib.h>
#include <algorithm>//remove_ifを使うためにインクルードする
#include "../game.h"
#include "../Util/DrawFunctions.h"
#include "EnemyBase.h"
#include "EnemyMoveSide.h"

EnemyFactory::EnemyFactory(std::shared_ptr<Player>player):m_player(player)
{
	m_handleMap[EnemyType::widthMove] = my::MyLoadGraph(L"Data/enemy1.png");
}
EnemyFactory::~EnemyFactory()
{

}

void EnemyFactory::Update()
{
	m_frame++;

	if (m_frame % 180 == 0)
	{
		Create(EnemyType::widthMove, { Game::kScreenWidth,400.0f });
	}

	//いなくなった敵は消える
	//条件に当てはまる敵を後ろによけている 
	//remove_if条件に合致したものを消す begin,end 対象はenemise_の最初から最後まで 消えてもらう条件を表すラムダ式 true消えるfalseだと消えない
	auto rmIt = std::remove_if(m_enemies.begin(), m_enemies.end(),
		[](const std::shared_ptr<EnemyBase>& enemy) {
			return !enemy->IsExist();
		});
	//remove系の関数は不要なものを後ろによけて、よけた場所をイテレータとして返す。

	//実際に範囲を指定して消している
	m_enemies.erase(rmIt, m_enemies.end());

	//敵全部更新
	for (auto& enemy : m_enemies)
	{
		if (enemy->IsExist())
		{
			enemy->Update();
		}
	}

}
void EnemyFactory::Draw()
{
	for (auto& enemy : m_enemies)
	{
		if (enemy->IsExist())
		{
			enemy->Draw();
		}
	}
}

std::shared_ptr<EnemyBase> EnemyFactory::Create(EnemyType type, const Position2 pos)
{
	switch (type)
	{
	case EnemyType::widthMove:
		m_enemies.push_back(
			std::make_shared<MoveSide>(
				m_player, pos, m_handleMap[EnemyType::widthMove]));
		break;
	default:
		break;
	}
	return m_enemies.back();
}

std::list<std::shared_ptr<EnemyBase>>& EnemyFactory::GetEnemies()
{
	return m_enemies;
}