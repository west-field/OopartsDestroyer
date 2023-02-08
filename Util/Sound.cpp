#include "Sound.h"
#include <DxLib.h>
#include <vector>

namespace
{
	//サウンドハンドル
	std::vector<int> m_soundHandle;

	//サウンドファイル名
	const TCHAR* const kFileName[Sound::Max] =
	{
		L"Sound/sekibaku.mp3",//メインBGM
		L"Sound/mistake.mp3",//終わった時のBGM

		L"Sound/paperRoll.mp3",//カーソル移動音
		L"Sound/decisionPressing.mp3",//決定ボタン
		L"Sound/count.mp3",//カウント音
		L"Sound/blockMove.mp3",//ブロック移動音
		L"Sound/blockPlace.mp3",//ブロックを置く音
		L"Sound/blockBreak.mp3"//ブロックを壊した音
	};
}

namespace Sound
{
	//ロードアンロード
	void Load()
	{
		// サウンドデータの読み込み
		for (auto& fileName : kFileName)
		{
			int handle = LoadSoundMem(fileName);
			m_soundHandle.push_back(handle);
		}
	}
	void Unload()
	{
		// サウンドデータの解放
		for (auto& handle : m_soundHandle)
		{
			DeleteSoundMem(handle);
			handle = -1;
		}
	}
	//BGMの再生
	void StartBgm(SoundId id, int volume)
	{
		PlaySoundMem(m_soundHandle[id], DX_PLAYTYPE_BACK, true);
		SetVolume(id, volume);
	}
	void StopBgm(SoundId id)
	{
		StopSoundMem(m_soundHandle[id]);
	}
	//効果音の再生
	void Play(SoundId id)
	{
		PlaySoundMem(m_soundHandle[id], DX_PLAYTYPE_BACK, true);
	}
	//音量設定
	void SetVolume(SoundId id, int volume)
	{
		ChangeVolumeSoundMem(volume, m_soundHandle[id]);
	}
}