#pragma once
#include <array>
#include "Geometry.h"

enum class BgGraph
{
	BgGraph_1,
	BgGraph_2,
	BgGraph_3,
	BgGraph_4,
	BgGraph_5,
	BgGraph_Max
};

class Background
{
private:
	~Background();
	//コピーも代入も禁止する
	Background(const Background&) = delete;
	void operator= (const Background&) = delete;

	/// <summary>
	/// 背景のロード
	/// </summary>
	/// <param name="id">画像名</param>
	/// <param name="fileName">ファイル名</param>
	/// <returns>背景ハンドル</returns>
	int Load(BgGraph id,const wchar_t* fileName);

	struct BackImg
	{
		int handle = -1;//画像のハンドル
		float scrollSpeed;//スクロールスピード
		Size imgSize;
	};
	std::array<BackImg, static_cast<int>(BgGraph::BgGraph_Max)> bgImgs;//背景

	int scrollx = 0;//横にスクロール
public:
	Background();
	/// <summary>
	/// BackImg使用者はGetInstance()を通した参照からしか利用できない
	/// </summary>
	/// <returns>実体の参照を返す</returns>
	static Background& GetInstance()
	{
		static Background instance;
		return instance;
	}
	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// 更新
	void Update();
	/// 表示
	void Draw();
	///マップの一部だけ表示する
	void Bg();
};

