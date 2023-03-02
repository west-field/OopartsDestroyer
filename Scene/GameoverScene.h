#pragma once
#include "Secne.h"

#include <memory>//スマートポインタをつかうため
class Player;               //プレイヤー

/// <summary>
/// ゲームオーバーシーン
/// </summary>
class GameoverScene : public Scene
{
private:
    unsigned int m_fadeColor = 0x000000;//フェードの色（黒

    void FadeInUpdat(const InputState& input);
    void NormalUpdat(const InputState& input);
    void FadeOutUpdat(const InputState& input);

    using UpdateFunc_t = void (GameoverScene::*)(const InputState&);
    UpdateFunc_t m_updateFunc;

    //プレイヤー
    std::shared_ptr<Player> m_player;
public:
    GameoverScene(SceneManager& manager,std::shared_ptr<Player>player);
    virtual ~GameoverScene();

    void Update(const InputState& input);
    void Draw();
};

