#pragma once
#include "Secne.h"
#include <vector>
#include "../Shot/ShotBase.h"
#include "../Util/Geometry.h"

#include <memory>//�X�}�[�g�|�C���^����������
#include <array>//�z��p

class Player;
class EnemyFactory;
class ShotBase;
class HpBar;
class Map;

enum MapChip
{
    no,			//�Ȃ�       ������Ȃ�
    sky,		//��         ������Ȃ�
    terrain,	//�n��       ������
    wall1,		//��1        ������
    wall2,		//��2        ������
    ladder1,	//��q1      ������Ȃ�
    ladder2,	//��q2      ������Ȃ�
    stone,		//��         ������
    thorn,		//�Ƃ�       ������

    max
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
    bool createShot(Position2 pos, bool isPlayer,bool isLeft);
private:
    //�L�����N�^�[�ƃ}�b�v�̓����蔻��
    int MovePlayer(float MoveX, float MoveY);
    //�G�l�~�[�ƃ}�b�v�̓����蔻��
    int MoveEnemy(float MoveX, float MoveY);
    //�e�ƃ}�b�v�̓����蔻��
    int MoveShot(int i ,float MoveX, float MoveY);
    //�}�b�v�Ƃ̓����蔻��
    int MapHitCheck(float X, float Y, float& MoveX, float& MoveY);
    //�v���C���[����ʂ̒����ɋ��邩
    void PlayerCenter();
    //�}�b�v�𓮂����ăL�����N�^�[���}�b�v�Ɠ����邩�ǂ���
    int MoveMap(float MoveX, float MoveY);
 
    virtual void FadeInUpdat(const InputState& input);
    virtual void NormalUpdat(const InputState& input);
    virtual void FadeOutUpdat(const InputState& input);
public:
    int m_test;//��ʂ�h�点��(��������]������)

    unsigned int m_fadeColor = 0x000000;//�t�F�[�h�̐F�i��
    //UpdateFunc�̌^
    using UpdateFunc_t = void (GameplayingScene::*)(const InputState& input);
    UpdateFunc_t m_updateFunc = nullptr;

    std::shared_ptr<Map> m_map;//�}�b�v
    Position2 m_add;//�v���C���[���ǂ̂��炢�ړ����Ă��邩

    std::shared_ptr<Player> m_player;//�v���C���[
    float m_fallPlayerSpeed = 0;//�����Ă����X�s�[�h
    bool m_isPlayerCenterLR = false;//�v���C���[����ʂ̒��S�ɋ��邩
    bool m_isPlayerCenterUD = false;//�v���C���[����ʂ̒��S�ɋ��邩

    std::shared_ptr <EnemyFactory> m_enemyFactory;//�G�l�~�[
    float m_fallEnemySpeed = 0;//�����Ă����X�s�[�h    
    
    enum
    {
        Object_Player,
        Object_Enemy,

        Object_Max
    };
    std::array<std::shared_ptr<HpBar>, Object_Max> m_hp;//HP�o�[
    
    static constexpr int kShot = 15;//�V���b�g�̕\����
    std::array<std::shared_ptr<ShotBase>, kShot> m_shots;//�V���b�g
    int m_shotCount = 0;//�����݂��Ă���V���b�g�̐�
};

