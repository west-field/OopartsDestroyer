#pragma once
#include "EnemyBase.h"

/// <summary>
///�@���ɔ��ł����G�@HP1�A�U����4
/// (�u���r�[�ւ�[�@��s���Ȃ���ߕt���āA�v���C���[�̋߂��ŏ㉺�ɓ������G�B)
/// </summary>
class EnemyMoveLeft : public EnemyBase
{
public:
	EnemyMoveLeft(std::shared_ptr<Player>player, const Position2 pos,int handle, int burstH, std::shared_ptr<ShotFactory> sFactory, std::shared_ptr<ItemFactory> itFactory);
	virtual ~EnemyMoveLeft();
	virtual void Update()override;
	virtual void Draw()override;

	//�����蔻��Ώۂ��ǂ���
	virtual bool IsCollidable()const override;
	//�_���[�W���󂯂�
	virtual void Damage(int damage) override;
	//�ꏊ�����߂�(title�ł̂ݎg�p���Ă���)
	void SetPos(Position2 pos) { m_rect.center = pos; }
private:
	//�ʏ�
	void NormalUpdate();
	void NormalDraw();

	//����
	void BurstUpdate();
	void BurstDraw();

	//�����o�֐��|�C���^
	void (EnemyMoveLeft::* m_updateFunc)();
	void (EnemyMoveLeft::* m_drawFunc)();
};

