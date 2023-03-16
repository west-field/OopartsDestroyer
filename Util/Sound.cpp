#include "Sound.h"
#include <DxLib.h>
#include <cassert>
#include <string>

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
	LoadSoundFile(Recovery,L"recovery.mp3");
	
	LoadSoundFile(PlayeyShot,L"playerShot.wav");
	LoadSoundFile(PlayeyHit,L"playerShotHit.wav");
	LoadSoundFile(EnemyShot,L"enemyShot.wav");
	LoadSoundFile(EnemyHit,L"playerShotHit.wav");
	LoadSoundFile(EnemyBurst,L"burst.wav");
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
	PlaySoundMem(nameAndHandleTable_[id], DX_PLAYTYPE_BACK);
}

void SoundManager::PlayBGM(int soundH)
{
	PlaySoundMem(soundH, DX_PLAYTYPE_LOOP);
	ChangeVolumeSoundMem(m_volumeBGM,soundH);
}

void SoundManager::SetSEVolume(int volume)
{
	for (auto& record : nameAndHandleTable_)
	{
		ChangeVolumeSoundMem(volume, record.second);
	}
	m_volumeSE = volume;
}

int SoundManager::GetSEVolume() const
{
	return m_volumeSE;
}

void SoundManager::SetBGMVolume(int volume,int soundH)
{
	ChangeVolumeSoundMem(volume,soundH);
	m_volumeBGM = volume;
}

int SoundManager::GetBGMVolume() const
{
	return m_volumeBGM;
}

void SoundManager::StopBgm(SoundId id)
{
	StopSoundMem(nameAndHandleTable_[id]);
}
