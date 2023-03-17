#include "EnemyBattery.h"
#include <DxLib.h>
#include "../Util/DrawFunctions.h"
#include "../game.h"
#include "../Game/HpBar.h"
#include "../Game/ShotFactory.h"
#include "../Game/ItemFactory.h"
#include "../Game/Player.h"
#include "../Util/Sound.h"

namespace
{
	constexpr int kBatteryTouchAttackPower = 1;//�ڐG�������̍U����

	//�G�l�~�[�A�j���[�V����
	constexpr int kAnimFrameSpeed = 20;//�ꖇ�ɕK�v�ȃt���[����
	constexpr int kAnimFrameNum = 5;//�A�j���[�V��������
	constexpr int kSize = 32;//�傫��
	constexpr float kDrawScall = 1.0f;//�\���g�嗦

	//�����A�j���[�V����
	constexpr int kBurstImgWidth = 32;//�摜�T�C�YX
	constexpr int kBurstImgHeight = 32;//�摜�T�C�YY
	constexpr float kBurstDrawScale = 1.0f;//�g�嗦
	constexpr int kBurstAnimNum = 8;//�A�j���[�V��������
	constexpr int kBurstAnimSpeed = 5;//�A�j���[�V�����X�s�[�h
}

EnemyBattery::EnemyBattery(std::shared_ptr<Player>player, const Position2 pos, int handle, int burstH, std::shared_ptr<ShotFactory> sFactory, std::shared_ptr<ItemFactory> itFactory,bool isLeft):
	EnemyBase(player,pos,sFactory,itFactory),m_updateFunc(&EnemyBattery::NormalUpdate),m_drawFunc(&EnemyBattery::NormalDraw)
{
	m_handle = handle;
	m_burstHandle = burstH;
	//��`�ƃT�C�Y
	m_rect = { pos, { static_cast<int>(kSize * Game::kScale * kDrawScall),static_cast<int>(kSize * Game::kScale * kDrawScall) } };
	
	m_hp->MaxHp(1);//���̓G�̃}�b�N�XHP
	m_isLeft = isLeft;
}

EnemyBattery::~EnemyBattery()
{
	
}

void EnemyBattery::Update()
{
	(this->*m_updateFunc)();
}

void EnemyBattery::Draw()
{
	(this->*m_drawFunc)();
}

int EnemyBattery::TouchAttackPower() const
{
	return kBatteryTouchAttackPower;
}

void EnemyBattery::Damage(int damage)
{
	m_hp->Damage(damage);
	
	if (m_hp->GetHp() == 0)
	{
		SoundManager::GetInstance().Play(SoundId::EnemyBurst);
		m_updateFunc = &EnemyBattery::BurstUpdate;
		m_drawFunc = &EnemyBattery::BurstDraw;
		m_idx = 0;
		//�������Ă���Ƃ��͉񕜃A�C�e���𗎂Ƃ��Ȃ�
		if (GetRand(100) % 3 == 0 && m_isLeft)
		{
			m_itemFactory->Create(ItemType::Heal, m_rect.center);
		}
		return;
	}
	SoundManager::GetInstance().Play(SoundId::EnemyHit);
}

bool EnemyBattery::IsCollidable() const
{
	//BurstUpdate�̎��͓�����Ȃ�
	return (m_updateFunc != &EnemyBattery::BurstUpdate);
}

void EnemyBattery::NormalUpdate()
{
	//�A�j���[�V����
	m_idx = (m_idx + (GetRand(10) % 3)) % (kAnimFrameSpeed * kAnimFrameNum);
	//�U�����[�V�����̎��e�𔭎˂���
	if (m_idx / kAnimFrameSpeed == 2 && m_num == 0)
	{
		Vector2 vel = { 0.0f,0.0f };
		//�����_���ɕ����Ƒ��x�����肷��i�΂�
		vel.x = -1.0f;
		vel.y = static_cast<float>(GetRand(100) - 50) / 50.0f;
		if (!m_isLeft) vel *= -1.0f;
		vel.Normalize();
		
		m_shotFactory->Create(ShotType::ShotBattery, m_rect.center, vel, m_isLeft,false);
		m_num++;
	}
	//�U�����[�V�������I�����珉��������
	else if (m_idx / kAnimFrameSpeed == 0)
	{
		m_num = 0;
	}
}

void EnemyBattery::NormalDraw()
{
	int imgX = (m_idx / kAnimFrameSpeed) * kSize;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		imgX, 0, kSize, kSize, kDrawScall * Game::kScale, 0.0f, m_handle, true, m_isLeft);
#ifdef _DEBUG
	m_rect.Draw(0xff0000);
	DrawFormatString(0, 360, 0xffffff, L"�O���t�B�b�N�؂�ւ�%d", m_idx / kAnimFrameSpeed);
#endif
}

void EnemyBattery::BurstUpdate()
{
	m_idx++;
	if (m_idx == kBurstAnimNum * kBurstAnimSpeed)
	{
		m_isExist = false;
	}
}

void EnemyBattery::BurstDraw()
{
	int imgX = (m_idx / kBurstAnimSpeed) * kBurstImgWidth;

	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		imgX, 0, kBurstImgWidth, kBurstImgHeight, kBurstDrawScale * Game::kScale, 0.0f, m_burstHandle, true, false);
}
