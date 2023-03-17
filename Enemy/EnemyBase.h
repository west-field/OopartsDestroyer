#pragma once
#include <memory>
#include "../Util/Geometry.h"

class Player;			//プレイヤー
class HpBar;			//HPバー
class ShotFactory;		//ショット
class ItemFactory;		//アイテム

/// <summary>
/// 敵基底クラス
/// </summary>
class EnemyBase
{
public:
	EnemyBase(std::shared_ptr<Player>player,const Position2& pos,std::shared_ptr<ShotFactory> sFactory, std::shared_ptr<ItemFactory> itFactory);
	virtual ~EnemyBase();

	virtual void Update() = 0;//更新
	virtual void Draw() = 0;//表示

	/// <summary>
	/// エネミー移動
	/// </summary>
	/// <param name="vec">移動量</param>
	void Movement(Vector2 vec);
	/// <summary>
	/// 矩形を取得
	/// </summary>
	/// <returns>矩形構造体</returns>
	const Rect& GetRect() const;
	/// <summary>
	/// 当たり判定対象か
	/// </summary>
	/// <returns>true:当たる false:当たらない</returns>
	virtual bool IsCollidable()const = 0;

	/// <summary>
	/// 接触した時の攻撃
	/// </summary>
	/// <returns>接触した時の攻撃力</returns>
	virtual int TouchAttackPower()const = 0;
	/// <summary>
	/// ダメージを受けた
	/// </summary>
	/// <param name="damage">ダメージ量</param>
	virtual void Damage(int damage) = 0;
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
	void SetJump(bool isJump) { m_isJump = isJump; m_vec = {}; }

	/// <summary>
	/// 移動量を返す
	/// </summary>
	/// <returns>移動</returns>
	Vector2 GetVec()const { return m_vec; }

	/// <summary>
	/// 現在のHPを返す
	/// </summary>
	/// <returns>現在のHP</returns>
	int GetHp() const;

	/// <summary>
	/// マップチップを入手する
	/// </summary>
	/// <param name="chipId">マップチップ</param>
	void GetChip(int chipId) { m_chipId = chipId; }
protected:
	std::shared_ptr<Player> m_player = nullptr;				//プレイヤー
	std::shared_ptr<HpBar> m_hp = nullptr;					//Hpバー
	std::shared_ptr<ShotFactory> m_shotFactory = nullptr;	//ショット
	std::shared_ptr<ItemFactory> m_itemFactory = nullptr;	//アイテム

	bool m_isExist = true;	//存在するか
	Rect m_rect;	//敵の矩形

	Vector2 m_vec;//移動速度

	int m_handle = -1;//画像ハンドル
	int m_burstHandle = -1;//爆発画像ハンドル

	int m_idx = 0;//画像変更

	bool m_isLeft = false;	//左を向いている

	bool m_isJump = false;	//ジャンプをしているか

	int m_ultimateTimer = 0;//無敵時間

	int m_chipId;//マップチップID
	bool m_isOnDamage = false;//ダメージを受けたかどうか
};

