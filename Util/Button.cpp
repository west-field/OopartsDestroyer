#include "Button.h"
#include "Font.h"
#include "DrawFunctions.h"
#include "../game.h"
#include <DxLib.h>
#include <array>

namespace
{
	//�L�[
	constexpr int kButtonSize = 480;
	constexpr float kButtonDrawScale = 0.1f;
	constexpr int kButtonDrawSize = kButtonSize * kButtonDrawScale;
	constexpr int kButtonPushFrame = 5;

	constexpr float kArrowDrawScale = 0.3f;

	struct ButtonImg
	{
		Position2 buttonPos;//�{�^���̈ʒu
		int buttonIdxX = 0;
		int buttonIdxY = 0;
		int buttonFram = 0;
	};
	std::array<ButtonImg, Button::ButtonType_Max> button;

	ButtonImg  arrows;

	int buttonHandle;
	int arrowHandle;
}

namespace Button
{
	void Load()
	{
		buttonHandle = my::MyLoadGraph(L"Data/button.png");
		arrowHandle = my::MyLoadGraph(L"Data/arrows.png");
		Init();
	}
	void Init()
	{
		int i = 0;
		for (auto& but : button)
		{
			but.buttonIdxY = i;
			i++;
		}

		arrows.buttonPos = { static_cast<float>(kButtonDrawSize/2),static_cast<float>(kButtonDrawSize) };

		button[ButtonType_X].buttonPos = { static_cast<float>(kButtonDrawSize * 2),static_cast<float>(kButtonDrawSize) };
		button[ButtonType_Y].buttonPos = { static_cast<float>(kButtonDrawSize * 3),static_cast<float>(0) };
		button[ButtonType_B].buttonPos = { static_cast<float>(kButtonDrawSize * 4),static_cast<float>(kButtonDrawSize) };
		button[ButtonType_A].buttonPos = { static_cast<float>(kButtonDrawSize * 3),static_cast<float>(kButtonDrawSize * 2) };

		button[ButtonType_START].buttonPos = { static_cast<float>(kButtonDrawSize),static_cast<float>( kButtonDrawSize/2) };
		button[ButtonType_BACK].buttonPos = { static_cast<float>(kButtonDrawSize*0.05),static_cast<float>( kButtonDrawSize/2) };
	}
	void Unload()
	{
		DeleteGraph(buttonHandle);
		DeleteGraph(arrowHandle);
	}

	void PushButton(ButtonType type)
	{
		button[type].buttonIdxX = 1;
	}
	void PushArrow(ButtonType type)
	{
		arrows.buttonIdxX = static_cast<int>(type);
	}
	void Update()
	{
		for (auto& but : button)
		{
			if (but.buttonIdxX == 0) continue;
			if (but.buttonFram++ >= kButtonPushFrame)
			{
				but.buttonIdxX = (but.buttonIdxX + 1) % 3;
				but.buttonFram = 0;
			}
		}

		arrows.buttonIdxX = 0;
	}
	void Draw(Position2 pos)
	{
		//���
		my::MyDrawRectRotaGraph(arrows.buttonPos.x + pos.x, arrows.buttonPos.y + pos.y,
			arrows.buttonIdxX * kButtonSize, arrows.buttonIdxY * kButtonSize,
			kButtonSize, kButtonSize,
			kArrowDrawScale, 0.0f, arrowHandle, true, false);

		pos += {kButtonDrawSize * 3, 0.0f};
		//�{�^��
		for (auto& but : button)
		{
			my::MyDrawRectRotaGraph(but.buttonPos.x+pos.x, but.buttonPos.y+pos.y,
				but.buttonIdxX * kButtonSize, but.buttonIdxY * kButtonSize,
				kButtonSize, kButtonSize,
				kButtonDrawScale, 0.0f, buttonHandle, true, false);
		}
	}
	void NextUpdate()
	{
		if (button[ButtonType_A].buttonFram++ >= kButtonPushFrame*2)
		{
			button[ButtonType_A].buttonIdxX = (button[ButtonType_A].buttonIdxX + 1) % 3;
			button[ButtonType_A].buttonFram = 0;
		}
	}
	void NextDraw(Position2 pos)
	{
		int PosX = button[ButtonType_A].buttonPos.x + pos.x;
		int PosY = button[ButtonType_A].buttonPos.y + pos.y;
		
		int size = 20;
		//�w�i
		DrawBox(PosX- kButtonDrawSize/2, PosY - kButtonDrawSize / 2,
			PosX - kButtonDrawSize / 2 + kButtonDrawSize + size * 4, PosY - kButtonDrawSize / 2 + kButtonDrawSize, 0xaaaaaa, true);
		//�{�^��
		my::MyDrawRectRotaGraph(PosX, PosY,
			button[ButtonType_A].buttonIdxX * kButtonSize, button[ButtonType_A].buttonIdxY * kButtonSize,
			kButtonSize, kButtonSize,
			kButtonDrawScale, 0.0f, buttonHandle, true, false);
		//����
		Font::ChangeFontSize(size);
		DrawString(PosX - kButtonDrawSize / 2 + kButtonDrawSize, PosY-10, L"�m�d�w�s", 0x000000);
		Font::ChangeFontSize(0);
	}
}