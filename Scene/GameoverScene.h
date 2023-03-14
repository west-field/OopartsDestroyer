#pragma once
#include "Secne.h"
#include "../Util/Geometry.h"
#include "../game.h"

#include <memory>//�X�}�[�g�|�C���^����������
#include <array>

class Player;               //�v���C���[

/// <summary>
/// �Q�[���I�[�o�[�V�[��
/// </summary>
class GameoverScene : public Scene
{
public:
    GameoverScene(SceneManager& manager, std::shared_ptr<Player>player);
    virtual ~GameoverScene();

    void Update(const InputState& input);
    void Draw();

    static constexpr int kMojiNum = 7;
private:
    unsigned int m_fadeColor = 0xff0000;//�t�F�[�h�̐F�i��

    void FadeInUpdat(const InputState& input);
    void FadeOutUpdat(const InputState& input);
    void NormalUpdat(const InputState& input);
    void MojiUpdate(const InputState& input);

    void NormalDraw();
    void MojiDraw();

    void (GameoverScene::*m_updateFunc )(const InputState&) ;
    void (GameoverScene::*m_drawFunc )() ;

    //�v���C���[
    std::shared_ptr<Player> m_player;

    //����
    struct Moji
    {
        Position2 pos;
        float moveY;
        float add;
    };
    
    static constexpr int kMojiSize = 80;
    std::array<Moji, kMojiNum> m_moji;
    int  m_soundVolume = 0;

    static constexpr int kMenuFontSize = 50;//�����̃T�C�Y
    static constexpr int kOriginalPosX = Game::kScreenWidth / 3 + kMenuFontSize * 2;    //���j���[������x�ʒu
    static constexpr int kOriginalPosY = Game::kScreenHeight / 2 + kMenuFontSize * 2;    //���j���[������y�ʒu
    static constexpr int kMovedPosX = kOriginalPosX - kMenuFontSize;//���j���[�����̈ړ�����x�ʒu
    //���j���[����
    enum MenuItem
    {
        menuGameEnd,	//�Q�[���I��
        menuRestart,	//���X�^�[�g

        menuNum			//���ڂ̐�
    };
    //���j���[�v�f
    struct MenuElement
    {
        int x;
        int y;				//���W
        unsigned int color;//�F
    };
    MenuElement SelectMenu[menuNum] = {
        { kMovedPosX, kOriginalPosY + kMenuFontSize * menuGameEnd,0xffa0aa  },
        { kOriginalPosX, kOriginalPosY + kMenuFontSize * menuRestart + 5,0xaaa0ff}
    };
    int m_selectNum = 0;//�I�����Ă��郁�j���[����
};

