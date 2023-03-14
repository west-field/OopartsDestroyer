#include "Player.h"
#include <DxLib.h>
#include "../game.h"
#include "../Util/DrawFunctions.h"
#include "HpBar.h"

namespace
{
	constexpr int kSize = 5;

	//�v���C���[�O���t�B�b�N
	constexpr int kGraphSizeWidth = 32;		//�T�C�Y
	constexpr int kGraphSizeHeight = 32;	//�T�C�Y
	constexpr float kDrawScale = 2.0f;		//�g�嗦
	constexpr int kFrameSpeed = 10;		//�A�j���[�V�����X�s�[�h

	constexpr int kBurstFrameNum = 5;
	constexpr int kBurstFrameSpeed = 5;

	constexpr int ultimate_frame = 120;//���G���� 2�b

}

Player::Player(Position2 pos, std::shared_ptr<HpBar>hp):m_updateFunc(&Player::NormalUpdate),m_drawFunc(&Player::NormalDraw),m_hp(hp)
{
	m_drawScale = kDrawScale;
	m_rect.center = pos;
	m_rect.size = { static_cast<int>(kGraphSizeWidth*Game::kScale - kSize) ,static_cast<int>(kGraphSizeHeight * Game::kScale - kSize)};
	m_handle = my::MyLoadGraph(L"Data/player.png");
}

Player::~Player()
{
}

void Player::Update()
{
	if (!m_isExist)	return;
	(this->*m_updateFunc)();
}

void Player::Draw()
{
	if (!m_isExist)	return;
	(this->*m_drawFunc)();
}

void Player::Movement(Vector2 vec)
{
	m_rect.center += vec;
}

const Rect& Player::GetRect() const
{
	return m_rect;
}

void Player::Action(ActionType type)
{
	//if (m_idxX == 0 || m_idxY == 0 || m_idxY == 1)
	{
		switch (type)
		{
		case ActionType::grah_idle:
			m_idxY = static_cast<int>(ActionType::grah_idle);
			break;
		case ActionType::grah_walk:
			if(m_idxX == 0 && m_idxY == 0) m_idxY = static_cast<int>(ActionType::grah_walk);
			break;
		case ActionType::grah_jump:
			m_idxY = static_cast<int>(ActionType::grah_jump);
			break;
		case ActionType::grah_attack:
			m_idxY = static_cast<int>(ActionType::grah_attack);
			m_idxX = 0;
			break;
		case ActionType::grah_death:
			m_idxY = static_cast<int>(ActionType::grah_death);
			break;
		default:
			m_idxY = 0;
			break;
		}
	}
	
}

bool Player::IsCollidable() const
{
	return (m_updateFunc == &Player::NormalUpdate) && m_ultimateTimer == 0;
}

void Player::Damage(int damage)
{
	/*m_updateFunc = &Player::BurstUpdate;
	m_drawFunc = &Player::BurstDraw;*/
	m_ultimateTimer = ultimate_frame;
	m_hp->Damage(damage);
}

void Player::ScaleEnlarge(float scale)
{
	if (m_drawScale >= 3.0f)	return;
	if(m_ultimateTimer < 0) m_ultimateTimer = 0;
	m_drawScale += scale;
}

void Player::NormalUpdate()
{
	float chipSizeHalf = Game::kDrawSize / 2;
	//��ʂ̍��[�ɂ����炻��ȏ㓮���Ȃ��悤�ɂ���
	if (m_rect.center.x < Game::kMapScreenLeftX + chipSizeHalf)
	{
		m_rect.center.x = Game::kMapScreenLeftX + chipSizeHalf;
	}
	//��ʂ̉E�[�ɂ����炻��ȏ㓮���Ȃ��悤�ɂ���
	else if (m_rect.center.x > Game::kMapScreenRightX - chipSizeHalf)
	{
		m_rect.center.x = Game::kMapScreenRightX - chipSizeHalf;
	}
	//��ʂ̏�[�ɂ����炻��ȏ㓮���Ȃ��悤��
	if (m_rect.center.y < Game::kMapScreenTopY + chipSizeHalf)
	{
		m_rect.center.y = Game::kMapScreenTopY + chipSizeHalf;
	}
	//��ʂ̉��[�ɂ����炻��ȏ㓮���Ȃ��悤��
	else if (m_rect.center.y > Game::kMapScreenBottomY - chipSizeHalf)
	{
		m_rect.center.y = Game::kMapScreenBottomY - chipSizeHalf;
	}

	if (m_frame++ > kFrameSpeed)
	{
		switch (m_idxY)
		{
		case static_cast<int>(ActionType::grah_idle):
			m_idxX = (m_idxX + 1) % (1);
			break;
		case static_cast<int>(ActionType::grah_walk):
			m_idxX = (m_idxX + 1) % (2);
			break;
		case  static_cast<int>(ActionType::grah_jump):
		case  static_cast<int>(ActionType::grah_attack):
			m_idxX = (m_idxX + 1) % (4);
			break;
		case  static_cast<int>(ActionType::grah_death):
			m_idxX = (m_idxX + 1) % (6);
			if (m_idxX == 0)
			{
				m_isExist = false;
			}
			break;
		}
		if (m_idxX == 0)
		{
			m_idxY = 0;
		}
		
		m_frame = 0;
	}

	//0����������������0��Ԃ�
	if (--m_ultimateTimer <= 0)
	{
		m_ultimateTimer = 0;
	}
}

void Player::NormalDraw()
{
	if ((m_ultimateTimer / 10) % 2 == 1)
	{
		return;
	}
	//�v���C���[��\��
	my::MyDrawRectRotaGraph(static_cast<int>(m_rect.center.x), static_cast<int>(m_rect.center.y- kGraphSizeHeight/2),
		m_idxX * kGraphSizeWidth, m_idxY * kGraphSizeHeight, kGraphSizeWidth, kGraphSizeHeight, m_drawScale*Game::kScale, 0.0f, m_handle, true, m_isLeft);
#ifdef _DEBUG
	m_rect.Draw(0xaaffaa);

	DrawFormatString(0, 0, 0xaaaaaa, L"X%d", m_idxX);
	DrawFormatString(0, 20, 0xaaaaaa, L"Y%d", m_idxY);
#endif
}
