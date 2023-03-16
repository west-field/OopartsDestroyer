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
class ItemFactory;          //アイテムを生成
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
    MapEvent_pause,//一時停止する
    MapEvent_restart,//リスタート地点
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
    /// ボスの当たり判定
    /// </summary>
    /// <param name="MoveX">移動量</param>
    /// <param name="MoveY">移動量</param>
    void MoveBoss(float MoveX, float MoveY);
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
    /// 画面を移動させられるかどうか
    /// </summary>
    void ScreenMove();
    /// <summary>
    /// マップを動かしてキャラクターがマップと当たるかどうか
    /// </summary>
    /// <param name="MoveX">移動量</param>
    /// <param name="MoveY">移動量</param>
    void MoveMap(float MoveX, float MoveY);

    /// <summary>
    /// 梯子移動
    /// </summary>
    void Ladder(const InputState& input);

    /// <summary>
    /// 梯子とプレイヤーの位置を合わせる
    /// </summary>
    void LadderAlign();


    //プレイヤー登場シーン
    void PlayerOnScreen(const InputState& input);
    //画面のフェードイン
     void FadeInUpdat(const InputState& input);
    //通常更新
     void NormalUpdat(const InputState& input);
    //画面移動の更新
     void MoveMapUpdat(const InputState& input);
    //画面のフェードアウト
     void FadeOutUpdat(const InputState& input);
private:
    unsigned int m_fadeColor = 0x000000;//フェードの色（黒
    
    void (GameplayingScene::* m_updateFunc)(const InputState& input);

    //マップ
    std::shared_ptr<Map> m_map; 
    Position2 m_add;            //プレイヤーがどのくらい移動しているか
    int m_framecolor = 0xc0c0c0;//灰色　マップの枠の色
    

    //プレイヤー
    std::shared_ptr<Player> m_player;
    float m_fallPlayerSpeed = 0;    //落ちていくスピード
    bool m_isPlayerCenterLR = false;//プレイヤーが画面の中心に居るか
    
    bool m_isScreenMoveUp = false;//プレイヤーが画面移動位置にいるかどうか（上に移動）
    bool m_isScreenMoveDown = false;//プレイヤーが画面移動位置にいるかどうか（下に移動）
    bool m_isScreenMoveWidth = false;//プレイヤーが画面移動位置にいつかどうか（横に移動）
    float m_playerPosUp;
    float m_playerPosBottom;

    //ショット
    std::shared_ptr <ShotFactory> m_shotFactory;
    //アイテム
    std::shared_ptr <ItemFactory> m_itemFactory;
    
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

    //梯子を上っているときtrue
    bool m_isLadder = false;
    //梯子を上っているときは位置を合わせる
    bool m_isLadderAlign = false;
    //梯子を上り終えたとき一度だけ上に移動する
    bool m_isLadderFirst = false;
    //落ちているかどうか 落ちているときはジャンプできない
    bool m_isFall;

    bool m_isBoss = false;
    //0:ゲームクリアか1:ゲームオーバーか
    int m_crea = 0;
    //画面移動する際一回だけエネミーを削除
    bool m_isFirst = false;
    //ボス音楽
    int m_bossBgm = -1;
    //サウンドの音量
    int m_soundVolume;

    int tempScreenH_ = -1;//画面効果用スクリーンハンドル
    int quakeTimer_ = 0;//画面揺れタイマー
    float quakeX_ = 0.0f;//横揺れパラメータ
};

