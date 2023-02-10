#pragma once
#include "../Util/Geometry.h"

class HpBar
{
public:
	HpBar();
	virtual ~HpBar();

	void Init(int handle);
	
	void MaxHp(int maxHp);
	
	void Update();
	void Draw(bool isPlayer);

	//HPを取得する
	int GetHp() const { return m_Hp; }

	//ダメージ計算
	void Damage(int damage);
	//回復計算
	void Heal(const int heal);
private:
	virtual void SetHp(const int hp);

	static constexpr int kHpMax = 28;

	int m_MaxHp;
	//現在のHP
	int m_Hp;
	//HPの画像
	int m_HpHandle;

	//HP画像表示座標
	Position2 m_pos;
	//画像の幅と高さ
	Size m_size;

	//HP画像を何枚表示するか
	bool m_isHpGraph[kHpMax];
};

