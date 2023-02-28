#pragma once

//フォント関連処理
namespace Font
{
	enum FontId
	{
		NormalFont,

		Max
	};

	//ロードアンロード
	void Load();
	void Unload();

	//フォントを変える
	void FontChange(FontId id);
	//フォントサイズを変更する
	void ChangeFontSize(int size);
};

