#pragma once
#include <array>
#include "Geometry.h"

enum BgGraph
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
	Background();
	~Background();

	Background(const Background&) = delete;
	void operator= (const Background&) = delete;

	int Load(BgGraph id,const wchar_t* fileName);

	struct BackImg
	{
		int handle = -1;//�摜�̃n���h��
		float scrollSpeed;//�X�N���[���X�s�[�h
		Size imgSize;
	};
	std::array<BackImg, BgGraph_Max> bgImgs;

	int scrollx = 0;
	int scrolly = 0;
public:
	static Background& GetInstance()
	{
		static Background instance;
		return instance;
	}
	void Init();
	void Update();
	void Draw(int scrollX);
	void Bg();
};

