#pragma once
#include "EnemyBase.h"
#include "../Util/Geometry.h"

class Stage;

/// <summary>
/// �������X�[�W�[�@�ǁA�V��A�����㉺�A�܂��́A���E�ɉ����@HP5,�U����4
/// </summary>
class EnemyMoveLeftRight : public EnemyBase
{
public:
	EnemyMoveLeftRight(std::shared_ptr<Player>player, const Position2 pos, int handle, std::shared_ptr<ShotFactory> sFactory, std::shared_ptr<Stage> stage);
	virtual ~EnemyMoveLeftRight();

	virtual void Update()override;
	virtual void Draw()override;
	virtual void Movement(Vector2 vec)override;
	virtual int TouchAttackPower() const override;
private:
	void NormalUpdate();
	void LeftUpdate();
	void RightUpdate();

	void (EnemyMoveLeftRight::* m_updateFunc)();

	int m_frame = 0;
	int m_idx = 0;
	int m_handle = -1;
};

