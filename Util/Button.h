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

	//�ǂ̃{�^������������
	void PushButton(ButtonType type);
	//�ǂ̖��{�^������������
	void PushArrow(ButtonType type);

	//�摜�A�j���[�V����
	void Update();
	//�\������
	void Draw(Position2 pos);

	//���փ{�^���̃A�j���[�V����
	void NextUpdate();
	//���փ{�^����\������
	void NextDraw(Position2 pos);
}

