#pragma once
#include "../Util/Geometry.h"
#include <memory>
class HpBar;

enum class ActionType
{
	grah_idle,
	grah_walk,
	grah_reload,
	grah_jump,
	grah_attack,
	grah_death,

	grah_max
};

class Player
{
public:
	Player(Position2 pos,std::shared_ptr<HpBar>hp);
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
	//�����蔻��Ώۂ�
	bool IsCollidable()const;
	//�_���[�W���󂯂���
	void Damage(int damage);
	//���݂��Ă��邩�ǂ���
	bool IsExist()const { return m_isExist; }
	//�T�C�Y��傫������
	void ScaleEnlarge(float scale);
private:
	//���ʂ̃A�b�v�f�[�g
	void NormalUpdate();
	//���ʂ̕`��
	void NormalDraw();

	void (Player::* m_updateFunc)();//�A�b�v�f�[�g�̑J��
	void (Player::* m_drawFunc)();//�`��̑J��
	
	Rect m_rect;//�v���C���[�̋�`
	
	int m_handle = -1;//�v���C���[�̃O���t�B�b�N�n���h��
	int m_idxX = 0;//�\������`��ʒuX
	int m_idxY = 2;//�\������`��ʒuY

	float m_drawScale = 0.0f;//�g�嗦

	int m_frame = 0;//�A�j���[�V��������

	bool m_isLeft = false;//���������Ă���
	
	bool m_isJump = false;//�W�����v�����Ă��邩

	int m_ultimateTimer = 0;//���G����

	bool m_isExist = true;//���݂��Ă��邩�ǂ���

	std::shared_ptr<HpBar> m_hp;
};

