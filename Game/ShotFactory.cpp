#include "ShotFactory.h"
#include "../Util/DrawFunctions.h"
#include "../Util/Sound.h"

#include "../Shot/RockBuster.h"
#include "../Shot/ShotBattery.h"

ShotFactory::ShotFactory()
{
	m_shotHTable[ShotType::RockBuster] = my::MyLoadGraph(L"Data/rockBuster.png");
	m_shotHTable[ShotType::ShotBattery] = my::MyLoadGraph(L"Data/burstShot.png");
}

ShotFactory::~ShotFactory()
{
}

void ShotFactory::Update()
{
	//ïsóvÇ…Ç»Ç¡ÇΩíeÇÃçÌèú
	auto rmIt = std::remove_if(m_shots.begin(), m_shots.end(),
		[](const std::shared_ptr<ShotBase>& shotbase) {
			return !shotbase->IsExist();
		});
	m_shots.erase(rmIt, m_shots.end());

	for (auto& shot : m_shots)
	{
		shot->Update();
	}
}

void ShotFactory::Draw()
{
	for (auto& shot : m_shots)
	{
		shot->Draw();
	}
}

void ShotFactory::Movement(Vector2 vel)
{
	for (auto& shot : m_shots)
	{
		shot->Movement(vel);
	}
}

void ShotFactory::Create(ShotType type, const Position2& pos, const Vector2& vel,bool isleft, bool isPlayer)
{
	SoundManager::GetInstance().Play(SoundId::EnemyShot);
	switch (type)
	{
	case ShotType::RockBuster:
		m_shots.push_back(std::make_shared<RockBuster>(m_shotHTable[ShotType::RockBuster]));
		m_shots.back()->Start(pos, vel,isleft, isPlayer);
		break;
	case ShotType::ShotBattery:
		m_shots.push_back(std::make_shared<ShotBattery>(m_shotHTable[ShotType::ShotBattery]));
		m_shots.back()->Start(pos, vel,isleft, isPlayer);
		break;
	default:
		break;
	}
}

const ShotList_t& ShotFactory::GetShot() const
{
	return m_shots;
}


