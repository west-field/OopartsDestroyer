#include "EnemyMoveLeft.h"
#include <DxLib.h>
#include "../Util/DrawFunctions.h"
#include "../game.h"
#include "../AnimInfo.h"
#include "../Game/HpBar.h"
#include "../Game/Player.h"
#include "../Game/ItemFactory.h"
#include "../Util/Sound.h"

namespace
{
	constexpr int kTouchAttackPower = 4;//�ڐG�������̍U����
	
	//�G�l�~�[�A�j���[�V����
	constexpr float kEnemyMoveSpeed = -4.0f;//�G�l�~�[�̈ړ����x
	constexpr int kSize = 96;//�摜�T�C�YX
	constexpr float kDrawScale = 0.3f;//�g�嗦
	constexpr int kAnimFrameNum = 4;//�A�j���[�V��������
	constexpr int kAnimFrameSpeed = 20;//�A�j���[�V�����X�s�[�h
	
}

EnemyMoveLeft::EnemyMoveLeft(std::shared_ptr<Player>player, const Position2 pos, int handle, int burstH, std::shared_ptr<ShotFactory> sFactory, std::shared_ptr<ItemFactory> itFactory):
	EnemyBase(player,pos, handle, burstH, sFactory,itFactory),m_updateFunc(&EnemyMoveLeft::NormalUpdate),m_drawFunc(&EnemyMoveLeft::NormalDraw)
{
	m_hp->MaxHp(1);

	m_idx = 0;

	m_rect.size = {  static_cast<int>(kSize * Game::kScale * kDrawScale),static_cast<int>(kSize * Game::kScale * kDrawScale) };
	
	m_touchDamagePower = kTouchAttackPower;
}

EnemyMoveLeft::~EnemyMoveLeft()
{

}

void EnemyMoveLeft::Update()
{
	(this->*m_updateFunc)();
}

void EnemyMoveLeft::Draw()
{
	(this->*m_drawFunc)();
}

//�����蔻��Ώۂ��ǂ���
bool EnemyMoveLeft::IsCollidable() const
{
	//BurstUpdate�̎��͓�����Ȃ�
	return (m_updateFunc != &EnemyMoveLeft::BurstUpdate);
}

//�_���[�W���󂯂�
void EnemyMoveLeft::Damage(int damage)
{
	m_hp->Damage(damage);

	if (m_hp->GetHp() == 0)
	{
		SoundManager::GetInstance().Play(SoundId::EnemyBurst);
		m_updateFunc = &EnemyMoveLeft::BurstUpdate;
		m_drawFunc = &EnemyMoveLeft::BurstDraw;
		m_idx = 0;
		if (GetRand(100) % 3 == 0)
		{
			m_itemFactory->Create(ItemType::Heal, m_rect.center);//�񕜃A�C�e��
		}
		return;
	}
	SoundManager::GetInstance().Play(SoundId::EnemyHit);
}

//�ʏ�X�V
void EnemyMoveLeft::NormalUpdate()
{
	Movement({ kEnemyMoveSpeed ,0.0f});//�G�l�~�[�ړ�
	m_idx = (m_idx + 1) % (kAnimFrameSpeed * kAnimFrameNum);
}

//�ʏ�\��
void EnemyMoveLeft::NormalDraw()
{
	int imgX = (m_idx / kAnimFrameSpeed) * kSize;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		imgX, 0, kSize, kSize, kDrawScale * Game::kScale, 0.0f, m_handle, true, m_isLeft);
#ifdef _DEBUG
	m_rect.Draw(0xff0000);
#endif
}

//�����X�V
void EnemyMoveLeft::BurstUpdate()
{
	m_idx++;
	if (m_idx == kBurstAnimNum * kBurstAnimSpeed)
	{
		m_isExist = false;
	}
}

//�����\��
void EnemyMoveLeft::BurstDraw()
{
	int imgX = (m_idx / kBurstAnimSpeed) * kBurstImgWidth;

	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		imgX, 0, kBurstImgWidth, kBurstImgHeight, kBurstDrawScale * Game::kScale, 0.0f, m_burstHandle, true, false);
}
