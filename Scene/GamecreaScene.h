#pragma once
#include "Secne.h"

/// <summary>
/// ゲームクリアシーン
/// </summary>
class GamecreaScene : public Scene
{
private:
    unsigned int m_fadeColor = 0x000000;//フェードの色（黒

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

