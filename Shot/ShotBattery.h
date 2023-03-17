#pragma once
#include "ShotBase.h"

class ShotBattery : public ShotBase
{
public:
	ShotBattery(int handle);
	virtual ~ShotBattery();

	/// <summary>
	/// ショット開始
	/// </summary>
	/// <param name="pos">作成位置</param>
	/// <param name="vel">移動方向</param>
	/// <param name="left">左右どちらを向いているか</param>
	/// <param name="isPlayer">プレイヤーが撃った弾か</param>
	virtual void Start(Position2 pos, Vector2 vel,bool left, bool isPlayer)override;
	///更新
	virtual void Update()override;
	///表示
	virtual void Draw()override;
	/// <summary>
	/// 移動する
	/// </summary>
	/// <param name="vec">画面移動</param>
	virtual void Movement(Vector2 vec);
	/// <summary>
	/// 攻撃力を返す
	/// </summary>
	/// <returns>攻撃力</returns>
	virtual const int AttackPower()const override;
private:
	
};

