#pragma once
#include "Secne.h"
#include "../Util/Geometry.h"
#include <memory>//スマートポインタをつかうため
#include <array>
class Player;

/// <summary>
/// ゲームクリアシーン
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
    unsigned int m_fadeColor = 0x000000;//フェードの色（黒

    void FadeInUpdat(const InputState& input);
    void FadeOutUpdat(const InputState& input);
    void NormalUpdat(const InputState& input);
    void MojiUpdate(const InputState& input);

    void NormalDraw();
    void MojiDraw();

    void (GameclearScene::* m_updateFunc)(const InputState&);
    void (GameclearScene::* m_drawFunc)();

    //プレイヤー
    std::shared_ptr<Player> m_player;

    struct Moji
    {
        Position2 pos = {};
        float moveY = 0.0f;
        float add = 0.0f;
    };

    static constexpr int kMojiSize = 80;
    std::array<Moji, kMojiNum> m_moji;
    int m_soundVolume = 0;
};

