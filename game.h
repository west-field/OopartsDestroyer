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
	const wchar_t* const kTitleText = L"オーパーツ";
	//ウィンドウサイズ
	constexpr int kScreenWidth = 1600;//16:
	constexpr int kScreenHeight = 900;//9
	//カラーモード
	constexpr int kColorDepth = 32;	//32 or 16
	//マップチップサイズ
	constexpr int ChipSize = 32;
	constexpr float kScale = 1.5f;

	constexpr int kDrawSize = static_cast<int>(ChipSize * kScale);

	//画面におけるマップチップの数
	 constexpr int kNumX = kScreenWidth / kDrawSize;
	 constexpr int kNumY = kScreenHeight / kDrawSize;

	//画面に表示できるマップの大きさ
	 constexpr int kMapNumX = 16;
	 constexpr int kMapNumY = 15;
	//マップを表示する画面左上座標
	 constexpr int kMapScreenLeftX = (kScreenWidth - (kMapNumX-1) * kDrawSize)/2;
	 constexpr int kMapScreenTopY = (kScreenHeight - kMapNumY * kDrawSize)/2;
	//マップを表示する画面右下座標
	 constexpr int kMapScreenRightX = kMapScreenLeftX + kMapNumX * kDrawSize;
	 constexpr int kMapScreenBottomY = kMapScreenTopY + kMapNumY * kDrawSize;

	//マップの大きさ X218*Y135
	 constexpr int kMapChipNumX = 218;//219;
	 constexpr int kMapChipNumY = 135;//135;
};