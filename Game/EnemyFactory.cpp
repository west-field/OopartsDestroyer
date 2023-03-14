#include "EnemyFactory.h"
#include <DxLib.h>
#include <algorithm>//remove_ifを使うためにインクルードする
#include "../game.h"
#include "../Util/DrawFunctions.h"
#include "../Enemy/EnemyBase.h"
#include "../Enemy/EnemyMoveLeft.h"
#include "../Enemy/EnemyBattery.h"
#include "../Enemy/EnemyJump.h"
#include "../Enemy/EnemyMoveLR.h"
#include "../Enemy/EnemyMoveUD.h"
#include "../Enemy/CutMan.h"
#include "Player.h"

namespace
{
	//固定砲台を表示したい位置
	constexpr int X = 53;
	constexpr int Y = 125;
}

EnemyFactory::EnemyFactory(std::shared_ptr<Player>player, std::shared_ptr<ShotFactory> sFactory):
	m_player(player), m_shotFactory(sFactory)
{
	//敵の画像をロードする
	m_handleMap[EnemyType::MoveLeft] = my::MyLoadGraph(L"Data/goldenSpaceShuttle.png");
	m_handleMap[EnemyType::BatteryRight] = my::MyLoadGraph(L"Data/burst.png");
	m_handleMap[EnemyType::Jump] = my::MyLoadGraph(L"Data/jump.png");
	m_handleMap[EnemyType::MoveLeftRight] = my::MyLoadGraph(L"Data/move.png");
	m_handleMap[EnemyType::Boss] = my::MyLoadGraph(L"Data/FlyingObelisk.png");

	m_burstHandle = my::MyLoadGraph(L"Data/player_burst.png");
	m_bossBurstHandle = my::MyLoadGraph(L"Data/boss_burst.png");
}
EnemyFactory::~EnemyFactory()
{

}

void EnemyFactory::Update()
{
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

	for (auto& enemy : m_enemies)
	{
		//敵全部更新
		if (enemy->IsExist() && enemy->GetRect().GetCenter().x < Game::kMapScreenRightX + enemy->GetRect().GetSize().w / 2)
		{
			enemy->Update();
		}
	}

}
void EnemyFactory::Draw()
{
	for (auto& enemy : m_enemies)
	{
		if (enemy->IsExist())//&& enemy->GetRect().GetCenter().x < Game::kMapScreenRightX + enemy->GetRect().GetSize().w / 2)
		{
			enemy->Draw();
		}
	}
#ifdef _DEBUG
	DrawFormatString(0, 320, 0xffffff, L"敵の数%d", m_enemies.size());
#endif

}

std::shared_ptr<EnemyBase> EnemyFactory::Create(EnemyType type, const Position2 pos)
{
	//EnemyTypeによって作るものを決める
	switch (type)
	{
	case EnemyType::MoveLeft:
		m_enemies.push_back(
			std::make_shared<EnemyMoveLeft>(
				m_player, pos, m_handleMap[EnemyType::MoveLeft], m_burstHandle,m_shotFactory));
		break;
	case EnemyType::BatteryRight:
		m_enemies.push_back(
			std::make_shared<EnemyBattery>(
				m_player, pos, m_handleMap[EnemyType::BatteryRight], m_burstHandle, m_shotFactory,false));
		break;
	case EnemyType::BatteryLeft:
		m_enemies.push_back(
			std::make_shared<EnemyBattery>(
				m_player, pos, m_handleMap[EnemyType::BatteryRight], m_burstHandle, m_shotFactory,true));
		break;
	case EnemyType::Jump:
		m_enemies.push_back(
			std::make_shared<EnemyJump>(
				m_player, pos, m_handleMap[EnemyType::Jump], m_burstHandle, m_shotFactory));
		break;
	case EnemyType::MoveLeftRight:
		m_enemies.push_back(
			std::make_shared<EnemyMoveLR>(
				m_player, pos, m_handleMap[EnemyType::MoveLeftRight], m_burstHandle, m_shotFactory));
		break;
	case EnemyType::MoveShot:
		break;
	case EnemyType::Boss:
		m_enemies.push_back(
			std::make_shared<CutMan>(
				m_player, pos, m_handleMap[EnemyType::Boss], m_bossBurstHandle, m_burstHandle, m_shotFactory));
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