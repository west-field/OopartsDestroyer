#pragma once
#include "ShotBase.h"

class RockBuster : public ShotBase
{
public:
	RockBuster(int handle);
	virtual ~RockBuster();

	/// <summary>
	/// �V���b�g�J�n
	/// </summary>
	/// <param name="pos">�쐬�ʒu</param>
	/// <param name="vel">�ړ�����</param>
	/// <param name="left">���E�ǂ���������Ă��邩</param>
	/// <param name="isPlayer">�v���C���[���������e��</param>
	virtual void Start(Position2 pos, Vector2 vel, bool left, bool isPlayer) override;
	///�X�V
	virtual void Update()override;
	///�\��
	virtual void Draw()override;
	/// <summary>
	/// �ړ�����
	/// </summary>
	/// <param name="vec">��ʈړ�</param>
	virtual void Movement(Vector2 vec)override;
	/// <summary>
	/// �U���͂�Ԃ�
	/// </summary>
	/// <returns>�U����</returns>
	virtual const int AttackPower()const;
private:
};

