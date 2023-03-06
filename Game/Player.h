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
	//当たり判定対象か
	bool IsCollidable()const;
	//ダメージを受けたか
	void Damage(int damage);
	//存在しているかどうか
	bool IsExist()const { return m_isExist; }
	//サイズを大きくする
	void ScaleEnlarge(float scale);
private:
	//普通のアップデート
	void NormalUpdate();
	//普通の描画
	void NormalDraw();

	void (Player::* m_updateFunc)();//アップデートの遷移
	void (Player::* m_drawFunc)();//描画の遷移
	
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

	std::shared_ptr<HpBar> m_hp;
};

