#pragma once
#include "Secne.h"
#include "../Util/Geometry.h"

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

    struct Moji
    {
        Position2 pos;
        float moveY;
        float add;
    };
    
    static constexpr int kMojiSize = 80;
    std::array<Moji, kMojiNum> m_moji;
};

