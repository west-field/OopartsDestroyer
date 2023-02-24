#include "EnemyBattery.h"
#include <DxLib.h>
#include "../Util/DrawFunctions.h"
#include "../game.h"
#include "../Game/HpBar.h"
#include "../Game/ShotFactory.h"
#include "../Game/Player.h"

namespace
{
	constexpr int kBatteryTouchAttackPower = 1;//�ڐG�������̍U����

	constexpr int anim_frame_speed = 20;//�ꖇ�ɕK�v�ȃt���[����
	constexpr int anim_frame_num = 5;//�A�j���[�V��������
	constexpr int kSize = 32;
	constexpr float kDrawScall = 1.0f;
}

EnemyBattery::EnemyBattery(std::shared_ptr<Player>player, const Position2 pos, int handle, std::shared_ptr<ShotFactory> sFactory,bool isLeft):
	EnemyBase(player,pos,sFactory)
{
	m_handle = handle;
	//��`�ƃT�C�Y
	m_rect = { pos, { static_cast<int>(kSize * kDrawScall),static_cast<int>(kSize* kDrawScall) } };
	
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
	m_idx = (m_idx + (GetRand(10) % 3) ) % (anim_frame_speed * anim_frame_num);

	if ( m_idx / anim_frame_speed == 2 && num == 0)
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
		float speed = static_cast<float>(GetRand(60) + 20) / 20.0f;
		vel *= speed;

		m_shotFactory->Create(ShotType::ShotBattery, m_rect.center, vel, m_isLeft);//�΂�
		num++;
	}
	else if(m_idx / anim_frame_speed == 0)
	{
		num = 0;
	}
}

void EnemyBattery::Draw()
{
	int imgX = (m_idx / anim_frame_speed) * kSize;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		imgX, 0, kSize, kSize, kDrawScall, 0.0f, m_handle, false, m_isLeft);
#ifdef _DEBUG
	m_rect.Draw(0xff0000);
	DrawFormatString(0, 360, 0xffffff, L"�O���t�B�b�N�؂�ւ�%d", m_idx / anim_frame_speed);
#endif
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
