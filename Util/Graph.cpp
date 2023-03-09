#include "Graph.h"
#include "Geometry.h"
#include "DrawFunctions.h"
#include <DxLib.h>
#include <string>
#include <cassert>

namespace
{
	constexpr float kBgScale = 3.0f;	
}

Background::Background()
{
	Load(BgGraph_1, L"1.png");
	Load(BgGraph_2, L"2.png");
	Load(BgGraph_3, L"3.png");
	Load(BgGraph_4, L"4.png");
	Load(BgGraph_5, L"5.png");

	bgImgs[BgGraph_1].scrollSpeed = 0.5f;//Å‰œ
	bgImgs[BgGraph_2].scrollSpeed = 0.6f;
	bgImgs[BgGraph_3].scrollSpeed = 0.75f;
	bgImgs[BgGraph_4].scrollSpeed = 0.8f;
	bgImgs[BgGraph_5].scrollSpeed = 1.0f;//Žè‘O
}

Background::~Background()
{
	for (auto& bg : bgImgs)
	{
		DeleteGraph(bg.handle);
	}
}

int Background::Load(BgGraph id, const wchar_t* fileName)
{
	std::wstring path = L"Data/bg/";
	path += fileName;
	int handle = my::MyLoadGraph(path.c_str());
	assert(handle >= 0);
	bgImgs[id].handle = handle;
	GetGraphSize(bgImgs[id].handle, &bgImgs[id].imgSize.w, &bgImgs[id].imgSize.h);
	
	return handle;
}

void Background::Init()
{
	scrollx = 0;
	scrolly = 0;
}

void Background::Update()
{
	scrollx = scrollx + 1;
}

void Background::Draw(int scrollX)
{
	if (scrollX != 0)
	{
		scrollx = scrollX;
	}

	for (auto& bg : bgImgs)
	{
		int bgWidth = static_cast<int>(bg.imgSize.w * kBgScale);
		int scroX = static_cast<int>(scrollx * bg.scrollSpeed) % bgWidth;
		DrawRotaGraph(static_cast<int>(bgWidth / 2 - scroX),
			static_cast<int>(bg.imgSize.h / 2 * kBgScale),
			kBgScale,
			0.0f,
			bg.handle, true, false, false);
		DrawRotaGraph(static_cast<int>(bgWidth * (1 + 0.5) - scroX),
			static_cast<int>(bg.imgSize.h / 2 * kBgScale),
			kBgScale,
			0.0f,
			bg.handle, true, false, false);
	}
}

void Background::Bg()
{
	for (int i = 0; i <= 1; i++)
	{
		int bgWidth = static_cast<int>(bgImgs[i].imgSize.w * kBgScale);
		int scroX = static_cast<int>(scrollx * bgImgs[i].scrollSpeed) % bgWidth;
		DrawRotaGraph(static_cast<int>(bgWidth / 2 - scroX),
			static_cast<int>(bgImgs[i].imgSize.h / 2 * kBgScale),
			kBgScale,
			0.0f,
			bgImgs[i].handle, true, false, false);
		DrawRotaGraph(static_cast<int>(bgWidth * (1 + 0.5) - scroX),
			static_cast<int>(bgImgs[i].imgSize.h / 2 * kBgScale),
			kBgScale,
			0.0f,
			bgImgs[i].handle, true, false, false);
	}
}
