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
/// �J�}�h�[�}�[�@���b�N�}������������ɃW�����v���Ă���@HP1,�U����2,�ڐG2
///	�������X�[�W�[�@�ǁA�V��A�����㉺�A�܂��́A���E�ɉ����@HP5,�U����4,�ڐG4
///	�}���u�[�@�܂��������ŁA���X����o���A�W���������ɒe��ł@HP�F1�A�U���́F2�i�e�j�A1�i�ڐG�j

//�G���
enum class EnemyType
{
	MoveUpDown,//���@��X���������Ƃ��㉺�Ɉړ�����G
	BatteryRight,//�Œ�C��
	BatteryLeft,//�Œ�C��
	Jump,//�W�����v����G
	MoveLeftRight,//���ړ�����l�p���G
	MoveShot,//�ړ����Ȃ���8�����Ɍ��G

	Boss,//�{�X
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
	int m_burstHandle;

	int m_frame = 0;//�G�쐬�܂ł̃t���[����
};

