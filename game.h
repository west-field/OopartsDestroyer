#pragma once

namespace Game
{
	//ウィンドウモード設定
	constexpr bool kWindowMode = true;
	//ウィンドウ名
	const wchar_t* const kTitleText = L"ロックマン";
	//ウィンドウサイズ
	constexpr int kScreenWidth = 1600;//16
	constexpr int kScreenHeight = 900;//9
	//カラーモード
	constexpr int kColorDepth = 32;	//32 or 16
	//マップチップサイズ
	constexpr int ChipSize = 32;

	//画面におけるマップチップの数
	static constexpr int kNumX = kScreenWidth / ChipSize;
	static constexpr int kNumY = kScreenHeight / ChipSize;
	//マップの大きさ
	static constexpr int kMapChipNumX = kNumX + 46;// 219;// Game::kScreenWidth / Game::ChipSize * 2;
	static constexpr int kMapChipNumY = kNumY + 47;//135;//Game::kScreenHeight / Game::ChipSize;
};