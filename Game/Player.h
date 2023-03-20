#pragma once
#include "../Util/Geometry.h"
#include <memory>
class HpBar;

enum class ActionType
{
	grah_idle,
	grah_walk,
	grah_reload,
	grah_jump,
	grah_attack,
	grah_death,

	grah_max
};

class Player
{
public:
	Player(Position2 pos,std::shared_ptr<HpBar>hp);
	virtual ~Player();
	void Update();
	void Draw();

	/// <summary>
	/// プレイヤー移動
	/// </summary>
	/// <param name="vec">移動量</param>
	void Movement(Vector2 vec);
	
	/// <summary>
	/// 矩形を取得
	/// </summary>
	/// <returns矩形構造体></returns>
	const Rect& GetRect()const;
	
	/// <summary>
	/// 左を向いているかどうかを返す
	/// </summary>
	/// <returns>true:左を向いている false:右を向いている</returns>
	bool IsLeft()const { return m_isLeft; }
	/// <summary>
	/// 左を向いているかどうか
	/// </summary>
	/// <param name="isLeft">左を向いているか</param>
	void SetLeft(bool isLeft) { m_isLeft = isLeft; }
	
	/// <summary>
	/// ジャンプしているかどうかを返す
	/// </summary>
	/// <returns>true:ジャンプしている false:ジャンプしていない</returns>
	bool IsJump()const { return m_isJump; }
	/// <summary>
	/// ジャンプをしているかしていないか
	/// </summary>
	/// <param name="isJump">ジャンプしているか</param>
	void SetJump(bool isJump) { m_isJump = isJump; }
	
	/// <summary>
	/// 今どんな行動をしているのか
	/// </summary>
	/// <param name="type">行動タイプ</param>
	void Action(ActionType type);
	
	/// <summary>
	/// 当たり判定対象か
	/// </summary>
	/// <returns>true:当たる false:当たらない</returns>
	bool IsCollidable()const;
	
	/// <summary>
	/// ダメージを受けた
	/// </summary>
	/// <param name="damage">ダメージ量</param>
	void Damage(int damage);
	/// <summary>
	/// 回復する
	/// </summary>
	/// <param name="Heal">回復量</param>
	void Heal(int Heal);
	
	/// <summary>
	/// 存在しているかどうか
	/// </summary>
	/// <returns>true:存在する false:存在しない</returns>
	bool IsExist()const { return m_isExist; }
	
	/// <summary>
	/// サイズを大きくする
	/// </summary>
	/// <param name="scale">拡大率</param>
	void ScaleEnlarge(float scale);
private:
	Rect m_rect;//プレイヤーの矩形
	
	int m_handle = -1;//プレイヤーのグラフィックハンドル
	int m_idxX = 0;//表示する描画位置X
	int m_idxY = 2;//表示する描画位置Y

	float m_drawScale = 0.0f;//拡大率

	int m_frame = 0;//アニメーション時間

	bool m_isLeft = false;//左を向いている
	
	bool m_isJump = false;//ジャンプをしているか

	int m_ultimateTimer = 0;//無敵時間

	bool m_isExist = true;//存在しているかどうか

	std::shared_ptr<HpBar> m_hp;//HP

	bool m_isHeal = false;//回復アイテムをとったか
	int m_healH = -1;//回復エフェクト
	int m_healIdx = 0;//画像の表示位置
	int m_healAnimFram = 0;//次の画像に移動
};

