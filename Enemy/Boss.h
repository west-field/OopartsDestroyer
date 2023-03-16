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

	virtual void Movement(Vector2 vec)override;
	virtual void EnemyMovement(Vector2 vec)override;
	virtual int TouchAttackPower() const override;
	virtual void Damage(int damage) override;
	virtual bool IsCollidable()const override;
private:
	void MoveUpdate();//ボスを移動させる
	void StopUpdate();//次の指示を決める
	void JumpUpdate();//ボスをジャンプさせる
	void DownUpdate();//ボスをジャンプさせる
	void OneShotUpdate();//ボスが1回弾を撃つ
	void TwoShotUpdate();//ボスが2回弾を撃つ
	
	void NormalDraw();

	void BurstUpdate();
	void BurstDraw();

	void (Boss::* updateFunc)();
	void (Boss::* m_drawFunc)();

	int m_bossBurstH = -1;//ボス爆発画像

	int m_idx = 0;//画像の表示範囲
	
	int m_animFrame = 0;//次の画像に移動(ボス
	
	int m_frame = 0;//次の攻撃をするまでの時間
	
	int m_shotFrame = 20;//二回攻撃するとき次の弾を撃つまで
	int m_JumpFrame;//次のジャンプまで

	float m_posTemp = 0.0f;//仮入れ

};

