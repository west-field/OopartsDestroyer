#include "Sound.h"
#include <DxLib.h>
#include <vector>

namespace
{
	//�T�E���h�n���h��
	std::vector<int> m_soundHandle;

	//�T�E���h�t�@�C����
	const TCHAR* const kFileName[Sound::Max] =
	{
		L"Sound/noranekonokuchibue.mp3",//�^�C�g��BGM
		L"Sound/Disital_Delta.mp3",	//���C��BGM
		L"Sound/gameclear.mp3",	//�Q�[���N���A��
		L"Sound/emerald.mp3",	//�Q�[���N���ABGM
		L"Sound/gameover.mp3",	//�Q�[���I�[�o�[��
		L"Sound/shizukanoumi.mp3",	//�Q�[���I�[�o�[BGM
		L"Sound/arabiantechno.mp3",	//�{�X��BGM
		
		L"Sound/Cursor.mp3",		//�J�[�\��
		L"Sound/pushbotan.mp3",		//����{�^��
		L"Sound/blockMove.mp3",		//�u���b�N�ڐG��
		L"Sound/menuOpen.mp3",		//���j���[���J�����̉�
		L"Sound/playerJump.wav",	//�v���C���[�W�����v��
		L"Sound/enemyJump.wav",		//�G�W�����v��


		L"Sound/playerShot.wav",	//�v���C���[�e���ˉ�
		L"Sound/playerShotHit.wav",	//�v���C���[�̒e���G�Ƀq�b�g
		L"Sound/enemyShot.wav",	//�G�e���ˉ�
		L"Sound/playerShotHit.wav",	//�v���C���[�̒e���G�Ƀq�b�g
	};
}

namespace Sound
{
	//���[�h�A�����[�h
	void Load()
	{
		// �T�E���h�f�[�^�̓ǂݍ���
		for (auto& fileName : kFileName)
		{
			int handle = LoadSoundMem(fileName);
			m_soundHandle.push_back(handle);
		}
	}
	void Unload()
	{
		// �T�E���h�f�[�^�̉��
		for (auto& handle : m_soundHandle)
		{
			DeleteSoundMem(handle);
			handle = -1;
		}
	}
	//BGM�̍Đ�
	void StartBgm(SoundId id, int volume)
	{
		PlaySoundMem(m_soundHandle[id], DX_PLAYTYPE_LOOP, true);//���[�v�Đ�������
		SetVolume(id, volume);
	}
	void StopBgm(SoundId id)
	{
		StopSoundMem(m_soundHandle[id]);
	}
	//���ʉ��̍Đ�
	void Play(SoundId id)
	{
		PlaySoundMem(m_soundHandle[id], DX_PLAYTYPE_BACK, true);
	}
	//���ʐݒ�
	void SetVolume(SoundId id, int volume)
	{
		ChangeVolumeSoundMem(volume, m_soundHandle[id]);
	}
}