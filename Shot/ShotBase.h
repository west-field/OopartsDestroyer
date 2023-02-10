#pragma once
#include "../Util/Geometry.h"

enum class ShotType
{
	RockBuster,
};

/// <summary>
/// 弾基底クラス
/// </summary>
class ShotBase
{
public:
	ShotBase();
	virtual ~ShotBase();

	void SetHandle(int handle);
	//ショット開始
	virtual void Start(Position2 pos, bool left) = 0;
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
	virtual bool GetPlayerShot()const { return m_isPlayerShot; }
	//弾を移動
	virtual void Movement(Vector2 vec);
	
	virtual const Rect& GetRect()const;

	virtual const int AttackPower()const = 0;
protected:
	
	int m_handle;//グラフィックハンドル
	
	Rect m_rect;

	Vector2 m_vel;	//弾のスピード
	
	bool m_isExist;//存在するか
	
	bool m_isLeft;//左に撃ったかどうか
	
	bool m_isPlayerShot;//プレイヤーが撃った弾か
};

