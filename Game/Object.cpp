#include "Object.h"

Object::Object():
	m_hp(),m_rect(),m_handle(-1),m_idx(0),
	m_isExist(true),m_isLeft(false),m_isJump(false), m_type(ObjectType::Enemy)
{
}

Object::~Object()
{
}

void Object::Movement(Vector2 vec)
{
	m_rect.center += vec;
}

const Rect& Object::GetRect() const
{
	return m_rect;
}

bool Object::IsLeft() const
{
	return m_isLeft;
}

void Object::SetLeft(bool isLeft)
{
	m_isLeft = isLeft;
}

bool Object::IsJump() const
{
	return m_isJump;
}

void Object::SetJump(bool isJump)
{
	m_isJump = isJump;
}

void Object::EraseExist()
{
	m_isExist = false;
}

