#pragma once
#include "../Util/Geometry.h"

class HpBar
{
public:
	HpBar(Position2 pos);
	virtual ~HpBar();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="handle">画像ハンドル</param>
	void Init(int handle);
	
	/// <summary>
	/// 最大HPを指定する
	/// </summary>
	/// <param name="maxHp">最大HP</param>
	void MaxHp(int maxHp);
	/// <summary>
	/// 更新プレイヤー
	/// </summary>
	void UpdatePlayer();
	/// <summary>
	/// 更新エネミー
	/// </summary>
	void UpdateBoss();
	/// <summary>
	/// 表示
	/// </summary>
	void Draw();

	/// <summary>
	/// HPを取得する
	/// </summary>
	/// <returns>現在のHP</returns>
	int GetHp() const { return m_Hp; }
	/// <summary>
	/// 最大HPを取得する
	/// </summary>
	/// <returns>最大HP</returns>
	int GetMaxHp()const { return m_MaxHp; }
	/// <summary>
	/// ダメージ計算
	/// </summary>
	/// <param name="damage">受けたダメージ量</param>
	void Damage(int damage);
	/// <summary>
	/// 回復計算
	/// </summary>
	/// <param name="heal">回復量</param>
	void Heal(const int heal);
private:
	/// <summary>
	/// HPが最大値または0を超えていないか
	/// </summary>
	/// <param name="hp">計算したHP</param>
	virtual void SetHp(const int hp);

	int m_MaxHp;//最大HP
	int m_Hp;//現在のHP
	int m_HpHandle;	//HPの画像

	Rect m_rect;

	int idxX = 0;
	int idxY = 0;
};

