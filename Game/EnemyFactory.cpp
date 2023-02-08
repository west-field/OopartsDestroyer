#include "EnemyFactory.h"
#include <DxLib.h>
#include <algorithm>//remove_if���g�����߂ɃC���N���[�h����
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