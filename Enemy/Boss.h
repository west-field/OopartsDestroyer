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
	/// �����蔻��Ώۂ�
	/// </summary>
	/// <returns>true:������ false:������Ȃ�</returns>
	virtual bool IsCollidable()const override;

	/// <summary>
	/// �_���[�W���󂯂�
	/// </summary>
	/// <param name="damage">�_���[�W��</param>
	virtual void Damage(int damage) override;
	
private:
	//�ʏ�
	void NormalUpdate();
	void NormalDraw();

	//����
	void BurstUpdate();
	void BurstDraw();

	//�{�X���e������
	void ShotUpdate();

	void (Boss::* m_updateFunc)();
	void (Boss::* m_drawFunc)();

	int m_bossBurstH;	//�{�X�����摜

	int m_idx;			//�摜�̕\���͈�
	
	int m_animFrame;	//���̉摜�Ɉړ�(�{�X
	
	int m_frame;		//���̍U��������܂ł̎���
	
	int m_shotFrame;	//���U������Ƃ����̒e�����܂�
};

