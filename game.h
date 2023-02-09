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
	static constexpr int kNumX = kScreenWidth / ChipSize;
	static constexpr int kNumY = kScreenHeight / ChipSize;

	//画面に表示できるマップの大きさ
	static constexpr int kMapNumX = 16;
	static constexpr int kMapNumY = 16;
	//マップを表示する画面サイズ
	static constexpr int kMapScreenX = (kScreenWidth - kMapNumX * ChipSize)/2;
	static constexpr int kMapScreenY = (kScreenHeight - kMapNumY * ChipSize)/2;

	//マップの大きさ
	static constexpr int kMapChipNumX = 96;//219;
	static constexpr int kMapChipNumY = 92;//135;
};