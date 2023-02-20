#pragma once
#include "Secne.h"
#include <vector>
#include "../Shot/ShotBase.h"
#include "../Util/Geometry.h"

#include <memory>//スマートポインタをつかうため
#include <array>//配列用

class Player;               //プレイヤー
class EnemyFactory;         //エネミーを生成
class ShotBase;             //弾基底クラス
class ShotFactory;          //弾を生成
class HpBar;                //Hpバー
class Map;                  //マップ
class Stage;                //ステージ

enum MapEvent
{
    MapEvent_no,//何もない
    MapEvent_hit,//当たる
    MapEvent_death,//死ぬ
    MapEvent_screenMoveU,//画面上移動
    MapEvent_screenMoveD,//画面下移動
    MapEvent_screenMoveW,//画面横移動
    MapEvent_ladder,//梯子
    MapEvent_screen,//読み込める範囲
};

/// <summary>
/// ゲーム中シーン
/// </summary>
class GameplayingScene : public Scene
{
public:
    GameplayingScene(SceneManager& manager);
    virtual ~GameplayingScene();

    virtual void Update(const InputState& input);
    virtual void Draw();
    /// <summary>
    /// ショットを作る
    /// </summary>
    /// <param name="pos">発射位置</param>
    /// <param name="isPlayer">プレイヤーが撃った弾か</param>
    /// <param name="isLeft">左を向いているか</param>
    /// <returns>true作れた false作れなかった</returns>
    bool createShot(Position2 pos, bool isPlayer,bool isLeft);
private:
    /// <summary>
    /// キャラクターとマップの当たり判定
    /// </summary>
    /// <param name="MoveX">移動量</param>
    /// <param name="MoveY">移動量</param>
    void MovePlayer(float MoveX, float MoveY);
    /// <summary>
    /// エネミーとマップの当たり判定
    /// </summary>
    /// <param name="MoveX">移動量</param>
    /// <param name="MoveY">移動量</param>
    void MoveEnemy(float MoveX, float MoveY);
    /// <summary>
    /// マップとの当たり判定
    /// </summary>
    /// <param name="X">自身の位置X</param>
    /// <param name="Y">自身の位置Y</param>
    /// <param name="MoveX">移動量</param>
    /// <param name="MoveY">移動量</param>
    /// <returns>1:左辺に当たった 2:右辺に当たった 3:上辺に当たった 4:下辺に当たった 0:どこにも当たらなかった</returns>
    int MapHitCheck(float X, float Y, float& MoveX, float& MoveY);
    /// <summary>
    /// プレイヤーが画面の中央に居るか
    /// </summary>
    void PlayerCenter();
    /// <summary>
    /// マップを動かしてキャラクターがマップと当たるかどうか
    /// </summary>
    /// <param name="MoveX">移動量</param>
    /// <param name="MoveY">移動量</param>
    void MoveMap(float MoveX, float MoveY);
 
    //画面のフェードイン
    virtual void FadeInUpdat(const InputState& input);
    //通常更新
    virtual void NormalUpdat(const InputState& input);
    //画面移動の更新
    virtual void MoveMapUpdat(const InputState& input);
    //画面のフェードアウト
    virtual void FadeOutUpdat(const InputState& input);
public:
    unsigned int m_fadeColor = 0x000000;//フェードの色（黒
    //UpdateFuncの型
    using UpdateFunc_t = void (GameplayingScene::*)(const InputState& input);
    UpdateFunc_t m_updateFunc = nullptr;

    int m_handle = -1;

    //マップ
    std::shared_ptr<Map> m_map; 
    Position2 m_add;            //プレイヤーがどのくらい移動しているか
    int m_framecolor = 0xc0c0c0;//灰色　マップの枠の色
    
    std::shared_ptr<Stage> m_stage;

    //プレイヤー
    std::shared_ptr<Player> m_player;
    float m_fallPlayerSpeed = 0;    //落ちていくスピード
    bool m_isPlayerCenterLR = false;//プレイヤーが画面の中心に居るか
    bool m_isPlayerMoveU = false;//プレイヤーが画面移動位置にいるかどうか（上に移動）
    bool m_isPlayerMoveD = false;//プレイヤーが画面移動位置にいるかどうか（下に移動）
    bool m_isPlayerMoveW = false;//プレイヤーが画面移動位置にいつかどうか（横に移動）

    //ショット
    std::shared_ptr <ShotFactory> m_shotFactory;  
    
    //エネミー
    std::shared_ptr <EnemyFactory> m_enemyFactory;
    Vector2 m_correction;//画面を動かしたとき位置を補正する
    
    enum HPBAR
    {
        Object_Player,
        Object_EnemyBoss,

        Object_Max
    };
    //HPバー
    std::array<std::shared_ptr<HpBar>, Object_Max> m_hp;    
    
    //自機ショット
    static constexpr int kShot = 15;                        //ショットの表示数
    std::array<std::shared_ptr<ShotBase>, kShot> m_shots;   //ショット

    //ゲームクリアかゲームオーバーか
    int m_crea = 0;
};

