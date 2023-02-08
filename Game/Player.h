#pragma once
#include "../Util/Geometry.h"

enum class ActionType
{
	grah_no,
	grah_walk,
	grah_idle,
	grah_attack,
	grah_jump,
	grah_hit,

	grah_max
};

class Player
{
public:
	Player(Position2 pos);
	virtual ~Player();
	void Update();
	void Draw();

	//プレイヤー移動
	void Movement(Vector2 vec);
	
	const Rect& GetRect()const;
	//左を向いているかどうかを返す
	bool IsLeft()const { return m_isLeft; }
	//左を向いているかどうか
	void SetLeft(bool isLeft) { m_isLeft = isLeft; }
	//ジャンプしているかどうかを返す
	bool IsJump()const { return m_isJump; }
	//ジャンプをしているかしていないかを決める
	void SetJump(bool isJump) { m_isJump = isJump; }
	//今どんな行動をしているのか
	void Action(ActionType type);
private:
	//プレイヤーの中心位置
	Rect m_rect;
	//プレイヤーのグラフィックハンドル
	int m_handle = -1;
	int m_idxX = 0;
	int m_idxY = 2;

	int m_frame = 0;

	//左を向いている
	bool m_isLeft = false;
	//ジャンプをしているか
	bool m_isJump = false;

};

