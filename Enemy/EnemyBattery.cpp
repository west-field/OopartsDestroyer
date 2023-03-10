#include "EnemyBattery.h"
#include <DxLib.h>
#include "../Util/DrawFunctions.h"
#include "../game.h"
#include "../Game/HpBar.h"
#include "../Game/ShotFactory.h"
#include "../Game/Player.h"
#include "../Util/Sound.h"

namespace
{
	constexpr int kBatteryTouchAttackPower = 1;//�ڐG�������̍U����

	constexpr int anim_frame_speed = 20;//�ꖇ�ɕK�v�ȃt���[����
	constexpr int anim_frame_num = 5;//�A�j���[�V��������
	constexpr int kSize = 32;
	constexpr float kDrawScall = 1.0f;

	constexpr int burst_img_width = 32;//�摜�T�C�YX
	constexpr int burst_img_height = 32;//�摜�T�C�YY
	constexpr float burst_draw_scale = 1.0f;//�g�嗦
	constexpr int burst_frame_num = 8;//�A�j���[�V��������
	constexpr int burst_frame_speed = 5;//�A�j���[�V�����X�s�[�h
}

EnemyBattery::EnemyBattery(std::shared_ptr<Player>player, const Position2 pos, int handle, int burstH, std::shared_ptr<ShotFactory> sFactory,bool isLeft):
	EnemyBase(player,pos,sFactory),m_updateFunc(&EnemyBattery::NormalUpdate),m_drawFunc(&EnemyBattery::NormalDraw)
{
	m_handle = handle;
	m_burstHandle = burstH;
	//��`�ƃT�C�Y
	m_rect = { pos, { static_cast<int>(kSize * Game::kScale * kDrawScall),static_cast<int>(kSize * Game::kScale * kDrawScall) } };
	
	m_hp->MaxHp(1);//���̓G�̃}�b�N�XHP
	m_isLeft = isLeft;

	for (int i = 0; i < kShotNum; i++)
	{
		m_shot[i].angle = i * 0.01f;
	}
}

EnemyBattery::~EnemyBattery()
{
	
}

void EnemyBattery::Update()
{
	//���݂��Ă��Ȃ��Ƃ��͍X�V���Ȃ�
	if (!m_isExist) return;
	(this->*m_updateFunc)();
}

void EnemyBattery::Draw()
{
	//���݂��Ă��Ȃ��Ƃ��͕\�����Ȃ�
	if (!m_isExist) return;
	(this->*m_drawFunc)();
}

void EnemyBattery::Movement(Vector2 vec)
{
	if (!m_isExist) return;

	m_rect.center += vec;
}

int EnemyBattery::TouchAttackPower() const
{
	return kBatteryTouchAttackPower;
}

void EnemyBattery::Damage(int damage)
{
	m_hp->Damage(damage);
	//m_ultimateTimer = kUltimateFrame;//���G����
	if (m_hp->GetHp() == 0)
	{
		SoundManager::GetInstance().Play(SoundId::EnemyBurst);
		m_updateFunc = &EnemyBattery::BurstUpdate;
		m_drawFunc = &EnemyBattery::BurstDraw;
		m_idx = 0;
	}
}

bool EnemyBattery::IsCollidable() const
{
	return (m_updateFunc != &EnemyBattery::BurstUpdate);
}

void EnemyBattery::NormalUpdate()
{
	m_idx = (m_idx + (GetRand(10) % 3)) % (anim_frame_speed * anim_frame_num);

	if (m_idx / anim_frame_speed == 2 && num == 0)
	{
		/*m_shot[num].vel.x = -2.0f;
		m_shot[num].vel.y = -sin(m_shot[num].angle);
		m_shot[num].vel.Normalize();
		Vector2 vel =  m_shot[num].vel;*/

		Vector2 vel = { 0.0f,0.0f };
		//�����_���ɕ����Ƒ��x�����肷��
		vel.x = -1.0f;
		vel.y = static_cast<float>(GetRand(100) - 50) / 50.0f;
		vel.Normalize();
		//float speed = static_cast<float>(GetRand(60) + 20) / 20.0f;
		//vel *= speed;

		m_shotFactory->Create(ShotType::ShotBattery, m_rect.center, vel, m_isLeft,false);//�΂�
		num++;
	}
	else if (m_idx / anim_frame_speed == 0)
	{
		num = 0;
	}
}

void EnemyBattery::NormalDraw()
{
	int imgX = (m_idx / anim_frame_speed) * kSize;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		imgX, 0, kSize, kSize, kDrawScall * Game::kScale, 0.0f, m_handle, true, m_isLeft);
#ifdef _DEBUG
	m_rect.Draw(0xff0000);
	DrawFormatString(0, 360, 0xffffff, L"�O���t�B�b�N�؂�ւ�%d", m_idx / anim_frame_speed);
#endif
}

void EnemyBattery::BurstUpdate()
{
	m_idx++;
	if (m_idx == burst_frame_num * burst_frame_speed)
	{
		m_isExist = false;
	}
}

void EnemyBattery::BurstDraw()
{
	int imgX = (m_idx / burst_frame_speed) * burst_img_width;

	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		imgX, 0, burst_img_width, burst_img_height, burst_draw_scale * Game::kScale, 0.0f, m_burstHandle, true, false);
}
