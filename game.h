#pragma once

namespace Game
{
#ifdef _DEBUG
	//ウィンドウモード設定
	constexpr bool kWindowMode = true;
#else
	constexpr bool kWindowMode = false;
#endif
	//ウィンドウ名
	const wchar_t* const kTitleText = L"ロックマン";
	//ウィンドウサイズ
	constexpr int kScreenWidth = 1600;//16:
	constexpr int kScreenHeight = 900;//9
	//カラーモード
	constexpr int kColorDepth = 32;	//32 or 16
	//マップチップサイズ
	constexpr int ChipSize = 32;
	constexpr float kScale = 2.0f;

	//画面におけるマップチップの数
	 constexpr int kNumX = kScreenWidth / ChipSize;
	 constexpr int kNumY = kScreenHeight / ChipSize;

	//画面に表示できるマップの大きさ
	 constexpr int kMapNumX = 16;
	 constexpr int kMapNumY = 15;
	//マップを表示する画面左上座標
	 constexpr int kMapScreenLeftX = (kScreenWidth - (kMapNumX-1) * ChipSize)/2;
	 constexpr int kMapScreenTopY = (kScreenHeight - kMapNumY * ChipSize)/2;
	//マップを表示する画面右下座標
	 constexpr int kMapScreenRightX = kMapScreenLeftX + kMapNumX * ChipSize;
	 constexpr int kMapScreenBottomY = kMapScreenTopY + kMapNumY * ChipSize;

	//マップの大きさ
	 constexpr int kMapChipNumX = 128+16+58+16;//219;
	 constexpr int kMapChipNumY = 135;//135;
};