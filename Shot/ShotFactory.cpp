#include "ShotFactory.h"
#include "../Util/DrawFunctions.h"
#include "RockBuster.h"

ShotFactory::ShotFactory()
{
	m_shotHTable[ShotType::RockBuster] = my::MyLoadGraph(L"Data/rockBuster.png");
}

ShotFactory::~ShotFactory()
{
}

void ShotFactory::Create(ShotType type, const Position2& pos, const Vector2& vel,bool isleft)
{
	switch (type)
	{
	case ShotType::RockBuster:
		m_shots.push_back(std::make_shared<RockBuster>(m_shotHTable[ShotType::RockBuster]));
		m_shots.back()->Start(pos, isleft);
		break;
	default:
		break;
	}
}

const ShotList_t& ShotFactory::GetShot() const
{
	return m_shots;
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
