#pragma once
#include "../Util/Geometry.h"

namespace Button
{
	enum ButtonType
	{
		ButtonType_A = 0,
		ButtonType_B,
		ButtonType_X,
		ButtonType_Y,
		ButtonType_START,
		ButtonType_BACK,
		
		ButtonType_Max,

		ARROWS_NORMAL = 0,
		ARROWS_LEFT,
		ARROWS_UP,
		ARROWS_RIGHT,
		ARROWS_DOWN,

		ARROWS_MAX
	};

	void Load();
	void Init();
	void Unload();

	//どのボタンを押したか
	void PushButton(ButtonType type);
	//どの矢印ボタンを押したか
	void PushArrow(ButtonType type);

	//画像アニメーション
	void Update();
	//表示する
	void Draw(Position2 pos);

	//次へボタンのアニメーション
	void NextUpdate();
	//次へボタンを表示する
	void NextDraw(Position2 pos);
}

