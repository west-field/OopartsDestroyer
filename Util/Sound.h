#pragma once

//サウンド関連処理
namespace Sound
{
	enum SoundId
	{
		//BGM
		BgmMain,		//メインBGM
		BgmGameover,	//終わった時のBGM
		//効果音
		Cursor,			//カーソル移動音
		Determinant,	//決定ボタン
		Count,			//カウント音
		BlockMove,		//ブロック移動音
		BlockPlacing,	//ブロックを置く音
		Blocking,		//ブロックを壊した音

		Max
	};

	//ロードアンロード
	void Load();
	void Unload();
	//BGMの再生
	void StartBgm(SoundId id,int volume = 255);
	void StopBgm(SoundId id);
	//効果音の再生
	void Play(SoundId id);
	//音量設定
	void SetVolume(SoundId id, int volume);
};

