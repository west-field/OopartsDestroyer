#include "Sound.h"
#include <DxLib.h>
#include <cassert>

SoundManager::SoundManager()
{
	LoadSoundFile(Gameclear,L"gameclear.mp3");
	LoadSoundFile(Gameover,L"gameover.mp3");
	
	LoadSoundFile(Cursor,L"Cursor.mp3");
	LoadSoundFile(Determinant,L"pushbotan.mp3");
	LoadSoundFile(BlockMove,L"blockMove.mp3");
	LoadSoundFile(MenuOpen,L"menuOpen.mp3");
	LoadSoundFile(PlayerJump,L"playerJump.wav");
	LoadSoundFile(EnemyJump,L"enemyJump.wav");
	
	LoadSoundFile(PlayeyShot,L"playerShot.wav");
	LoadSoundFile(PlayeyHit,L"playerShotHit.wav");
	LoadSoundFile(EnemyShot,L"enemyShot.wav");
	LoadSoundFile(EnemyHit,L"burst.wav");
}
SoundManager::~SoundManager()
{
}

int SoundManager::LoadSoundFile(SoundId id, const wchar_t* fileName)
{
	std::wstring path = L"Sound/SE/";
	path += fileName;
	int handle = LoadSoundMem(path.c_str());

	assert(handle >= 0);
	nameAndHandleTable_[id] = handle;

	return handle;
}

void SoundManager::Play(SoundId id, int volume)
{
	SetVolume(volume, id);
	PlaySoundMem(nameAndHandleTable_[id], DX_PLAYTYPE_BACK);
}

void SoundManager::SetVolume(int volume, SoundId id)
{
	ChangeVolumeSoundMem(volume, nameAndHandleTable_[id]);
}

void SoundManager::StopBgm(SoundId id)
{
	StopSoundMem(nameAndHandleTable_[id]);
}
