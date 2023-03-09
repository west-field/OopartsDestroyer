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
	const wchar_t* const kTitleText = L"�I�[�p�[�c";
	//�E�B���h�E�T�C�Y
	constexpr int kScreenWidth = 1600;//16:
	constexpr int kScreenHeight = 900;//9
	//�J���[���[�h
	constexpr int kColorDepth = 32;	//32 or 16
	//�}�b�v�`�b�v�T�C�Y
	constexpr int ChipSize = 32;
	constexpr float kScale = 1.5f;

	constexpr int kDrawSize = static_cast<int>(ChipSize * kScale);

	//��ʂɂ�����}�b�v�`�b�v�̐�
	 constexpr int kNumX = kScreenWidth / kDrawSize;
	 constexpr int kNumY = kScreenHeight / kDrawSize;

	//��ʂɕ\���ł���}�b�v�̑傫��
	 constexpr int kMapNumX = 16;
	 constexpr int kMapNumY = 15;
	//�}�b�v��\�������ʍ�����W
	 constexpr int kMapScreenLeftX = (kScreenWidth - (kMapNumX-1) * kDrawSize)/2;
	 constexpr int kMapScreenTopY = (kScreenHeight - kMapNumY * kDrawSize)/2;
	//�}�b�v��\�������ʉE�����W
	 constexpr int kMapScreenRightX = kMapScreenLeftX + kMapNumX * kDrawSize;
	 constexpr int kMapScreenBottomY = kMapScreenTopY + kMapNumY * kDrawSize;

	//�}�b�v�̑傫�� X218*Y135
	 constexpr int kMapChipNumX = 218;//219;
	 constexpr int kMapChipNumY = 135;//135;
};