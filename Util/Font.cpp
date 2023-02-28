#include "Font.h"
#include <DxLib.h>

namespace
{
	LPCWSTR font_path = L"Font/PixelMplus10-Regular.ttf"; //　読み込むフォントファイルのパス;
	//LPCWSTR font_path = L"Font/NewDokabenFont_.otf"; //　読み込むフォントファイルのパス;
}

void Font::Load()
{
	//　リソースの読み込み
	if (AddFontResourceEx(font_path, FR_PRIVATE, NULL) > 0) {
	}
	else {
		//　フォント読込エラー処理
		MessageBox(NULL, L"フォント読込失敗", L"", MB_OK);
	}
}

void Font::Unload()
{
	//　リソースの解放
	if (RemoveFontResourceEx(font_path, FR_PRIVATE, NULL)) {
	}
	else {
		MessageBox(NULL, L"remove failure", L"", MB_OK);
	}
}

void Font::FontChange(FontId id)
{
	switch (id)
	{
	case Font::NormalFont:
		ChangeFont(L"PixelMplus10", DX_CHARSET_DEFAULT);
		//ChangeFont(L"NewDokabenFont", DX_CHARSET_DEFAULT);
		break;
	default:
		break;
	}
}

void Font::ChangeFontSize(int size)
{
	SetFontSize(size);
}
