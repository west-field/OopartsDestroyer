#pragma once
#include "Secne.h"

#include <memory>//スマートポインタをつかうため
class Player;

/// <summary>
/// ゲームクリアシーン
/// </summary>
class GameclearScene : public Scene
{
private:
    unsigned int m_fadeColor = 0x000000;//フェードの色（黒

    void FadeInUpdat(const InputState& input);
    void NormalUpdat(const InputState& input);
    void FadeOutUpdat(const InputState& input);

    using UpdateFunc_t = void (GameclearScene::*)(const InputState&);
    UpdateFunc_t m_updateFunc;

    //プレイヤー
    std::shared_ptr<Player> m_player;
public:
    GameclearScene(SceneManager& manager, std::shared_ptr<Player>player);
    virtual ~GameclearScene();

    void Update(const InputState& input);
    void Draw();
};

