#pragma once
#include <memory>
#include <list>
#include <map>
#include "../Util/Geometry.h"

class Player;
class HpBar;
class EnemyBase;
class ShotFactory;
class ItemFactory;

//�G���
enum class EnemyType
{
	no,
	MoveLeft,		//���@��X���������Ƃ��㉺�Ɉړ�����G
	BatteryRight,	//�Œ�C��
	BatteryLeft,	//�Œ�C��
	Jump,			//�W�����v����G
	MoveLeftRight,	//���ړ�����l�p���G
	no2,

	Boss,			//�{�X
};

class EnemyFactory
{
public:
	EnemyFactory(std::shared_ptr<Player>player, std::shared_ptr<ShotFactory> sFactory, std::shared_ptr<ItemFactory> itFactory, std::shared_ptr<HpBar> hp);
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
	std::list<std::shared_ptr<EnemyBase>> m_enemies;	//�G�l�~�[
	std::shared_ptr<Player> m_player;					//�v���C���[
	std::shared_ptr<HpBar> m_hp;						//HP
	std::shared_ptr<ShotFactory> m_shotFactory;			//�V���b�g
	std::shared_ptr<ItemFactory> m_itemFactory;			//�A�C�e��

	std::map<EnemyType, int>m_handleMap;//�G�̉摜�n���h��
	int m_burstHandle;					//�����O���t�B�b�N
	int m_bossBurstHandle;				//�{�X�����O���t�B�b�N
};

