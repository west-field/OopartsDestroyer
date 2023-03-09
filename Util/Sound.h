#pragma once

enum SoundId
{
	Gameclear,		//�Q�[���N���A��
	Gameover,		//�Q�[���I�[�o�[��

	Cursor,			//�J�[�\���ړ���
	Determinant,	//����{�^��
	BlockMove,		//�u���b�N�ڐG��
	MenuOpen,		//���j���[���J�����̉�
	PlayerJump,		//�v���C���[�W�����v��
	EnemyJump,		//�G�l�~�[�W�����v��
	//ItemGet,		//�A�C�e���Q�b�g�� itemGet.wav
	//Recovery,		//�񕜉�

	//�U����
	PlayeyShot,		//�v���C���[�̒e���ˉ�
	PlayeyHit,	//�v���C���[���U�����󂯂�

	EnemyShot,		//�G�̒e���ˉ�
	EnemyHit,	//�G���U�����󂯂��Ƃ��̉�

	SoundId_Max
};

#include <unordered_map>
#include <string>

class SoundManager
{
private:
	SoundManager();
	~SoundManager();
	//�R�s�[��������֎~����
	SoundManager(const SoundManager&) = delete;
	void operator= (const SoundManager&) = delete;

	int LoadSoundFile(SoundId id,const wchar_t* fileName);
	
	std::unordered_map<SoundId, int> nameAndHandleTable_;
public:
	/// <summary>
	/// SoundManager�g�p�҂�GetInstance()��ʂ����Q�Ƃ��炵�����p�ł��Ȃ�
	/// </summary>
	/// <returns>���̂̎Q�Ƃ�Ԃ�</returns>
	static SoundManager& GetInstance()
	{
		static SoundManager instance;//�B��̎���
		return instance;//����̎Q�Ƃ�Ԃ�
	}

	/// <summary>
	/// �w��̃T�E���h��炷
	/// </summary>
	/// <param name="name">�T�E���h��</param>
	void Play(SoundId id, int volume = 255);

	/// <summary>
	/// �T�E���h�̉��ʂ�ς���
	/// </summary>
	/// <param name="volume">����</param>
	/// <param name="name">�T�E���h��</param>
	void SetVolume(int volume, SoundId id);

	/// <summary>
	/// �T�E���h���~�߂�
	/// </summary>
	/// <param name="id">�T�E���h��</param>
	void StopBgm(SoundId id);
};


