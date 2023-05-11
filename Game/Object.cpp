#include "Object.h"

Object::Object():
	m_hp(),m_rect(),m_handle(-1),m_idx(0),
	m_isExist(true),m_isLeft(false),m_isJump(false), m_type(ObjectType::Enemy)
{
}

Object::~Object()
{
}

//�ړ�
void Object::Movement(Vector2 vec)
{
	m_rect.center += vec;
}

//��`���擾
const Rect& Object::GetRect() const
{
	return m_rect;
}

//���������Ă��邩
bool Object::IsLeft() const
{
	return m_isLeft;
}

//���������Ă��邩�ǂ��������
void Object::SetLeft(bool isLeft)
{
	m_isLeft = isLeft;
}

//�W�����v���Ă��邩�ǂ���
bool Object::IsJump() const
{
	return m_isJump;
}

//�W�����v���Ă��邩���Ă��Ȃ��������߂�
void Object::SetJump(bool isJump)
{
	m_isJump = isJump;
}

//���݂�����
void Object::EraseExist()
{
	m_isExist = false;
}

Vector2 Object::GetVec() const
{
	return Vector2(0,0);
}

