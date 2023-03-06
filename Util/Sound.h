#pragma once

//サウンド関連処理
namespace Sound
{
	enum SoundId
	{
		//BGM
		BgmTitle,		//タイトルBGM
		BgmMain,		//メインBGM
		Gameclear,		//ゲームクリア音
		GameclearBgm,	//ゲームクリアBGM
		Gameover,		//ゲームオーバー音
		GameoverBgm,	//ゲームオーバーBGM
		BgmBoss,		//ボス戦BGM

		//効果音
		Cursor,			//カーソル移動音
		Determinant,	//決定ボタン
		BlockMove,		//ブロック接触音
		MenuOpen,		//メニューを開く時の音
		PlayerJump,		//プレイヤージャンプ音
		EnemyJump,		//エネミージャンプ音
		//ItemGet,		//アイテムゲット音 itemGet.wav
		//Recovery,		//回復音

		//攻撃音
		PlayeyShot,		//プレイヤーの弾発射音
		PlayeyShotHit,	//プレイヤーの弾が敵にヒット

		EnemyShot,		//敵の弾発射音
		EnemyShotHit,	//弾のプレイヤーが敵にヒット

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

