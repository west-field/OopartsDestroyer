#pragma once

//�t�H���g�֘A����
namespace Font
{
	enum FontId
	{
		NormalFont,

		Max
	};

	//���[�h�A�����[�h
	void Load();
	void Unload();

	//�t�H���g��ς���
	void FontChange(FontId id);
	//�t�H���g�T�C�Y��ύX����
	void ChangeFontSize(int size);
};

