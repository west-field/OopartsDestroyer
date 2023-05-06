#include "Boss.h"
#include <DxLib.h>
#include "../Util/DrawFunctions.h"
#include "../Info.h"
#include "../Game/ShotFactory.h"
#include "../Game/Player.h"
#include "../Game/HpBar.h"
#include "../game.h"
#include "../Util/Sound.h"

namespace
{
	constexpr int kCutManTouchAttackPower = 4;//�ڐG�������̍U����
	
	//�{�X�A�j���[�V����
	constexpr int kGraphNum = 13;						//�{�X�摜�̐�
	constexpr int kGraphSizeWidth = 2600 / kGraphNum;	//�{�X�摜�T�C�YX
	constexpr int kGraphSizeHeight = 400;				//�{�X�摜�T�C�YY
	constexpr float kDrawScale = 0.5f;					//�{�X�摜�g�嗦
	constexpr int kAnimFrameSpeed = 5;					//�{�X�A�j���[�V�����X�s�[�h

	//�{�X�����A�j���[�V����
	constexpr int kBossBurstImgWidth = 100;		//�摜�T�C�YX
	constexpr int kBossBurstImgHeight = 100;	//�摜�T�C�YY
	constexpr float kBossBurstDrawScale = 2.0f;	//�g�嗦
	constexpr int kBossBurstAnimNum = 61;		//�A�j���[�V��������
	constexpr int kBossBurstAnimSpeed = 1;		//�A�j���[�V�����X�s�[�h
}

Boss::Boss(std::shared_ptr<Player>player, const Position2& pos, int handle, int bossBurstH, int burstH, std::shared_ptr<ShotFactory> sFactory, std::shared_ptr<ItemFactory> itFactory, std::shared_ptr<HpBar>hp) :
	EnemyBase(player, pos,handle, burstH, sFactory,itFactory), m_updateFunc(&Boss::NormalUpdate), m_drawFunc(&Boss::NormalDraw),
	m_bossBurstH(-1), m_idx(0), m_animFrame(0),
	m_frame(0), m_shotFrame(0)
{
	m_hp = hp;
	m_hp->MaxHp(50);

	m_isLeft = true;

	m_bossBurstH= bossBurstH;

	m_rect.center.y -= 8.0f;
	m_rect.size = { static_cast<int>(kGraphSizeWidth * Game::kScale * kDrawScale / 2) - 20,static_cast<int>(kGraphSizeHeight * Game::kScale * kDrawScale) - 20 };

	m_touchDamagePower = kCutManTouchAttackPower;
	m_isBoss = true;
}

Boss::~Boss()
{
	DeleteGraph(m_bossBurstH);
}

void Boss::Update()
{
	(this->*m_updateFunc)();
}

void Boss::Draw()
{
	(this->*m_drawFunc)();
}

// �����蔻��Ώۂ��ǂ���
bool Boss::IsCollidable() const
{
	//BurstUpdate�łȂ������G���Ԃ�0�̎�������
	return (m_updateFunc != &Boss::BurstUpdate);
}

// �_���[�W���󂯂�
void Boss::Damage(int damage)
{
	m_hp->Damage(damage);
	m_isOnDamage = true;
	if (m_hp->GetHp() == 0)
	{
		SoundManager::GetInstance().Play(SoundId::EnemyBurst);
		m_updateFunc = &Boss::BurstUpdate;
		m_drawFunc = &Boss::BurstDraw;
		m_idx = 0;
		return;
	}
	SoundManager::GetInstance().Play(SoundId::EnemyHit);
}

//�ʏ�X�V
void Boss::NormalUpdate()
{
	//�A�j���[�V����
	if (m_animFrame++ > kAnimFrameSpeed)
	{
		m_idx = (m_idx + 1) % kGraphNum;
		m_animFrame = 0;
	}

	//1�b���Ƃɒe��ł�
	if (m_frame++ == 60)
	{
		m_frame = 0;
		m_isOnDamage = false;

		//HP��3����2��؂�������U������
		if (m_hp->GetHp() <= (m_hp->GetMaxHp() / 3) * 2)
		{
			m_shotFrame = 20;
		}
		else
		{
			m_shotFrame = 10;
		}
		m_updateFunc = &Boss::ShotUpdate;
	}
}

//�ʏ�\��
void Boss::NormalDraw()
{
	//�{�X�\��
	int img = m_idx * kGraphSizeWidth;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		img, 0, kGraphSizeWidth, kGraphSizeHeight, kDrawScale * Game::kScale, 0.0f, m_handle, true, m_isLeft);
	if (m_isOnDamage)
	{
		//�_���[�W���󂯂��Ƃ��_�ł�����
		SetDrawBlendMode(DX_BLENDMODE_ADD, 255);//���Z����
		my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
			img, 0, kGraphSizeWidth, kGraphSizeHeight, kDrawScale * Game::kScale, 0.0f, m_handle, true, m_isLeft);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//���Ƃɖ߂�
		m_isOnDamage = false;
	}
#ifdef _DEBUG
	DrawFormatString(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y), 0xffffff, L"%d", m_idx);
	m_rect.Draw(0xaaffaa);
#endif
}

//�����X�V
void Boss::BurstUpdate()
{
	//�����A�j���[�V����
	m_idx += 1;
	if (m_idx == (kBossBurstAnimNum * 2) * kBossBurstAnimSpeed)
	{
		//�����A�j���[�V�������I������瑶�݂�����
		m_isExist = false;
	}
}

//�����\��
void Boss::BurstDraw()
{
	//�ʏ픚���G�t�F�N�g���d�˂ĕ\��
	int animNum = (m_idx / kBurstAnimSpeed);
	int imgX = (animNum % kBurstAnimNum) * kBurstImgWidth;
	int imgY = 0;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x + 20), static_cast<int>(m_rect.center.y + 50),
		imgX, imgY, kBurstImgWidth, kBurstImgHeight, kBurstDrawScale * Game::kScale, 0.0f, m_burstHandle, true, false);
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x - 40), static_cast<int>(m_rect.center.y + 50),
		imgX, imgY, kBurstImgWidth, kBurstImgHeight, kBurstDrawScale * Game::kScale, 0.0f, m_burstHandle, true, false);
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x + 20), static_cast<int>(m_rect.center.y - 50),
		imgX, imgY, kBurstImgWidth, kBurstImgHeight, kBurstDrawScale * Game::kScale, 0.0f, m_burstHandle, true, false);
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x - 40), static_cast<int>(m_rect.center.y - 50),
		imgX, imgY, kBurstImgWidth, kBurstImgHeight, kBurstDrawScale * Game::kScale, 0.0f, m_burstHandle, true, false);

	//�{�X�����G�t�F�N�g��\��
	animNum = (m_idx / kBossBurstAnimSpeed);
	if (animNum >= kBossBurstAnimNum)
	{
		animNum -= kBossBurstAnimNum;
	}
	imgX = animNum % 8 * kBossBurstImgWidth;
	imgY = animNum / 8 * kBossBurstImgHeight;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		imgX, imgY, kBossBurstImgWidth, kBossBurstImgHeight, kBossBurstDrawScale * Game::kScale, 0.0f, m_bossBurstH, true, false);
#ifdef _DEBUG
	DrawFormatStringF(m_rect.center.x, m_rect.center.y, 0x000000, L"%d", animNum);
	DrawFormatStringF(m_rect.center.x, m_rect.center.y - 20, 0x000000, L"X%d,Y%d", imgX / kBossBurstImgWidth, imgY / kBossBurstImgHeight);
#endif
}

//�{�X���e������
void Boss::ShotUpdate()
{
	//���@�_���e�����@���@�_���x�N�g��=�I�_(�v���C���[���W)�@-�@�n�_(�G�@���g�̍��W)
	auto vel = m_player->GetRect().GetCenter() - m_rect.center;

	if (vel.SQLength() == 0.0f)
	{
		vel = { -2.0f,0.0f };//�G�@�̎������d�Ȃ��Ă���Ƃ��͊���̕����@�^���ɔ�΂��Ă���
	}
	else
	{
		vel.Normalize();
	}
	//�V���b�g��ł�
	if (m_shotFrame-- % 10 == 0)
	{
		m_shotFactory->Create(ShotType::ShotBattery, m_rect.center, vel, m_isLeft, false);
	}

	if (m_shotFrame <= 0)
	{
		//���̎w����҂�
		m_updateFunc = &Boss::NormalUpdate;
		return;
	}
}
