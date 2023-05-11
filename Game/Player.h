#pragma once
#include "Object.h"
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

class Player :public Object
{
public:
	Player(Position2 pos,std::shared_ptr<HpBar>hp);
	virtual ~Player();
	void Update();
	void Draw();
	
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
	virtual void Damage(int damage) override;
	/// <summary>
	/// �񕜂���
	/// </summary>
	/// <param name="Heal">�񕜗�</param>
	void Heal(int Heal);
	
	/// <summary>
	/// �T�C�Y��傫������
	/// </summary>
	/// <param name="scale">�g�嗦</param>
	void ScaleEnlarge(float scale);

	/// <summary>
	/// �}�b�v�`�b�v����肷��
	/// </summary>
	/// <param name="chipId">�}�b�v�`�b�v</param>
	virtual void GetChip(int chipId) override {}
private:
	
	int m_idxX = 0;//�\������`��ʒuX
	int m_idxY = 2;//�\������`��ʒuY

	float m_drawScale = 0.0f;//�g�嗦

	int m_frame = 0;//�A�j���[�V��������

	int m_ultimateTimer = 0;//���G����

	bool m_isHeal = false;//�񕜃A�C�e�����Ƃ�����
	int m_healH = -1;//�񕜃G�t�F�N�g
	int m_healIdx = 0;//�摜�̕\���ʒu
	int m_healAnimFram = 0;//���̉摜�Ɉړ�
};

