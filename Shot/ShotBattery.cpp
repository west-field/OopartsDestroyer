#include "ShotBattery.h"
#include <DxLib.h>
#include "../Util/DrawFunctions.h"

#ifdef _DEBUG
#include "../game.h"
#endif

namespace
{
	constexpr int kShotFive = 2;//�U����
	constexpr float kShotBatterySpeed = 5.0f;//�U���X�s�[�h
}

ShotBattery::ShotBattery(int handle):
	ShotBase(handle)
{
}

ShotBattery::~ShotBattery()
{
}

//�V���b�g�J�n
void ShotBattery::Start(Position2 pos, Vector2 vel, bool left, bool isPlayer)
{
	ShotBase::Start(pos, vel, left, isPlayer);

	m_vel *= kShotBatterySpeed;
	//���������Ă�����A�t�����ɂ���
	if (m_isLeft) m_vel *= -1.0f;
	//�U���͂��w�肷��
	m_hitDamagePower = kShotFive;
}

void ShotBattery::Update()
{
	Movement(m_vel);
	ShotBase::Update();
}

void ShotBattery::Draw()
{
	if (!m_isExist)	return;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		0, 0, m_rect.size.w, m_rect.size.h, 1.5f, 0.0f, m_handle, true, m_isLeft);
#ifdef _DEBUG
	m_rect.Draw(0xaa00ff);
	DrawFormatString(Game::kMapScreenRightX, Game::kMapScreenTopY, 0x000000,L"%3f,%3f", m_vel.x, m_vel.y);
#endif
}
