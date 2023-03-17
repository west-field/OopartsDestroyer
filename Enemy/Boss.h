#pragma once
#include "EnemyBase.h"
#include "../Util/Geometry.h"

/// <summary>
/// カットマン（ボス） 攻撃力,接触:4
/// </summary>
class Boss :public EnemyBase
{
public :
	Boss(std::shared_ptr<Player>player, const Position2& pos,int handle, int bossBurstH, int burstH, std::shared_ptr<ShotFactory> sFactory, std::shared_ptr<ItemFactory> itFactory, std::shared_ptr<HpBar>hp);
	virtual ~Boss();
	virtual void Update()override;
	virtual void Draw()override;

	/// <summary>
	/// 接触したときの攻撃
	/// </summary>
	/// <returns>接触したときの攻撃力</returns>
	virtual int TouchAttackPower() const override;
	/// <summary>
	/// ダメージを受けた
	/// </summary>
	/// <param name="damage">ダメージ量</param>
	virtual void Damage(int damage) override;
	/// <summary>
	/// あたり判定対象か
	/// </summary>
	/// <returns>true:当たる false:当たらない</returns>
	virtual bool IsCollidable()const override;
private:
	void NormalUpdate();//次の指示を決める
	void NormalDraw();//通常描画

	void BurstUpdate();//爆発更新
	void BurstDraw();//爆発表示

	void ShotUpdate();//ボスが弾を撃つ

	void (Boss::* m_updateFunc)();
	void (Boss::* m_drawFunc)();

	int m_bossBurstH = -1;//ボス爆発画像

	int m_idx = 0;//画像の表示範囲
	
	int m_animFrame = 0;//次の画像に移動(ボス
	
	int m_frame = 0;//次の攻撃をするまでの時間
	
	int m_shotFrame = 20;//二回攻撃するとき次の弾を撃つまで
};

