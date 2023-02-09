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
	 constexpr int kNumX = kScreenWidth / ChipSize;
	 constexpr int kNumY = kScreenHeight / ChipSize;

	//��ʂɕ\���ł���}�b�v�̑傫��
	 constexpr int kMapNumX = 16;
	 constexpr int kMapNumY = 15;
	//�}�b�v��\�������ʍ�����W
	 constexpr int kMapScreenLeftX = (kScreenWidth - (kMapNumX-1) * ChipSize)/2;
	 constexpr int kMapScreenTopY = (kScreenHeight - kMapNumY * ChipSize)/2;
	//�}�b�v��\�������ʉE�����W
	 constexpr int kMapScreenRightX = kMapScreenLeftX + kMapNumX * ChipSize;
	 constexpr int kMapScreenBottomY = kMapScreenTopY + kMapNumY * ChipSize;

	//�}�b�v�̑傫��
	 constexpr int kMapChipNumX = 128+16+58+16;//219;
	 constexpr int kMapChipNumY = 135;//135;
};