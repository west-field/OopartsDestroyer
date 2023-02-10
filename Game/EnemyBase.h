#pragma once
#include <memory>
#include "../Util/Geometry.h"

class Player;
class HpBar;
class ShotFactory;

/// <summary>
/// �G���N���X
/// </summary>
class EnemyBase
{
public:
	EnemyBase(std::shared_ptr<Player>player,const Position2& pos,std::shared_ptr<ShotFactory> sFactory);
	virtual ~EnemyBase();

	virtual void Update() = 0;
	virtual void Draw() = 0;

	//�G�l�~�[�ړ�
	virtual void Movement(Vector2 vec) = 0;

	const Rect& GetRect() const;

	//�ڐG�������̍U����
	virtual int TouchAttackPower()const = 0;
	//�_���[�W���󂯂�
	virtual void Damage(int damage) = 0;
	//���̓G�͐����Ă��邩
	bool IsExist()const;
	//��������
	void SetExist(bool isExist) { m_isExist = isExist; }

	//���������Ă��邩�ǂ�����Ԃ�
	bool IsLeft()const { return m_isLeft; }
	//���������Ă��邩�ǂ���
	void SetLeft(bool isLeft) { m_isLeft = isLeft; }

	//�W�����v���Ă��邩�ǂ�����Ԃ�
	bool IsJump()const { return m_isJump; }
	//�W�����v�����Ă��邩���Ă��Ȃ��������߂�
	void SetJump(bool isJump) { m_isJump = isJump; }
protected:
	std::shared_ptr<Player> m_player = nullptr;
	std::shared_ptr<HpBar> m_hp = nullptr;
	std::shared_ptr<ShotFactory> m_shotFactory = nullptr;

	bool m_isExist = true;//���݂��邩
	Rect m_rect;//�G�̋�`

	bool m_isLeft = false;	//���������Ă���

	bool m_isJump = false;	//�W�����v�����Ă��邩
};

