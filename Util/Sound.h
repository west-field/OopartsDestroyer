#pragma once

//�T�E���h�֘A����
namespace Sound
{
	enum SoundId
	{
		//BGM
		BgmTitle,		//�^�C�g��BGM
		BgmMain,		//���C��BGM
		Gameclear,		//�Q�[���N���A��
		GameclearBgm,	//�Q�[���N���ABGM
		Gameover,		//�Q�[���I�[�o�[��
		GameoverBgm,	//�Q�[���I�[�o�[BGM
		BgmBoss,		//�{�X��BGM

		//���ʉ�
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

		Max
	};

	//���[�h�A�����[�h
	void Load();
	void Unload();
	//BGM�̍Đ�
	void StartBgm(SoundId id,int volume = 255);
	void StopBgm(SoundId id);
	//���ʉ��̍Đ�
	void Play(SoundId id, int volume = 255);
	//���ʐݒ�
	void SetVolume(SoundId id, int volume);
};

