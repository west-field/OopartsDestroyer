#pragma once
#include "EnemyBase.h"
#include "../Util/Geometry.h"

/// <summary>
/// �J�b�g�}���i�{�X�j �U����,�ڐG:4
/// </summary>
class Boss :public EnemyBase
{
public :
	Boss(std::shared_ptr<Player>player, const Position2& pos,int handle, int bossBurstH, int burstH, std::shared_ptr<ShotFactory> sFactory, std::shared_ptr<ItemFactory> itFactory, std::shared_ptr<HpBar>hp);
	virtual ~Boss();
	virtual void Update()override;
	virtual void Draw()override;

	/// <summary>
	/// �ڐG�����Ƃ��̍U��
	/// </summary>
	/// <returns>�ڐG�����Ƃ��̍U����</returns>
	virtual int TouchAttackPower() const override;
	/// <summary>
	/// �_���[�W���󂯂�
	/// </summary>
	/// <param name="damage">�_���[�W��</param>
	virtual void Damage(int damage) override;
	/// <summary>
	/// �����蔻��Ώۂ�
	/// </summary>
	/// <returns>true:������ false:������Ȃ�</returns>
	virtual bool IsCollidable()const override;
private:
	void NormalUpdate();//���̎w�������߂�
	void NormalDraw();//�ʏ�`��

	void BurstUpdate();//�����X�V
	void BurstDraw();//�����\��

	void ShotUpdate();//�{�X���e������

	void (Boss::* m_updateFunc)();
	void (Boss::* m_drawFunc)();

	int m_bossBurstH = -1;//�{�X�����摜

	int m_idx = 0;//�摜�̕\���͈�
	
	int m_animFrame = 0;//���̉摜�Ɉړ�(�{�X
	
	int m_frame = 0;//���̍U��������܂ł̎���
	
	int m_shotFrame = 20;//���U������Ƃ����̒e�����܂�
};

