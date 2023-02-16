#pragma once
#include "EnemyBase.h"
#include "../Util/Geometry.h"
/// <summary>
/// �������X�[�W�[�@�ǁA�V��A�����㉺�A�܂��́A���E�ɉ����@HP5,�U����4
/// </summary>
class EnemyMoveLeftRight : public EnemyBase
{
public:
	EnemyMoveLeftRight(std::shared_ptr<Player>player, const Position2 pos, int handle, std::shared_ptr<ShotFactory> sFactory);
	virtual ~EnemyMoveLeftRight();

	virtual void Update();
	virtual void Draw();
	virtual void Movement(Vector2 vec);
	virtual int TouchAttackPower() const;
	virtual void Damage(int damage);
private:
	int m_idx = 0;
	int m_handle = -1;
};

