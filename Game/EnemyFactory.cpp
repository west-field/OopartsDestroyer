#include "EnemyFactory.h"
#include <DxLib.h>
#include "../game.h"
#include "../Util/DrawFunctions.h"
#include "../Enemy/EnemyBase.h"
#include "../Enemy/EnemyMoveLeft.h"
#include "../Enemy/EnemyBattery.h"
#include "../Enemy/EnemyJump.h"
#include "../Enemy/EnemyMoveLR.h"
#include "../Enemy/Boss.h"
#include "Player.h"

EnemyFactory::EnemyFactory(std::shared_ptr<Player>player, std::shared_ptr<ShotFactory> sFactory, std::shared_ptr<ItemFactory> itFactory, std::shared_ptr<HpBar> hp):
	m_player(player), m_shotFactory(sFactory),m_itemFactory(itFactory),m_hp(hp), 
	m_burstHandle(-1), m_bossBurstHandle(-1)
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
	for (auto& handle : m_handleMap)
	{
		DeleteGraph(handle.second);
	}
	DeleteGraph(m_burstHandle);
	DeleteGraph(m_bossBurstHandle);
}

void EnemyFactory::Update()
{
	//いなくなった敵は消える
	auto rmIt = std::remove_if(m_enemies.begin(), m_enemies.end(),
		[](const std::shared_ptr<EnemyBase>& enemy) {
			return !enemy->IsExist();
		});
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
		if (enemy->IsExist())
		{
			enemy->Draw();
		}
	}
#ifdef _DEBUG
	DrawFormatString(0, 320, 0xffffff, L"敵の数%d", m_enemies.size());
#endif

}
//敵オブジェクトの生成
std::shared_ptr<EnemyBase> EnemyFactory::Create(EnemyType type, const Position2 pos)
{
	//EnemyTypeによって作るものを決める
	switch (type)
	{
	case EnemyType::MoveLeft:
		m_enemies.push_back(
			std::make_shared<EnemyMoveLeft>(
				m_player, pos, m_handleMap[EnemyType::MoveLeft], m_burstHandle,m_shotFactory, m_itemFactory));
		break;
	case EnemyType::BatteryRight:
		m_enemies.push_back(
			std::make_shared<EnemyBattery>(
				m_player, pos, m_handleMap[EnemyType::BatteryRight], m_burstHandle, m_shotFactory, m_itemFactory,false));
		break;
	case EnemyType::BatteryLeft:
		m_enemies.push_back(
			std::make_shared<EnemyBattery>(
				m_player, pos, m_handleMap[EnemyType::BatteryRight], m_burstHandle, m_shotFactory, m_itemFactory,true));
		break;
	case EnemyType::Jump:
		m_enemies.push_back(
			std::make_shared<EnemyJump>(
				m_player, pos, m_handleMap[EnemyType::Jump], m_burstHandle, m_shotFactory, m_itemFactory));
		break;
	case EnemyType::MoveLeftRight:
		m_enemies.push_back(
			std::make_shared<EnemyMoveLR>(
				m_player, pos, m_handleMap[EnemyType::MoveLeftRight], m_burstHandle, m_shotFactory, m_itemFactory));
		break;
	case EnemyType::Boss:
		m_enemies.push_back(
			std::make_shared<Boss>(
				m_player, pos, m_handleMap[EnemyType::Boss], m_bossBurstHandle, m_burstHandle, m_shotFactory, m_itemFactory,m_hp));
		break;
	default:
		break;
	}
	return m_enemies.back();//最後尾の要素を返す
}
//エネミーすべてを取得する
std::list<std::shared_ptr<EnemyBase>>& EnemyFactory::GetEnemies()
{
	return m_enemies;
}