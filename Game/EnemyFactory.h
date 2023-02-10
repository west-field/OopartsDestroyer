#pragma once
#include <memory>
#include <list>
#include <map>
#include "../Util/Geometry.h"

class Player;
class EnemyBase;
class ShotFactory;

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
	EnemyFactory(std::shared_ptr<Player>player, std::shared_ptr<ShotFactory> sFactory);
	virtual ~EnemyFactory();

	void Update();//�X�V
	void Draw();//�\��

	/// <summary>
	/// �G�I�u�W�F�N�g�̐���
	/// </summary>
	/// <param name="type">�G���</param>
	/// <param name="pos">�����ʒu</param>
	/// <returns>�G�I�u�W�F�N�g</returns>
	std::shared_ptr<EnemyBase> Create(EnemyType type, const Position2 pos);

	/// <summary>
	/// �G�l�~�[�����ׂĎ擾
	/// </summary>
	/// <returns>�G�l�~�[���ׂ�</returns>
	std::list<std::shared_ptr<EnemyBase>>& GetEnemies();
private:
	std::list<std::shared_ptr<EnemyBase>> m_enemies;	//�G�l�~�[���쐬
	std::shared_ptr<Player> m_player;					//�v���C���[
	std::shared_ptr<ShotFactory> m_shotFactory;			//�V���b�g

	std::map<EnemyType, int>m_handleMap;//�G�̉摜�n���h��
	int m_frame = 0;//�G�쐬�܂ł̃t���[����
};

