#pragma once

enum SoundId
{
	Gameclear,		//ゲームクリア音
	Gameover,		//ゲームオーバー音

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
	PlayeyHit,	//プレイヤーが攻撃を受けた

	EnemyShot,		//敵の弾発射音
	EnemyHit,	//敵が攻撃を受けたときの音

	SoundId_Max
};

#include <unordered_map>
#include <string>

class SoundManager
{
private:
	SoundManager();
	~SoundManager();
	//コピーも代入も禁止する
	SoundManager(const SoundManager&) = delete;
	void operator= (const SoundManager&) = delete;

	int LoadSoundFile(SoundId id,const wchar_t* fileName);
	
	std::unordered_map<SoundId, int> nameAndHandleTable_;
public:
	/// <summary>
	/// SoundManager使用者はGetInstance()を通した参照からしか利用できない
	/// </summary>
	/// <returns>実体の参照を返す</returns>
	static SoundManager& GetInstance()
	{
		static SoundManager instance;//唯一の実体
		return instance;//それの参照を返す
	}

	/// <summary>
	/// 指定のサウンドを鳴らす
	/// </summary>
	/// <param name="name">サウンド名</param>
	void Play(SoundId id, int volume = 255);

	/// <summary>
	/// サウンドの音量を変える
	/// </summary>
	/// <param name="volume">音量</param>
	/// <param name="name">サウンド名</param>
	void SetVolume(int volume, SoundId id);

	/// <summary>
	/// サウンドを止める
	/// </summary>
	/// <param name="id">サウンド名</param>
	void StopBgm(SoundId id);
};


