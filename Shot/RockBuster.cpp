#include "RockBuster.h"
#include <DxLib.h>
#include "../game.h"
#include "../Util/DrawFunctions.h"

namespace
{
	constexpr float kShotSpeed = 8.0f;
	constexpr int kAttackPower = 1;//���b�N�o�X�^�[�̍U����
}

RockBuster::RockBuster(int handle) : ShotBase(handle)
{
	
}

RockBuster::~RockBuster()
{
	
}

void RockBuster::Start(Position2 pos, Vector2 vel,bool left, bool isPlayer)
{
	m_isExist = true;
	m_isLeft = left;
	m_rect.center = pos;
	m_vel = vel;
	m_isPlayerShot = isPlayer;
	//if (m_isLeft) m_vel *= -1.0f;
}

void RockBuster::Update()
{
	if (!m_isExist)	return;
	Movement({ 8.0f ,0.0f});
	//��ʂ̊O�ɏo���������
	float hsize, wsize;

	// �����̃T�C�Y���Z�o
	wsize = m_rect.size.w * 0.5f;
	hsize = m_rect.size.h * 0.5f;
	//���[
	if (m_rect.center.x + wsize < Game::kMapScreenLeftX)
	{
		m_isExist = false;
	}
	//�E�[
	if (m_rect.center.x - wsize > Game::kMapScreenRightX)
	{
		m_isExist = false;
	}
	//��[
	if (m_rect.center.y + hsize < Game::kMapScreenTopY)
	{
		m_isExist = false;
	}
	//���[
	if (m_rect.center.y - hsize > Game::kMapScreenBottomY)
	{
		m_isExist = false;
	}

}

void RockBuster::Draw()
{
	if (!m_isExist)	return;
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y),
		0, 0, m_rect.size.w, m_rect.size.h, 1.5f, 0.0f, m_handle, true,m_isLeft);
#ifdef _DEBUG
	m_rect.Draw(0xaa00ff);
#endif
}

void RockBuster::Movement(Vector2 vec)
{
	if (vec.x != 8.0f)	return;
	if (m_isLeft) vec *= -1.0f;
	m_rect.center += vec;
}

const int RockBuster::AttackPower() const
{
	return kAttackPower;
}
