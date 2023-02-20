#include "EnemyFactory.h"
#include <DxLib.h>
#include <algorithm>//remove_if���g�����߂ɃC���N���[�h����
#include "../game.h"
#include "../Util/DrawFunctions.h"
#include "../Enemy/EnemyBase.h"
#include "../Enemy/EnemyMoveUpDown.h"
#include "../Enemy/EnemyBattery.h"
#include "../Enemy/EnemyJump.h"
#include "../Enemy/EnemyMoveLeftRight.h"
#include "Player.h"

namespace
{
	//�Œ�C���\���������ʒu
	constexpr int X = 53;
	constexpr int Y = 125;
}

EnemyFactory::EnemyFactory(std::shared_ptr<Player>player, std::shared_ptr<ShotFactory> sFactory, std::shared_ptr<Stage> stage):
	m_player(player), m_shotFactory(sFactory),m_stage(stage)
{
	//�G�̉摜�����[�h����
	m_handleMap[EnemyType::MoveUpDown] = my::MyLoadGraph(L"Data/enemy1.png");
	m_handleMap[EnemyType::BatteryRight] = my::MyLoadGraph(L"Data/burst.png");
	m_handleMap[EnemyType::Jump] = my::MyLoadGraph(L"Data/jump.png");
	m_handleMap[EnemyType::MoveLeftRight] = my::MyLoadGraph(L"Data/move.png");
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

	//�G�S���X�V
	for (auto& enemy : m_enemies)
	{
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
	case EnemyType::MoveUpDown:
		m_enemies.push_back(
			std::make_shared<EnemyMoveUpDown>(
				m_player, pos, m_handleMap[EnemyType::MoveUpDown], m_shotFactory, m_stage));
		break;
	case EnemyType::BatteryRight:
		m_enemies.push_back(
			std::make_shared<EnemyBattery>(
				m_player, pos, m_handleMap[EnemyType::BatteryRight], m_shotFactory, m_stage,false));
		break;
	case EnemyType::BatteryLeft:
		m_enemies.push_back(
			std::make_shared<EnemyBattery>(
				m_player, pos, m_handleMap[EnemyType::BatteryRight], m_shotFactory, m_stage,true));
		break;
	case EnemyType::Jump:
		m_enemies.push_back(
			std::make_shared<EnemyJump>(
				m_player, pos, m_handleMap[EnemyType::Jump], m_shotFactory, m_stage));
		break;
	case EnemyType::MoveLeftRight:
		m_enemies.push_back(
			std::make_shared<EnemyMoveLeftRight>(
				m_player, pos, m_handleMap[EnemyType::MoveLeftRight], m_shotFactory, m_stage));
		break;
	case EnemyType::MoveShot:
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