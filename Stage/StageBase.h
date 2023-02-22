#pragma once
#include "../Util/Geometry.h"
#include <memory>//�X�}�[�g�|�C���^����������
#include <array>//�z��p
class Player;               //�v���C���[
class EnemyFactory;         //�G�l�~�[�𐶐�
class ShotBase;             //�e���N���X
class ShotFactory;          //�e�𐶐�
class HpBar;                //Hp�o�[
class Map;                  //�}�b�v

enum MapEvent
{
    MapEvent_no,//�����Ȃ�
    MapEvent_hit,//������
    MapEvent_death,//����
    MapEvent_screenMoveU,//��ʏ�ړ�
    MapEvent_screenMoveD,//��ʉ��ړ�
    MapEvent_screenMoveW,//��ʉ��ړ�
    MapEvent_ladder,//��q
    MapEvent_screen,//�ǂݍ��߂�͈�
};

class StageBase
{
public:
	StageBase();
	virtual ~StageBase();

	virtual void Update();
	virtual void Draw();

    /// <summary>
    /// �V���b�g�����
    /// </summary>
    /// <param name="pos">���ˈʒu</param>
    /// <param name="isPlayer">�v���C���[���������e��</param>
    /// <param name="isLeft">���������Ă��邩</param>
    /// <returns>true��ꂽ false���Ȃ�����</returns>
    bool createShot(Position2 pos, bool isPlayer, bool isLeft);
    /// <summary>
   /// �L�����N�^�[�ƃ}�b�v�̓����蔻��
   /// </summary>
   /// <param name="MoveX">�ړ���</param>
   /// <param name="MoveY">�ړ���</param>
    void MovePlayer(float MoveX, float MoveY);
    /// <summary>
    /// �G�l�~�[�ƃ}�b�v�̓����蔻��
    /// </summary>
    /// <param name="MoveX">�ړ���</param>
    /// <param name="MoveY">�ړ���</param>
    void MoveEnemy(float MoveX, float MoveY);
    /// <summary>
    /// �}�b�v�Ƃ̓����蔻��
    /// </summary>
    /// <param name="X">���g�̈ʒuX</param>
    /// <param name="Y">���g�̈ʒuY</param>
    /// <param name="MoveX">�ړ���</param>
    /// <param name="MoveY">�ړ���</param>
    /// <returns>1:���ӂɓ������� 2:�E�ӂɓ������� 3:��ӂɓ������� 4:���ӂɓ������� 0:�ǂ��ɂ�������Ȃ�����</returns>
    int MapHitCheck(float X, float Y, float& MoveX, float& MoveY);
    /// <summary>
    /// �v���C���[����ʂ̒����ɋ��邩
    /// </summary>
    void PlayerCenter();
    /// <summary>
    /// �}�b�v�𓮂����ăL�����N�^�[���}�b�v�Ɠ����邩�ǂ���
    /// </summary>
    /// <param name="MoveX">�ړ���</param>
    /// <param name="MoveY">�ړ���</param>
    void MoveMap(float MoveX, float MoveY);
private:
    //�}�b�v
    std::shared_ptr<Map> m_map;
    Position2 m_add;            //�v���C���[���ǂ̂��炢�ړ����Ă��邩
    int m_framecolor = 0xc0c0c0;//�D�F�@�}�b�v�̘g�̐F

    //�v���C���[
    std::shared_ptr<Player> m_player;
    float m_fallPlayerSpeed = 0;    //�����Ă����X�s�[�h
    bool m_isPlayerCenterLR = false;//�v���C���[����ʂ̒��S�ɋ��邩
    bool m_isPlayerMoveU = false;//�v���C���[����ʈړ��ʒu�ɂ��邩�ǂ����i��Ɉړ��j
    bool m_isPlayerMoveD = false;//�v���C���[����ʈړ��ʒu�ɂ��邩�ǂ����i���Ɉړ��j
    bool m_isPlayerMoveW = false;//�v���C���[����ʈړ��ʒu�ɂ����ǂ����i���Ɉړ��j

    //�V���b�g
    std::shared_ptr <ShotFactory> m_shotFactory;

    //�G�l�~�[
    std::shared_ptr <EnemyFactory> m_enemyFactory;
    Vector2 m_correction;//��ʂ𓮂������Ƃ��ʒu��␳����

    enum HPBAR
    {
        Object_Player,
        Object_EnemyBoss,

        Object_Max
    };
    //HP�o�[
    std::array<std::shared_ptr<HpBar>, Object_Max> m_hp;

    //���@�V���b�g
    static constexpr int kShot = 15;                        //�V���b�g�̕\����
    std::array<std::shared_ptr<ShotBase>, kShot> m_shots;   //�V���b�g

    //�Q�[���N���A���Q�[���I�[�o�[��
    int m_crea = 0;

    int m_handle = -1;
};
