#pragma once
#include "Secne.h"
#include <vector>
#include "../Shot/ShotBase.h"
#include "../Util/Geometry.h"

#include <memory>//スマートポインタをつかうため
#include <array>//配列用

class Player;
class EnemyFactory;
class ShotBase;
class HpBar;
class Map;

enum MapChip
{
    no,			//ない       当たらない
    sky,		//空         当たらない
    terrain,	//地面       当たる
    wall1,		//壁1        当たる
    wall2,		//壁2        当たる
    ladder1,	//梯子1      当たらない
    ladder2,	//梯子2      当たらない
    stone,		//石         当たる
    thorn,		//とげ       当たる

    max
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
    /// 弾とマップの当たり判定
    /// </summary>
    /// <param name="i">何番目の弾</param>
    /// <param name="MoveX">移動量</param>
    /// <param name="MoveY">移動量</param>
    void MoveShot(int i ,float MoveX, float MoveY);
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
 
    virtual void FadeInUpdat(const InputState& input);
    virtual void NormalUpdat(const InputState& input);
    virtual void FadeOutUpdat(const InputState& input);
public:
    int m_test;//画面を揺らせる(文字を回転させる)

    unsigned int m_fadeColor = 0x000000;//フェードの色（黒
    //UpdateFuncの型
    using UpdateFunc_t = void (GameplayingScene::*)(const InputState& input);
    UpdateFunc_t m_updateFunc = nullptr;

    std::shared_ptr<Map> m_map; //マップ
    Position2 m_add;            //プレイヤーがどのくらい移動しているか
    bool m_isMapJump = false;   //マップでジャンプをしたか

    std::shared_ptr<Player> m_player;//プレイヤー
    float m_fallPlayerSpeed = 0;    //落ちていくスピード
    bool m_isPlayerCenterLR = false;//プレイヤーが画面の中心に居るか
    bool m_isPlayerCenterUD = false;//プレイヤーが画面の中心に居るか

    std::shared_ptr <EnemyFactory> m_enemyFactory;  //エネミー
    float m_fallEnemySpeed = 0;                     //落ちていくスピード    
    
    enum
    {
        Object_Player,
        Object_Enemy,

        Object_Max
    };
    std::array<std::shared_ptr<HpBar>, Object_Max> m_hp;    //HPバー
    
    //自機
    static constexpr int kShot = 15;                        //ショットの表示数
    std::array<std::shared_ptr<ShotBase>, kShot> m_shots;   //ショット
    int m_shotCount = 0;                                    //今存在しているショットの数
};

