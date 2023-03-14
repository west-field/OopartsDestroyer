#include "EnemyFactory.h"
#include <DxLib.h>
#include <algorithm>//remove_if���g�����߂ɃC���N���[�h����
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
	//�Œ�C���\���������ʒu
	constexpr int X = 53;
	constexpr int Y = 125;
}

EnemyFactory::EnemyFactory(std::shared_ptr<Player>player, std::shared_ptr<ShotFactory> sFactory):
	m_player(player), m_shotFactory(sFactory)
{
	//�G�̉摜�����[�h����
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
	//���Ȃ��Ȃ����G�͏�����
	//�����ɓ��Ă͂܂�G�����ɂ悯�Ă��� 
	//remove_if�����ɍ��v�������̂����� begin,end �Ώۂ�enemise_�̍ŏ�����Ō�܂� �����Ă��炤������\�������_�� true������false���Ə����Ȃ�
	auto rmIt = std::remove_if(m_enemies.begin(), m_enemies.end(),
		[](const std::shared_ptr<EnemyBase>& enemy) {
			return !enemy->IsExist();
		});
	//remove�n�̊֐��͕s�v�Ȃ��̂����ɂ悯�āA�悯���ꏊ���C�e���[�^�Ƃ��ĕԂ��B

	//���ۂɔ͈͂��w�肵�ď����Ă���
	m_enemies.erase(rmIt, m_enemies.end());

	for (auto& enemy : m_enemies)
	{
		//�G�S���X�V
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
	DrawFormatString(0, 320, 0xffffff, L"�G�̐�%d", m_enemies.size());
#endif

}

std::shared_ptr<EnemyBase> EnemyFactory::Create(EnemyType type, const Position2 pos)
{
	//EnemyType�ɂ���č����̂����߂�
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