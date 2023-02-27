#pragma once
#include "EnemyBase.h"
#include "../Util/Geometry.h"

/// <summary>
/// カットマン（ボス） 攻撃力,接触:4
/// </summary>
class CutMan :public EnemyBase
{
public :
	CutMan(std::shared_ptr<Player>player, const Position2& pos,int handle, std::shared_ptr<ShotFactory> sFactory);
	virtual ~CutMan();
	virtual void Update();
	virtual void Draw();

	virtual void Movement(Vector2 vec)override;
	virtual int TouchAttackPower() const override;
private:
	virtual void MoveUpdate();//ボスを移動させる
	virtual void StopUpdate();//次の指示を決める
	virtual void JumpUpdate();//ボスをジャンプさせる
	virtual void OneShotUpdate();//ボスが1回弾を撃つ
	virtual void TwoShotUpdate();//ボスが2回弾を撃つ
	void (CutMan::* updateFunc)();

	int m_idx = 0;//画像の表示範囲
	int m_shotFrame;//次の弾を撃つまで
	int m_JumpFrame;//次のジャンプまで
};

