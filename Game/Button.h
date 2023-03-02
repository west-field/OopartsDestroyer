#pragma once
#include "../Util/Geometry.h"

namespace Button
{
	enum ButtonType
	{
		ButtonType_A,
		ButtonType_B,
		ButtonType_X,
		ButtonType_Y,
		ButtonType_START,
		ButtonType_BACK,
		
		ButtonType_Max
	};
	enum ARROWS
	{
		ARROWS_NORMAL,
		ARROWS_LEFT,
		ARROWS_UP,
		ARROWS_RIGHT,
		ARROWS_DOWN,

		ARROWS_MAX
	};

	void Load();
	void Init();
	void Unload();

	void PushButton(ButtonType type);
	void PushArrow(ARROWS type);

	void Update();
	void Draw(Position2 pos);
}

