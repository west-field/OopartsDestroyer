#pragma once
#include "../Util/Geometry.h"
#include "ShotBase.h"
#include <map>
#include <list>
#include <memory>

using ShotList_t = std::list<std::shared_ptr<ShotBase>>;

/// <summary>
/// �e�H��
/// </summary>
class ShotFactory
{
public:
	ShotFactory();
	virtual ~ShotFactory();

	/// <summary>
	/// �e�̐���
	/// </summary>
	/// <param name="type">�쐬����e�̎��</param>
	/// <param name="pos">�쐬�҂̈ʒu</param>
	/// <param name="vel">�ړ���</param>
	/// <param name="isleft">���������Ă���̂�</param>
	void Create(ShotType type, const Position2& pos, const Vector2& vel, bool isleft);

	/// <summary>
	/// �����蔻��p�ɂ��ׂĂ̓G�̒e�ւ̎Q��
	/// </summary>
	/// <returns>�G�̒e���X�g</returns>
	const ShotList_t& GetShot()const;
	void Update();
	void Draw();
private:
	std::map<ShotType, int> m_shotHTable;//���[�h����摜�̃e�[�u��
	ShotList_t m_shots;//�e���X�g
};

