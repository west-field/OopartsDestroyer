#pragma once
#include <memory>
#include "../Util/Geometry.h"

class Player;

/// <summary>
/// 敵基底クラス
/// </summary>
class EnemyBase
{
public:
	EnemyBase(std::shared_ptr<Player>player,const Position2& pos);
	virtual ~EnemyBase();

	virtual void Update() = 0;
	virtual void Draw() = 0;

	//エネミー移動
	virtual void Movement(Vector2 vec) = 0;

	const Rect& GetRect() const;

	//ダメージを受けた
	virtual void Damage(int damage) = 0;
	//この敵は生きているか
	bool IsExist()const;
	//消すため
	void SetExist(bool isExist) { m_isExist = isExist; }

	//左を向いているかどうかを返す
	bool IsLeft()const { return m_isLeft; }
	//左を向いているかどうか
	void SetLeft(bool isLeft) { m_isLeft = isLeft; }

	//ジャンプしているかどうかを返す
	bool IsJump()const { return m_isJump; }
	//ジャンプをしているかしていないかを決める
	void SetJump(bool isJump) { m_isJump = isJump; }
protected:
	std::shared_ptr<Player> m_player = nullptr;
	bool m_isExist = true;//存在するか
	Rect m_rect;//敵の矩形

	bool m_isLeft = false;	//左を向いている

	bool m_isJump = false;	//ジャンプをしているか
};

