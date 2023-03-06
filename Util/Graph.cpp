#include "Graph.h"
#include "Geometry.h"
#include "DrawFunctions.h"
#include <DxLib.h>
#include <array>

namespace
{
	const TCHAR* const kFileBgName[Graph::BgGraph_Max] =
	{
		L"Data/bg/1.png",
		L"Data/bg/2.png",
		L"Data/bg/3.png",
		L"Data/bg/4.png",
		L"Data/bg/5.png",
	};

	struct BackImg
	{
		int handle;//画像のハンドル
		float scrollSpeed;//スクロールスピード
		Size imgSize;
	};

	std::array<BackImg, Graph::BgGraph_Max> bgImgs;
	int scrollx = 0;
	int scrolly = 0;

	constexpr float kBgScale = 3.0f;
}

namespace Graph
{
	void Load()
	{
		for (int i = 0;i < BgGraph_Max;i++)
		{
			bgImgs[i].handle = my::MyLoadGraph(kFileBgName[i]);
			GetGraphSize(bgImgs[i].handle, &bgImgs[i].imgSize.w, &bgImgs[i].imgSize.h);
		}

		bgImgs[BgGraph_1].scrollSpeed = 0.5f;//最奥
		bgImgs[BgGraph_2].scrollSpeed = 0.6f;
		bgImgs[BgGraph_3].scrollSpeed = 0.75f;
		bgImgs[BgGraph_4].scrollSpeed = 0.8f;
		bgImgs[BgGraph_5].scrollSpeed = 1.0f;//手前
	}
	void Init()
	{
		scrollx = 0;
		scrolly = 0;
	}
	void Unload()
	{
		for (auto& bg : bgImgs)
		{
			DeleteGraph(bg.handle);
		}
	}
	void BgUpdate()
	{
		scrollx = scrollx + 1;
	}
	void BgDraw(int scrollX)
	{
		if (scrollX != 0)
		{
			scrollx = scrollX;
		}

		for (auto& bg : bgImgs)
		{
			int bgWidth = static_cast<int>(bg.imgSize.w * kBgScale);
			int scroX = static_cast<int>(scrollx * bg.scrollSpeed) % bgWidth;
			DrawRotaGraph(bgWidth / 2 - scroX,
				bg.imgSize.h / 2 * kBgScale,
				kBgScale,
				0.0f,
				bg.handle, true, false, false);
			DrawRotaGraph(bgWidth * (1 + 0.5) - scroX,
				bg.imgSize.h / 2 * kBgScale,
				kBgScale,
				0.0f,
				bg.handle, true, false, false);
		}
	}
	void Bg()
	{
		for (int i = 0; i <= 1; i++)
		{
			int bgWidth = static_cast<int>(bgImgs[i].imgSize.w * kBgScale);
			int scroX = static_cast<int>(scrollx * bgImgs[i].scrollSpeed) % bgWidth;
			DrawRotaGraph(bgWidth / 2 - scroX,
				bgImgs[i].imgSize.h / 2 * kBgScale,
				kBgScale,
				0.0f,
				bgImgs[i].handle, true, false, false);
			DrawRotaGraph(bgWidth * (1 + 0.5) - scroX,
				bgImgs[i].imgSize.h / 2 * kBgScale,
				kBgScale,
				0.0f,
				bgImgs[i].handle, true, false, false);
		}
	}
}