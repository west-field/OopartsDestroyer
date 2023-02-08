#pragma once
#include "../Util/Geometry.h"

enum class ActionType
{
	grah_no,
	grah_walk,
	grah_idle,
	grah_attack,
	grah_jump,
	grah_hit,

	grah_max
};

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
	//���ǂ�ȍs�������Ă���̂�
	void Action(ActionType type);
private:
	//�v���C���[�̒��S�ʒu
	Rect m_rect;
	//�v���C���[�̃O���t�B�b�N�n���h��
	int m_handle = -1;
	int m_idxX = 0;
	int m_idxY = 2;

	int m_frame = 0;

	//���������Ă���
	bool m_isLeft = false;
	//�W�����v�����Ă��邩
	bool m_isJump = false;

};

