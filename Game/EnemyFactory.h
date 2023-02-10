#pragma once
#include <memory>
#include <list>
#include <map>
#include "../Util/Geometry.h"

class Player;
class EnemyBase;

/// �u���r�[�ւ�[�@��s���Ȃ���ߕt���āA�v���C���[�̋߂��ŏ㉺�ɓ������G�BHP1�A�U����4
/// �u���X�^�[�@�ǂɂ���Ԃ��C��B�V���b�^�[���J�����Ƃ��ɍU�����Ă���BHP1�A�U���́i�e�j2�A�i�ڐG�j1

//�G���
enum class EnemyType
{
	MoveUpDown,//���ړ�����G
	Battery,//�Œ�C��
};

class EnemyFactory
{
public:
	EnemyFactory(std::shared_ptr<Player>player);
	virtual ~EnemyFactory();

	void Update();
	void Draw();

	/// <summary>
	/// �G�I�u�W�F�N�g�̐���
	/// </summary>
	/// <param name="type">�G���</param>
	/// <param name="pos">�����ʒu</param>
	/// <returns>�G�I�u�W�F�N�g</returns>
	std::shared_ptr<EnemyBase> Create(EnemyType type, const Position2 pos);

	std::list<std::shared_ptr<EnemyBase>>& GetEnemies();
private:
	std::list<std::shared_ptr<EnemyBase>> m_enemies;
	std::shared_ptr<Player> m_player;

	std::map<EnemyType, int>m_handleMap;//�G�̃n���h��
	int m_frame = 0;
};

