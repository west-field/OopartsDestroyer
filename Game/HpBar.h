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
	//最大HPを取得する
	int GetMaxHp()const { return m_MaxHp; }
	//ダメージ計算
	void Damage(int damage);
	//回復計算
	void Heal(const int heal);
private:
	virtual void SetHp(const int hp);

	static constexpr int kHpMax = 20;

	int m_MaxHp;
	//現在のHP
	int m_Hp;
	//HPの画像
	int m_HpHandle;

	//HP画像表示座標
	//画像の幅と高さ
	Rect m_rect;

	int idxX = 0;
	int idxY = 0;
};

