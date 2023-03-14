#pragma once
#include "Secne.h"
#include "../Util/Geometry.h"
#include "../game.h"

#include <memory>//スマートポインタをつかうため
#include <array>

class Player;               //プレイヤー

/// <summary>
/// ゲームオーバーシーン
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
    unsigned int m_fadeColor = 0xff0000;//フェードの色（赤

    void FadeInUpdat(const InputState& input);
    void FadeOutUpdat(const InputState& input);
    void NormalUpdat(const InputState& input);
    void MojiUpdate(const InputState& input);

    void NormalDraw();
    void MojiDraw();

    void (GameoverScene::*m_updateFunc )(const InputState&) ;
    void (GameoverScene::*m_drawFunc )() ;

    //プレイヤー
    std::shared_ptr<Player> m_player;

    //文字
    struct Moji
    {
        Position2 pos;
        float moveY;
        float add;
    };
    
    static constexpr int kMojiSize = 80;
    std::array<Moji, kMojiNum> m_moji;
    int  m_soundVolume = 0;

    static constexpr int kMenuFontSize = 50;//文字のサイズ
    static constexpr int kOriginalPosX = Game::kScreenWidth / 3 + kMenuFontSize * 2;    //メニュー文字のx位置
    static constexpr int kOriginalPosY = Game::kScreenHeight / 2 + kMenuFontSize * 2;    //メニュー文字のy位置
    static constexpr int kMovedPosX = kOriginalPosX - kMenuFontSize;//メニュー文字の移動したx位置
    //メニュー項目
    enum MenuItem
    {
        menuGameEnd,	//ゲーム終了
        menuRestart,	//リスタート

        menuNum			//項目の数
    };
    //メニュー要素
    struct MenuElement
    {
        int x;
        int y;				//座標
        unsigned int color;//色
    };
    MenuElement SelectMenu[menuNum] = {
        { kMovedPosX, kOriginalPosY + kMenuFontSize * menuGameEnd,0xffa0aa  },
        { kOriginalPosX, kOriginalPosY + kMenuFontSize * menuRestart + 5,0xaaa0ff}
    };
    int m_selectNum = 0;//選択しているメニュー項目
};

