#pragma once
#include "../Util/Geometry.h"

class Player
{
public:
	Player(Position2 pos);
	virtual ~Player();
	void Update();
	void Draw();

	//�v���C���[�ړ�
	void Movement(Vector2 vec);
	
	const Rect& GetRect()const;
	//���������Ă��邩�ǂ�����Ԃ�
	bool IsLeft()const { return m_isLeft; }
	//���������Ă��邩�ǂ���
	void SetLeft(bool isLeft) { m_isLeft = isLeft; }
	//�W�����v���Ă��邩�ǂ�����Ԃ�
	bool IsJump()const { return m_isJump; }
	//�W�����v�����Ă��邩���Ă��Ȃ��������߂�
	void SetJump(bool isJump) { m_isJump = isJump; }
private:
	//�v���C���[�̒��S�ʒu
	Rect m_rect;

	//���������Ă���
	bool m_isLeft = false;
	//�W�����v�����Ă��邩
	bool m_isJump = false;

};

