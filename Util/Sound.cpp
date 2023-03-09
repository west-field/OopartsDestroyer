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
		L"Sound/gameclear.mp3",	//ゲームクリア音
		L"Sound/gameover.mp3",	//ゲームオーバー音
		
		L"Sound/Cursor.mp3",		//カーソル
		L"Sound/pushbotan.mp3",		//決定ボタン
		L"Sound/blockMove.mp3",		//ブロック接触音
		L"Sound/menuOpen.mp3",		//メニューを開く時の音
		L"Sound/playerJump.wav",	//プレイヤージャンプ音
		L"Sound/enemyJump.wav",		//敵ジャンプ音


		L"Sound/playerShot.wav",	//プレイヤー弾発射音
		L"Sound/playerShotHit.wav",	//プレイヤーに攻撃が当たった
		L"Sound/enemyShot.wav",		//敵弾発射音
		L"Sound/burst.wav",	//敵に攻撃が当たった
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
		SetVolume(id, volume);
		PlaySoundMem(m_soundHandle[id], DX_PLAYTYPE_LOOP, true);//ループ再生させる
	}
	void StopBgm(SoundId id)
	{
		StopSoundMem(m_soundHandle[id]);
	}
	//効果音の再生
	void Play(SoundId id, int volume)
	{
		SetVolume(id, volume);
		PlaySoundMem(m_soundHandle[id], DX_PLAYTYPE_BACK, true);
	}
	//音量設定
	void SetVolume(SoundId id, int volume)
	{
		ChangeVolumeSoundMem(volume, m_soundHandle[id]);
	}
}