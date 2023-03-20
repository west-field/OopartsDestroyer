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

	/// <summary>
	/// �v���C���[�ړ�
	/// </summary>
	/// <param name="vec">�ړ���</param>
	void Movement(Vector2 vec);
	
	/// <summary>
	/// ��`���擾
	/// </summary>
	/// <returns��`�\����></returns>
	const Rect& GetRect()const;
	
	/// <summary>
	/// ���������Ă��邩�ǂ�����Ԃ�
	/// </summary>
	/// <returns>true:���������Ă��� false:�E�������Ă���</returns>
	bool IsLeft()const { return m_isLeft; }
	/// <summary>
	/// ���������Ă��邩�ǂ���
	/// </summary>
	/// <param name="isLeft">���������Ă��邩</param>
	void SetLeft(bool isLeft) { m_isLeft = isLeft; }
	
	/// <summary>
	/// �W�����v���Ă��邩�ǂ�����Ԃ�
	/// </summary>
	/// <returns>true:�W�����v���Ă��� false:�W�����v���Ă��Ȃ�</returns>
	bool IsJump()const { return m_isJump; }
	/// <summary>
	/// �W�����v�����Ă��邩���Ă��Ȃ���
	/// </summary>
	/// <param name="isJump">�W�����v���Ă��邩</param>
	void SetJump(bool isJump) { m_isJump = isJump; }
	
	/// <summary>
	/// ���ǂ�ȍs�������Ă���̂�
	/// </summary>
	/// <param name="type">�s���^�C�v</param>
	void Action(ActionType type);
	
	/// <summary>
	/// �����蔻��Ώۂ�
	/// </summary>
	/// <returns>true:������ false:������Ȃ�</returns>
	bool IsCollidable()const;
	
	/// <summary>
	/// �_���[�W���󂯂�
	/// </summary>
	/// <param name="damage">�_���[�W��</param>
	void Damage(int damage);
	/// <summary>
	/// �񕜂���
	/// </summary>
	/// <param name="Heal">�񕜗�</param>
	void Heal(int Heal);
	
	/// <summary>
	/// ���݂��Ă��邩�ǂ���
	/// </summary>
	/// <returns>true:���݂��� false:���݂��Ȃ�</returns>
	bool IsExist()const { return m_isExist; }
	
	/// <summary>
	/// �T�C�Y��傫������
	/// </summary>
	/// <param name="scale">�g�嗦</param>
	void ScaleEnlarge(float scale);
private:
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

	std::shared_ptr<HpBar> m_hp;//HP

	bool m_isHeal = false;//�񕜃A�C�e�����Ƃ�����
	int m_healH = -1;//�񕜃G�t�F�N�g
	int m_healIdx = 0;//�摜�̕\���ʒu
	int m_healAnimFram = 0;//���̉摜�Ɉړ�
};

