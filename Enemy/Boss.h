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
	/// あたり判定対象か
	/// </summary>
	/// <returns>true:当たる false:当たらない</returns>
	virtual bool IsCollidable()const override;

	/// <summary>
	/// ダメージを受けた
	/// </summary>
	/// <param name="damage">ダメージ量</param>
	virtual void Damage(int damage) override;
	
private:
	//通常
	void NormalUpdate();
	void NormalDraw();

	//爆発
	void BurstUpdate();
	void BurstDraw();

	//ボスが弾を撃つ
	void ShotUpdate();

	void (Boss::* m_updateFunc)();
	void (Boss::* m_drawFunc)();

	int m_bossBurstH;	//ボス爆発画像

	int m_idx;			//画像の表示範囲
	
	int m_animFrame;	//次の画像に移動(ボス
	
	int m_frame;		//次の攻撃をするまでの時間
	
	int m_shotFrame;	//二回攻撃するとき次の弾を撃つまで
};

