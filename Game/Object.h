#pragma once

#include "../Util/Geometry.h"
#include <memory>
#include "../ObjectInfo.h"

class HpBar;

class Object
{
public:
	Object();
	virtual ~Object();

	virtual void Update() = 0;
	virtual void Draw() = 0;

	/// <summary>
	/// 移動
	/// </summary>
	/// <param name="vec">移動量</param>
	void Movement(Vector2 vec);
	/// <summary>
	/// 矩形を取得
	/// </summary>
	/// <returns>矩形構造体</returns>
	const Rect& GetRect() const;

	/// <summary>
	/// 左を向いているかどうかを返す
	/// </summary>
	/// <returns>true:左を向いている　false:右を向いている</returns>
	bool IsLeft()const;

	/// <summary>
	/// 左を向いているかどうかを入力
	/// </summary>
	/// <param name="isLeft">true:左を向いている　false:右を向いている</param>
	void SetLeft(bool isLeft);

	/// <summary>
	/// ジャンプしているかどうかを返す
	/// </summary>
	/// <returns>true:ジャンプしている　false:ジャンプしていない</returns>
	bool IsJump()const;

	/// <summary>
	/// ジャンプをしているかしていないかを決める
	/// </summary>
	/// <param name="isJump">true:ジャンプしている　false:ジャンプしていない</param>
	void SetJump(bool isJump);

	/// <summary>
	/// ダメージを受けた
	/// </summary>
	/// <param name="damage">ダメージ量</param>
	virtual void Damage(int damage) = 0;

	/// <summary>
	/// 存在しているかどうか
	/// </summary>
	/// <returns>true:存在する false:存在しない</returns>
	bool IsExist()const { return m_isExist; }

	/// <summary>
	/// 消すため
	/// </summary>
	/// <param name="isExist">存在を消す</param>
	virtual void EraseExist();

	/// <summary>
	/// 移動量を返す
	/// </summary>
	/// <returns>移動</returns>
	virtual Vector2 GetVec()const;

	/// <summary>
	/// マップチップを入手する
	/// </summary>
	/// <param name="chipId">マップチップ</param>
	virtual void GetChip(int chipId) = 0;

	/// <summary>
	/// 何を作ったのか
	/// </summary>
	/// <returns>オブジェクトの種類</returns>
	ObjectType Type()const { return m_type; }
protected:
	std::shared_ptr<HpBar> m_hp;

	Rect m_rect;//矩形

	int m_handle;//画像ハンドル

	int m_idx;//画像変更

	bool m_isExist;//存在するかどうか

	bool m_isLeft;//左を向いているか

	bool m_isJump;//ジャンプしているか

	ObjectType m_type;//何を作ったのか
};

