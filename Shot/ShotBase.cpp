#include "ShotBase.h"
#include "DxLib.h"
#include "../game.h"

ShotBase::ShotBase(int handle) :
	m_handle(handle), m_rect({}, {}), m_vel(), m_isExist(false), 
	m_isLeft(false), m_isPlayerShot(false), m_hitDamagePower(0)
{
	int X, Y;
	GetGraphSize(m_handle, &X, &Y);
	m_rect.size = { X,Y };
}

ShotBase::~ShotBase()
{
	
}

void ShotBase::Start(Position2 pos, Vector2 vel, bool left, bool isPlayer)
{
	m_isExist = true;
	m_isLeft = left;
	m_rect.center = pos;
	m_vel = vel;
	m_isPlayerShot = isPlayer;
}

void ShotBase::Update()
{
	int wsize = m_rect.size.w / 2;
	int hsize = m_rect.size.h / 2;
	//����
	if (m_rect.center.x - wsize < Game::kMapScreenLeftX)
	{
		m_isExist = false;
	}
	//�E��
	if (m_rect.center.x + wsize > Game::kMapScreenRightX)
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

//�ړ�������
void ShotBase::Movement(Vector2 vec)
{
	//���������Ă�����A�t�����ɂ���
	if (m_isLeft) vec *= -1.0f;
	m_rect.center += vec;
}

//��`���擾
const Rect& ShotBase::GetRect() const
{
	return m_rect;
}

//�U���͂��擾����
const int ShotBase::AttackPower() const
{
	return m_hitDamagePower;
}
