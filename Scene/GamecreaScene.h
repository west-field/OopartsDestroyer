#pragma once
#include "Secne.h"

/// <summary>
/// �Q�[���N���A�V�[��
/// </summary>
class GamecreaScene : public Scene
{
private:
    unsigned int m_fadeColor = 0x000000;//�t�F�[�h�̐F�i��

    void FadeInUpdat(const InputState& input);
    void NormalUpdat(const InputState& input);
    void FadeOutUpdat(const InputState& input);

    using UpdateFunc_t = void (GamecreaScene::*)(const InputState&);
    UpdateFunc_t m_updateFunc;
public:
    GamecreaScene(SceneManager& manager);
    virtual ~GamecreaScene();

    void Update(const InputState& input);
    void Draw();
};

