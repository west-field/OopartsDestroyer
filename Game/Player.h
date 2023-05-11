#pragma once
#include "Object.h"
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

class Player :public Object
{
public:
	Player(Position2 pos,std::shared_ptr<HpBar>hp);
	virtual ~Player();
	void Update();
	void Draw();
	
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
	virtual void Damage(int damage) override;
	/// <summary>
	/// 回復する
	/// </summary>
	/// <param name="Heal">回復量</param>
	void Heal(int Heal);
	
	/// <summary>
	/// サイズを大きくする
	/// </summary>
	/// <param name="scale">拡大率</param>
	void ScaleEnlarge(float scale);

	/// <summary>
	/// マップチップを入手する
	/// </summary>
	/// <param name="chipId">マップチップ</param>
	virtual void GetChip(int chipId) override {}
private:
	
	int m_idxX = 0;//表示する描画位置X
	int m_idxY = 2;//表示する描画位置Y

	float m_drawScale = 0.0f;//拡大率

	int m_frame = 0;//アニメーション時間

	int m_ultimateTimer = 0;//無敵時間

	bool m_isHeal = false;//回復アイテムをとったか
	int m_healH = -1;//回復エフェクト
	int m_healIdx = 0;//画像の表示位置
	int m_healAnimFram = 0;//次の画像に移動
};

