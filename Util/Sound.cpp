#include "Sound.h"
#include <DxLib.h>
#include <cassert>
#include <string>

namespace
{
	constexpr char kSoundConfigFilePath[] = "Data/sound.conf";
}

SoundManager::SoundManager()
{
	LoadSoundConfig();

	LoadSoundFile(SoundId::Gameclear,L"gameclear.mp3");
	LoadSoundFile(SoundId::Gameover,L"gameover.mp3");
	
	LoadSoundFile(SoundId::Cursor,L"Cursor.mp3");
	LoadSoundFile(SoundId::Determinant,L"pushbotan.mp3");
	LoadSoundFile(SoundId::BlockMove,L"blockMove.mp3");
	LoadSoundFile(SoundId::MenuOpen,L"menuOpen.mp3");
	LoadSoundFile(SoundId::PlayerJump,L"playerJump.wav");
	LoadSoundFile(SoundId::EnemyJump,L"enemyJump.wav");
	LoadSoundFile(SoundId::Recovery,L"recovery.mp3");
	
	LoadSoundFile(SoundId::PlayeyShot,L"playerShot.wav");
	LoadSoundFile(SoundId::PlayeyHit,L"playerShotHit.wav");
	LoadSoundFile(SoundId::EnemyShot,L"enemyShot.wav");
	LoadSoundFile(SoundId::EnemyHit,L"playerShotHit.wav");
	LoadSoundFile(SoundId::EnemyBurst,L"burst.wav");
}
SoundManager::~SoundManager()
{
}

//�T�E���h�n���h�����擾
int SoundManager::LoadSoundFile(SoundId id, const wchar_t* fileName)
{
	std::wstring path = L"Data/Sound/SE/";
	path += fileName;
	int handle = LoadSoundMem(path.c_str());

	assert(handle >= 0);
	m_nameAndHandleTable[id] = handle;

	return handle;
}

//�T�E���h���̃��[�h
void SoundManager::LoadSoundConfig()
{
	SoundConfigInfo conf = {};
	FILE* fp = nullptr;
	fopen_s(&fp, kSoundConfigFilePath, "rb");
	if (fp)
	{
		fread(&conf, sizeof(conf), 1, fp);
		fclose(fp);
		m_volumeBGM = conf.volumeBGM;
		m_volumeSE = conf.volumeSE;
	}
}

//�T�E���h���̃Z�[�u
void SoundManager::SaveSoundConfig()
{
	SoundConfigInfo conf = {};
	conf.volumeSE = m_volumeSE;
	conf.volumeBGM = m_volumeBGM;

	FILE* fp = nullptr;
	fopen_s(&fp, kSoundConfigFilePath, "wb");
	if (fp)
	{
		fwrite(&conf, sizeof(conf), 1, fp);
		fclose(fp);
	}
}

//�w��̃T�E���h��炷
void SoundManager::Play(SoundId id, int volume)
{
	PlaySoundMem(m_nameAndHandleTable[id], DX_PLAYTYPE_BACK);
}

//BGM��炷
void SoundManager::PlayBGM(int soundH)
{
	PlaySoundMem(soundH, DX_PLAYTYPE_LOOP);
	ChangeVolumeSoundMem(m_volumeBGM,soundH);
}

//SE�̃{�����[����ݒ肷��
void SoundManager::SetSEVolume(int volume)
{
	for (auto& record : m_nameAndHandleTable)
	{
		ChangeVolumeSoundMem(volume, record.second);
	}
	m_volumeSE = volume;
}

//SE�̃{�����[�����擾����
int SoundManager::GetSEVolume() const
{
	return m_volumeSE;
}

//BGM�̃{�����[����ݒ肷��
void SoundManager::SetBGMVolume(int volume,int soundH)
{
	ChangeVolumeSoundMem(volume,soundH);
	m_volumeBGM = volume;
}

//BGM�̃{�����[�����擾����
int SoundManager::GetBGMVolume() const
{
	return m_volumeBGM;
}

//�T�E���h���~�߂�
void SoundManager::StopBgm(SoundId id)
{
	StopSoundMem(m_nameAndHandleTable[id]);
}
