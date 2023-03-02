#include <DxLib.h>
#include "game.h"
#include "Util/Sound.h"
#include "Util/Font.h"
#include "Util/Graph.h"
#include "Util/InputState.h"
#include "Game/Button.h"
#include "Scene/SceneManager.h"
#include "Scene/TitleScene.h"

/*
�{�X��
�e�����킷
�t�B�[���h���}�b�v�`�b�v�ł���

*/

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// window���[�h�ݒ�
	ChangeWindowMode(Game::kWindowMode);
	// �E�C���h�E���ݒ�
	SetMainWindowText(Game::kTitleText);
	// ��ʃT�C�Y�̐ݒ�
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorDepth);

	if (DxLib_Init() == -1)
	{
		return -1;
	}
	Sound::Load();
	Button::Load();
	Font::Load();
	Font::FontChange(Font::NormalFont);

	SetDrawScreen(DX_SCREEN_BACK);

	InputState input;
	SceneManager sceneManeger;
	sceneManeger.ChangeScene(new TitleScene(sceneManeger));

	while (ProcessMessage() != -1)
	{
		LONGLONG  time = GetNowHiPerformanceCount();
		// ��ʂ̃N���A
		ClearDrawScreen();

		input.Update();
		sceneManeger.Update(input);
		sceneManeger.Draw();
		
		//����ʂ�\��ʂ����ւ���
		ScreenFlip();

		// esc�L�[����������I������
		if (CheckHitKey(KEY_INPUT_ESCAPE))	break;
		//isEnd��true�̂Ƃ��I������
		if (sceneManeger.GetIsEnd())	break;
		// fps��60�ɌŒ�
		while (GetNowHiPerformanceCount() - time < 16667){}
	}
	Sound::Unload();
	Button::Unload();
	Font::Unload();
	DxLib_End();

	return 0;
}