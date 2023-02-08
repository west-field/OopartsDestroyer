#include "ShotBase.h"
#include "DxLib.h"

namespace
{
	constexpr float kShotSpeed = 8.0f;
}

ShotBase::ShotBase():m_handle(-1),m_rect(), m_vec(), m_isExist(false), m_isLeft(false), m_isPlayerShot(false)
{
}

ShotBase::~ShotBase()
{
}

void ShotBase::SetHandle(int handle)
{
	m_handle = handle;
}



void ShotBase::Movement(Vector2 vec)
{
	if (m_isLeft) vec *= 1.0f;
	m_rect.center += vec;
}

const Rect& ShotBase::GetRect() const
{
	return m_rect;
}

bool ShotBase::IsColPlayer(bool isExist, Position2 pos, Size colSize)
{
	//Ž©•ª‚ÌŒ‚‚Á‚½’e‚É‚Í“–‚½‚ç‚È‚¢
	if (m_isPlayerShot)	return false;

	//‘¶Ý‚µ‚È‚¢Ž©•ªA‘¶Ý‚µ‚È‚¢‚½‚Ü‚É‚Í‰½‚à“–‚½‚ç‚È‚¢
	if (!m_isExist)	return false;
	if (!isExist)	return false;

	float shotLeft = m_rect.center.x;
	float shotTop = m_rect.center.y;
	float shotRight = m_rect.center.x + m_rect.size.w;
	float shotBottom = m_rect.center.y + m_rect.size.h;

	float playerLeft = pos.x;
	float playerTop = pos.y;
	float playerRight = pos.x + colSize.w;
	float playerBottom = pos.y + colSize.h;

	if (playerLeft > shotRight)	return false;
	if (playerTop > shotBottom)	return false;
	if (playerRight < shotLeft)	return false;
	if (playerBottom < shotTop)	return false;

	return true;
}

bool ShotBase::IsColEnemy(bool isExist, Position2 pos, Size colSize)
{
	//Ž©•ª‚ÌŒ‚‚Á‚½’e‚É‚Í“–‚½‚ç‚È‚¢
	if (!m_isPlayerShot)	return false;

	//‘¶Ý‚µ‚È‚¢“GA‘¶Ý‚µ‚È‚¢‚½‚Ü‚É‚Í‰½‚à“–‚½‚ç‚È‚¢
	if (!m_isExist)	return false;
	if (!isExist)	return false;

	float shotLeft = m_rect.center.x;
	float shotTop = m_rect.center.y;
	float shotRight = m_rect.center.x + m_rect.size.w;
	float shotBottom = m_rect.center.y + m_rect.size.h;

	float enemyLeft = pos.x;
	float enemyTop = pos.y;
	float enemyRight = pos.x + colSize.w;
	float enemyBottom = pos.y + colSize.h;

	if (enemyLeft > shotRight)	return false;
	if (enemyTop > shotBottom)	return false;
	if (enemyRight < shotLeft)	return false;
	if (enemyBottom < shotTop)	return false;

	return true;
}
