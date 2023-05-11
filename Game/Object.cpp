#include "Object.h"

Object::Object():
	m_hp(),m_rect(),m_handle(-1),m_idx(0),
	m_isExist(true),m_isLeft(false),m_isJump(false), m_type(ObjectType::Enemy)
{
}

Object::~Object()
{
}

//移動
void Object::Movement(Vector2 vec)
{
	m_rect.center += vec;
}

//矩形を取得
const Rect& Object::GetRect() const
{
	return m_rect;
}

//左を向いているか
bool Object::IsLeft() const
{
	return m_isLeft;
}

//左を向いているかどうかを入力
void Object::SetLeft(bool isLeft)
{
	m_isLeft = isLeft;
}

//ジャンプしているかどうか
bool Object::IsJump() const
{
	return m_isJump;
}

//ジャンプしているかしていないかを決める
void Object::SetJump(bool isJump)
{
	m_isJump = isJump;
}

//存在を消す
void Object::EraseExist()
{
	m_isExist = false;
}

Vector2 Object::GetVec() const
{
	return Vector2(0,0);
}

