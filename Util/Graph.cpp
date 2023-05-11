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

	m_bgImgs[static_cast<int>(BgGraph::BgGraph_1)].scrollSpeed = 0.5f;//ç≈âú
	m_bgImgs[static_cast<int>(BgGraph::BgGraph_2)].scrollSpeed = 0.6f;
	m_bgImgs[static_cast<int>(BgGraph::BgGraph_3)].scrollSpeed = 0.75f;
	m_bgImgs[static_cast<int>(BgGraph::BgGraph_4)].scrollSpeed = 0.8f;
	m_bgImgs[static_cast<int>(BgGraph::BgGraph_5)].scrollSpeed = 1.0f;//éËëO
}

Background::~Background()
{
	for (auto& bg : m_bgImgs)
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
	m_bgImgs[bgId].handle = handle;
	GetGraphSize(m_bgImgs[bgId].handle, &m_bgImgs[bgId].imgSize.w, &m_bgImgs[bgId].imgSize.h);
	
	return handle;
}

void Background::Init()
{
	m_scrollx = 0;
}

void Background::Update()
{
	m_scrollx = m_scrollx + 1;
}

void Background::Draw()
{
	for (auto& bg : m_bgImgs)
	{
		int bgWidth = static_cast<int>(bg.imgSize.w * kBgScale);
		int scroX = static_cast<int>(m_scrollx * bg.scrollSpeed) % bgWidth;
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
		int bgWidth = static_cast<int>(m_bgImgs[i].imgSize.w * kBgScale);
		int scroX = static_cast<int>(m_scrollx * m_bgImgs[i].scrollSpeed) % bgWidth;
		DrawRotaGraph(static_cast<int>(bgWidth / 2 - scroX),
			static_cast<int>(m_bgImgs[i].imgSize.h / 2 * kBgScale),
			kBgScale,
			0.0f,
			m_bgImgs[i].handle, true, false, false);
		DrawRotaGraph(static_cast<int>(bgWidth * (1 + 0.5) - scroX),
			static_cast<int>(m_bgImgs[i].imgSize.h / 2 * kBgScale),
			kBgScale,
			0.0f,
			m_bgImgs[i].handle, true, false, false);
	}
}
