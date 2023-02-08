#pragma once

namespace Game
{
	//�E�B���h�E���[�h�ݒ�
	constexpr bool kWindowMode = true;
	//�E�B���h�E��
	const wchar_t* const kTitleText = L"���b�N�}��";
	//�E�B���h�E�T�C�Y
	constexpr int kScreenWidth = 1600;//16
	constexpr int kScreenHeight = 900;//9
	//�J���[���[�h
	constexpr int kColorDepth = 32;	//32 or 16
	//�}�b�v�`�b�v�T�C�Y
	constexpr int ChipSize = 32;

	//��ʂɂ�����}�b�v�`�b�v�̐�
	static constexpr int kNumX = kScreenWidth / ChipSize;
	static constexpr int kNumY = kScreenHeight / ChipSize;
	//�}�b�v�̑傫��
	static constexpr int kMapChipNumX = kNumX + 46;// 219;// Game::kScreenWidth / Game::ChipSize * 2;
	static constexpr int kMapChipNumY = kNumY + 47;//135;//Game::kScreenHeight / Game::ChipSize;
};