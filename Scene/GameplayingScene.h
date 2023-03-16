#pragma once
#include "Secne.h"
#include <vector>
#include "../Shot/ShotBase.h"
#include "../Util/Geometry.h"

#include <memory>//�X�}�[�g�|�C���^����������
#include <array>//�z��p

class Player;               //�v���C���[
class EnemyFactory;         //�G�l�~�[�𐶐�
class ShotBase;             //�e���N���X
class ShotFactory;          //�e�𐶐�
class ItemFactory;          //�A�C�e���𐶐�
class HpBar;                //Hp�o�[
class Map;                  //�}�b�v
class Stage;                //�X�e�[�W

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
    MapEvent_pause,//�ꎞ��~����
    MapEvent_restart,//���X�^�[�g�n�_
};

/// <summary>
/// �Q�[�����V�[��
/// </summary>
class GameplayingScene : public Scene
{
public:
    GameplayingScene(SceneManager& manager);
    virtual ~GameplayingScene();

    virtual void Update(const InputState& input);
    virtual void Draw();
private:
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
    /// �{�X�̓����蔻��
    /// </summary>
    /// <param name="MoveX">�ړ���</param>
    /// <param name="MoveY">�ړ���</param>
    void MoveBoss(float MoveX, float MoveY);
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
    /// ��ʂ��ړ��������邩�ǂ���
    /// </summary>
    void ScreenMove();
    /// <summary>
    /// �}�b�v�𓮂����ăL�����N�^�[���}�b�v�Ɠ����邩�ǂ���
    /// </summary>
    /// <param name="MoveX">�ړ���</param>
    /// <param name="MoveY">�ړ���</param>
    void MoveMap(float MoveX, float MoveY);

    /// <summary>
    /// ��q�ړ�
    /// </summary>
    void Ladder(const InputState& input);

    /// <summary>
    /// ��q�ƃv���C���[�̈ʒu�����킹��
    /// </summary>
    void LadderAlign();


    //�v���C���[�o��V�[��
    void PlayerOnScreen(const InputState& input);
    //��ʂ̃t�F�[�h�C��
     void FadeInUpdat(const InputState& input);
    //�ʏ�X�V
     void NormalUpdat(const InputState& input);
    //��ʈړ��̍X�V
     void MoveMapUpdat(const InputState& input);
    //��ʂ̃t�F�[�h�A�E�g
     void FadeOutUpdat(const InputState& input);
private:
    unsigned int m_fadeColor = 0x000000;//�t�F�[�h�̐F�i��
    
    void (GameplayingScene::* m_updateFunc)(const InputState& input);

    //�}�b�v
    std::shared_ptr<Map> m_map; 
    Position2 m_add;            //�v���C���[���ǂ̂��炢�ړ����Ă��邩
    int m_framecolor = 0xc0c0c0;//�D�F�@�}�b�v�̘g�̐F
    

    //�v���C���[
    std::shared_ptr<Player> m_player;
    float m_fallPlayerSpeed = 0;    //�����Ă����X�s�[�h
    bool m_isPlayerCenterLR = false;//�v���C���[����ʂ̒��S�ɋ��邩
    
    bool m_isScreenMoveUp = false;//�v���C���[����ʈړ��ʒu�ɂ��邩�ǂ����i��Ɉړ��j
    bool m_isScreenMoveDown = false;//�v���C���[����ʈړ��ʒu�ɂ��邩�ǂ����i���Ɉړ��j
    bool m_isScreenMoveWidth = false;//�v���C���[����ʈړ��ʒu�ɂ����ǂ����i���Ɉړ��j
    float m_playerPosUp;
    float m_playerPosBottom;

    //�V���b�g
    std::shared_ptr <ShotFactory> m_shotFactory;
    //�A�C�e��
    std::shared_ptr <ItemFactory> m_itemFactory;
    
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

    //��q������Ă���Ƃ�true
    bool m_isLadder = false;
    //��q������Ă���Ƃ��͈ʒu�����킹��
    bool m_isLadderAlign = false;
    //��q�����I�����Ƃ���x������Ɉړ�����
    bool m_isLadderFirst = false;
    //�����Ă��邩�ǂ��� �����Ă���Ƃ��̓W�����v�ł��Ȃ�
    bool m_isFall;

    bool m_isBoss = false;
    //0:�Q�[���N���A��1:�Q�[���I�[�o�[��
    int m_crea = 0;
    //��ʈړ�����ۈ�񂾂��G�l�~�[���폜
    bool m_isFirst = false;
    //�{�X���y
    int m_bossBgm = -1;
    //�T�E���h�̉���
    int m_soundVolume;

    int tempScreenH_ = -1;//��ʌ��ʗp�X�N���[���n���h��
    int quakeTimer_ = 0;//��ʗh��^�C�}�[
    float quakeX_ = 0.0f;//���h��p�����[�^
};

