#pragma once
#include "EnemyBase.h"
#include "../Util/Geometry.h"

class Stage;

/// <summary>
/// �������X�[�W�[�@�ǁA�V��A�����㉺�A�܂��́A���E�ɉ����@HP5,�U����4
/// </summary>
class EnemyMoveLR : public EnemyBase
{
public:
	EnemyMoveLR(std::shared_ptr<Player>player, const Position2 pos, int handle, int burstH, std::shared_ptr<ShotFactory> sFactory, std::shared_ptr<ItemFactory> itFactory);
	virtual ~EnemyMoveLR();

	virtual void Update()override;
	virtual void Draw()override;
	//�ڐG�������̍U����
	virtual int TouchAttackPower() const override;
	//�_���[�W���󂯂�
	virtual void Damage(int damage) override;
	//�����蔻��Ώۂ�
	virtual bool IsCollidable()const override;
private:
	//�ʏ�
	void NormalUpdate();
	void NormalDraw();

	//����
	void BurstUpdate();
	void BurstDraw();

	//���E�ɓ���
	void MoveUpdate();

	void (EnemyMoveLR::* m_updateFunc)();
	void (EnemyMoveLR::* m_drawFunc)();

	int m_frame = 0;
};

