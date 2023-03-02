#pragma once
#include "Secne.h"

#include <memory>//�X�}�[�g�|�C���^����������
class Player;

/// <summary>
/// �Q�[���N���A�V�[��
/// </summary>
class GameclearScene : public Scene
{
private:
    unsigned int m_fadeColor = 0x000000;//�t�F�[�h�̐F�i��

    void FadeInUpdat(const InputState& input);
    void NormalUpdat(const InputState& input);
    void FadeOutUpdat(const InputState& input);

    using UpdateFunc_t = void (GameclearScene::*)(const InputState&);
    UpdateFunc_t m_updateFunc;

    //�v���C���[
    std::shared_ptr<Player> m_player;
public:
    GameclearScene(SceneManager& manager, std::shared_ptr<Player>player);
    virtual ~GameclearScene();

    void Update(const InputState& input);
    void Draw();
};

