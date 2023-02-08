#include "DrawFunctions.h"
#include <DxLib.h>
#include <cassert>

namespace my {
	/// <summary>
	/// グラフィックをロードする
	/// </summary>
	/// <param name="path">画像ファイルのパス</param>
	/// <returns>画像のハンドル</returns>
	int MyLoadGraph(const TCHAR* path)
	{
		int handle = LoadGraph(path);
		assert(handle >= 0);//成り立たないときはクラッシュ
		return handle;
	}

	Position2 MyGetGraphSize(const int handle)
	{
		Position2 size = {};

		GetGraphSizeF(handle, &size.x, &size.y);

		return size;
	}

	int MyDrawGraph(int x, int y, int handle, bool trasnsFlg)
	{
		return DrawGraph(x, y,handle, trasnsFlg);
	}

	int MyDrawRectRotaGraph(int x, int y, int left, int top, int width, int height, float scale, float angle, int handle, bool transFlg)
	{
		return DrawRectRotaGraph(x, y, left, top, width, height, scale, angle, handle, transFlg);
	}

}//end of namespace my