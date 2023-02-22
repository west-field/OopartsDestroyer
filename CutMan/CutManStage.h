#pragma once
#include "../Scene/Secne.h"
#include "../game.h"
#include <memory>
#include <array>

class Player;
class Map;
class HpBar;
class Camera;

enum MapChip
{
	no,			//ない
	sky,		//空
	terrain,	//地面
	wall1,		//壁1
	wall2,		//壁2
	ladder1,	//梯子1
	ladder2,	//梯子2
	stone,		//石
	thorn,		//とげ

	max
};

class CutManStage : public Scene
{
public:
	CutManStage(SceneManager& manager);
	virtual ~CutManStage();

	virtual void Update(const InputState& input);
	virtual void Draw();
private:
	//キャラクターとマップの当たり判定
	int MoveCharactor(float MoveX, float MoveY);
	//マップとの当たり判定
	int MapHitCheck(float X, float Y, float& MoveX, float& MoveY);

	virtual void FadeInUpdat(const InputState& input);
	virtual void NormalUpdat(const InputState& input);
	virtual void FadeOutUpdat(const InputState& input);

	unsigned int m_fadeColor = 0x000000;//フェードの色（黒
	//UpdateFuncの型
	using UpdateFunc_t = void (CutManStage::*)(const InputState& input);
	UpdateFunc_t m_updateFunc = nullptr;
private:
	static constexpr int ChipSize = 16;
	static constexpr int MapWidth = (Game::kScreenWidth/ ChipSize);
	static constexpr int MapHeight = (Game::kScreenHeight / ChipSize);

private:
	//プレイヤー
	std::shared_ptr<Player> m_player;
	float	m_fallPlayerSpeed;	// プレイヤーの落下速度

	std::shared_ptr<Map> m_mapData;//マップ

	enum
	{
		Object_Player,
		Object_Enemy,

		Object_Max
	};

	std::array<std::shared_ptr<HpBar>, Object_Max> m_hp;//HP
	std::shared_ptr<Camera>m_camera;//カメラ移動
};

