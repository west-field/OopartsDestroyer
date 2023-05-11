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

class Object;
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
    /// 当たり判定＆移動
    /// </summary>
    /// <param name="object">判定するObject</param>
    /// <param name="MoveX">移動量X</param>
    /// <param name="MoveY">移動量Y</param>
    void Move(std::shared_ptr<Object> object, float MoveX, float MoveY);
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

    std::shared_ptr<Map> m_map;         //マップ
    Position2 m_add;                    //プレイヤーがどのくらい移動しているか
    int m_framecolor = 0xc0c0c0;        //灰色　マップの枠の色
    
    std::shared_ptr<Player> m_player;   //プレイヤー
    float m_fallPlayerSpeed;            //落ちていくスピード
    bool m_isPlayerCenterLR;            //プレイヤーが画面の中心に居るか
    
    bool m_isScreenMoveUp;              //プレイヤーが画面移動位置にいるかどうか（上に移動）
    bool m_isScreenMoveDown;            //プレイヤーが画面移動位置にいるかどうか（下に移動）
    bool m_isScreenMoveWidth;           //プレイヤーが画面移動位置にいつかどうか（横に移動）
    float m_playerPos;                  //移動するときの基準になるプレイヤーの位置

    
    std::shared_ptr <ShotFactory> m_shotFactory;    //ショット
    
    std::shared_ptr <ItemFactory> m_itemFactory;    //アイテム
    
    std::shared_ptr <EnemyFactory> m_enemyFactory;  //エネミー
    Vector2 m_correction;                           //画面を動かしたとき位置を補正する
    
    enum HPBAR
    {
        Object_Player,
        Object_EnemyBoss,

        Object_Max
    };
    
    std::array<std::shared_ptr<HpBar>, Object_Max> m_hp;//HPバー

    bool m_isLadder;        //梯子を上っているときtrue
    bool m_isLadderAlign;   //梯子を上っているときは位置を合わせる
    bool m_isLadderFirst;   //梯子を上り終えたとき一度だけ上に移動する
    
    bool m_isFall;          //落ちているかどうか 落ちているときはジャンプできない

    bool m_isBoss;          //ボス戦に入ったかどうか
    
    int m_crea;             //0:ゲームクリアか1:ゲームオーバーか
    
    bool m_isFirst;         //画面移動する際一回だけエネミーを削除
    
    int m_bossBgm;          //ボス音楽
    int m_soundVolume;      //サウンドの音量

    int m_tempScreenH;      //画面効果用スクリーンハンドル
    int m_quakeTimer ;      //画面揺れタイマー
    float m_quakeX;         //横揺れパラメータ
};

