#pragma once

namespace Game
{
#ifdef _DEBUG
	//�E�B���h�E���[�h�ݒ�
	constexpr bool kWindowMode = true;
#else
	constexpr bool kWindowMode = false;
#endif
	//�E�B���h�E��
	const wchar_t* const kTitleText = L"���b�N�}��";
	//�E�B���h�E�T�C�Y
	constexpr int kScreenWidth = 1600;//16:
	constexpr int kScreenHeight = 900;//9
	//�J���[���[�h
	constexpr int kColorDepth = 32;	//32 or 16
	//�}�b�v�`�b�v�T�C�Y
	constexpr int ChipSize = 32;
	constexpr float kScale = 2.0f;

	//��ʂɂ�����}�b�v�`�b�v�̐�
	static constexpr int kNumX = kScreenWidth / ChipSize;
	static constexpr int kNumY = kScreenHeight / ChipSize;

	//��ʂɕ\���ł���}�b�v�̑傫��
	static constexpr int kMapNumX = 16;
	static constexpr int kMapNumY = 16;
	//�}�b�v��\�������ʃT�C�Y
	static constexpr int kMapScreenX = (kScreenWidth - kMapNumX * ChipSize)/2;
	static constexpr int kMapScreenY = (kScreenHeight - kMapNumY * ChipSize)/2;

	//�}�b�v�̑傫��
	static constexpr int kMapChipNumX = 96;//219;
	static constexpr int kMapChipNumY = 92;//135;
};