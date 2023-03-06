#pragma once

namespace Graph
{
	enum BgGraph
	{
		BgGraph_1,
		BgGraph_2,
		BgGraph_3,
		BgGraph_4,
		BgGraph_5,
		BgGraph_Max
	};

	void Load();
	void Init();
	void Unload();

	void BgUpdate();
	void BgDraw(int scrollX);
	void Bg();
}
