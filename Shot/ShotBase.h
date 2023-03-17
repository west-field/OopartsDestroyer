#pragma once
#include "../Util/Geometry.h"

enum class ShotType
{
	RockBuster,
	ShotBattery,
};

/// <summary>
/// 弾基底クラス
/// </summary>
class ShotBase
{
public:
	ShotBase(int handle);
	virtual ~ShotBase();

	//ショット開始
	virtual void Start(Position2 pos, Vector2 vel, bool left, bool isPlayer) = 0;
	//更新
	virtual void Update() = 0;
	//描画
	virtual void Draw() = 0;
	//左に撃ったかどうか
	virtual bool IsLeft() const { return m_isLeft; }
	//存在するか
	virtual bool IsExist() const { return m_isExist; }
	//消すため
	void SetExist(bool isExist) { m_isExist = isExist; }
	//プレイヤーが撃った弾か
	virtual void PlayerShot(bool isPlayerShot) { m_isPlayerShot = isPlayerShot; }
	/// <summary>
	/// プレイヤーが撃った弾か
	/// </summary>
	/// <returns>true:プレイヤーが撃った false:敵が撃った</returns>
	virtual bool IsPlayerShot()const { return m_isPlayerShot; }
	//弾を移動
	virtual void Movement(Vector2 vec) = 0;
	//矩形を取得
	virtual const Rect& GetRect()const;
	//攻撃力
	virtual const int AttackPower()const = 0;
protected:
	
	int m_handle;//グラフィックハンドル
	
	Rect m_rect;//矩形

	Vector2 m_vel;	//弾のスピード
	
	bool m_isExist;//存在するか
	
	bool m_isLeft;//左に撃ったかどうか
	
	bool m_isPlayerShot;//プレイヤーが撃った弾か
};

