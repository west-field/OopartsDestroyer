#pragma once
#include "Secne.h"
#include "../Util/Geometry.h"
#include <memory>//�X�}�[�g�|�C���^����������
#include <array>
class Player;

/// <summary>
/// �Q�[���N���A�V�[��
/// </summary>
class GameclearScene : public Scene
{
public:
    GameclearScene(SceneManager& manager, std::shared_ptr<Player>player);
    virtual ~GameclearScene();

    void Update(const InputState& input);
    void Draw();
    static constexpr int kMojiNum = 6;
private:
    unsigned int m_fadeColor = 0x000000;//�t�F�[�h�̐F�i��

    void FadeInUpdat(const InputState& input);
    void NormalUpdat(const InputState& input);
    void FadeOutUpdat(const InputState& input);

    void (GameclearScene::* m_updateFunc)(const InputState&);

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
    bool m_mojiDraw = false;
};

