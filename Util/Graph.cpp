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
	Load(BgGraph::BgGraph_1, L"1.png");
	Load(BgGraph::BgGraph_2, L"2.png");
	Load(BgGraph::BgGraph_3, L"3.png");
	Load(BgGraph::BgGraph_4, L"4.png");
	Load(BgGraph::BgGraph_5, L"5.png");

	bgImgs[static_cast<int>(BgGraph::BgGraph_1)].scrollSpeed = 0.5f;//ç≈âú
	bgImgs[static_cast<int>(BgGraph::BgGraph_2)].scrollSpeed = 0.6f;
	bgImgs[static_cast<int>(BgGraph::BgGraph_3)].scrollSpeed = 0.75f;
	bgImgs[static_cast<int>(BgGraph::BgGraph_4)].scrollSpeed = 0.8f;
	bgImgs[static_cast<int>(BgGraph::BgGraph_5)].scrollSpeed = 1.0f;//éËëO
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
	int bgId = static_cast<int>(id);

	std::wstring path = L"Data/bg/";
	path += fileName;
	int handle = my::MyLoadGraph(path.c_str());
	assert(handle >= 0);
	bgImgs[bgId].handle = handle;
	GetGraphSize(bgImgs[bgId].handle, &bgImgs[bgId].imgSize.w, &bgImgs[bgId].imgSize.h);
	
	return handle;
}

void Background::Init()
{
	scrollx = 0;
}

void Background::Update()
{
	scrollx = scrollx + 1;
}

void Background::Draw()
{
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
