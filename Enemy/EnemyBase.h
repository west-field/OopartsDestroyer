#pragma once
#include <memory>
#include "../Util/Geometry.h"

class Player;			//プレイヤー
class HpBar;			//HPバー
class ShotFactory;		//ショット

/// <summary>
/// 敵基底クラス
/// </summary>
class EnemyBase
{
public:
	EnemyBase(std::shared_ptr<Player>player,const Position2& pos,std::shared_ptr<ShotFactory> sFactory);
	virtual ~EnemyBase();

	virtual void Update() = 0;//更新
	virtual void Draw() = 0;//表示

	//エネミー移動
	virtual void Movement(Vector2 vec) = 0;
	//矩形を取得
	const Rect& GetRect() const;

	/// <summary>
	/// 接触した時の攻撃
	/// </summary>
	/// <returns>接触した時の攻撃力</returns>
	virtual int TouchAttackPower()const = 0;
	/// <summary>
	/// ダメージを受けた
	/// </summary>
	/// <param name="damage">ダメージ量</param>
	void Damage(int damage);
	/// <summary>
	/// この敵は生きているか
	/// </summary>
	/// <returns>true:生きている　false:生きていない</returns>
	bool IsExist()const;
	/// <summary>
	/// 消すため
	/// </summary>
	/// <param name="isExist">存在を消す</param>
	void SetExist(bool isExist) { m_isExist = isExist; }

	/// <summary>
	/// 左を向いているかどうかを返す
	/// </summary>
	/// <returns>true:左を向いている　false:右を向いている</returns>
	bool IsLeft()const { return m_isLeft; }
	/// <summary>
	/// 左を向いているかどうかを入力
	/// </summary>
	/// <param name="isLeft">true:左を向いている　false:右を向いている</param>
	void SetLeft(bool isLeft) { m_isLeft = isLeft; }

	/// <summary>
	/// ジャンプしているかどうかを返す
	/// </summary>
	/// <returns>true:ジャンプしている　false:ジャンプしていない</returns>
	bool IsJump()const { return m_isJump; }
	/// <summary>
	/// ジャンプをしているかしていないかを決める
	/// </summary>
	/// <param name="isJump">true:ジャンプしている　false:ジャンプしていない</param>
	void SetJump(bool isJump) { m_isJump = isJump; }

	Vector2 GetVec()const { return m_vec; }

	void OnDamage();

	/// <summary>
	/// マップチップを入手する
	/// </summary>
	/// <param name="chipId">マップチップ</param>
	void GetChip(int chipId) { m_chipId = chipId; }
protected:
	std::shared_ptr<Player> m_player = nullptr;				//プレイヤー
	std::shared_ptr<HpBar> m_hp = nullptr;					//Hpバー
	std::shared_ptr<ShotFactory> m_shotFactory = nullptr;	//ショット

	bool m_isExist = true;	//存在するか
	Rect m_rect;	//敵の矩形

	Vector2 m_vec;//移動速度

	int m_handle = -1;

	bool m_isLeft = false;	//左を向いている

	bool m_isJump = false;	//ジャンプをしているか

	int m_chipId;//マップチップ
	bool m_isOnDamage = false;//ダメージを受けたかどうか
};

