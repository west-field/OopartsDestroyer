#include "Font.h"
#include <DxLib.h>

namespace
{
	LPCWSTR font_path = L"Font/PixelMplus10-Regular.ttf"; //�@�ǂݍ��ރt�H���g�t�@�C���̃p�X;
	//LPCWSTR font_path = L"Font/NewDokabenFont_.otf"; //�@�ǂݍ��ރt�H���g�t�@�C���̃p�X;
}

void Font::Load()
{
	//�@���\�[�X�̓ǂݍ���
	if (AddFontResourceEx(font_path, FR_PRIVATE, NULL) > 0) {
	}
	else {
		//�@�t�H���g�Ǎ��G���[����
		MessageBox(NULL, L"�t�H���g�Ǎ����s", L"", MB_OK);
	}
}

void Font::Unload()
{
	//�@���\�[�X�̉��
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
