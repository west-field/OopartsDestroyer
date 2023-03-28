#include <DxLib.h>
#include "game.h"
#include "Util/Sound.h"
#include "Util/Graph.h"
#include "InputState.h"
#include "Scene/SceneManager.h"
#include "Scene/TitleScene.h"

//int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	// window���[�h�ݒ�
	ChangeWindowMode(Game::kWindowMode);
#if true
	// window���[�h��ύX���邽��
	SetChangeScreenModeGraphicsSystemResetFlag(false);
#endif
	// �E�C���h�E���ݒ�
	SetMainWindowText(Game::kTitleText);
	// ��ʃT�C�Y�̐ݒ�
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorDepth);

	if (DxLib_Init() == -1)
	{
		return -1;
	}
	auto& soundManager = SoundManager::GetInstance();
	auto& background = Background::GetInstance();

	//�t�H���g��ǂݍ���
	LPCWSTR font_path = L"Font/PixelMplus10-Regular.ttf"; //�@�ǂݍ��ރt�H���g�t�@�C���̃p�X
	if (AddFontResourceEx(font_path, FR_PRIVATE, NULL) > 0) 
	{
	}
	else
	{
		//�@�t�H���g�Ǎ��G���[����
		MessageBox(NULL, L"�t�H���g�Ǎ����s", L"", MB_OK);
	}
	ChangeFont(L"PixelMplus10", DX_CHARSET_DEFAULT);

	SetDrawScreen(DX_SCREEN_BACK);

	InputState input;
	SceneManager sceneManeger;
	sceneManeger.ChangeScene(new TitleScene(sceneManeger));
#if true
	bool isTriggerWindouMode = false;//ALT��F�������Ă��邩
	bool isWindouwMode = Game::kWindowMode;//�E�B���h�E���[�h��ύX����
#endif
	while (ProcessMessage() != -1)
	{
		LONGLONG  time = GetNowHiPerformanceCount();
		// ��ʂ̃N���A
		ClearDrawScreen();

		input.Update();
		sceneManeger.Update(input);
		sceneManeger.Draw();
#if true
		if (CheckHitKey(KEY_INPUT_LALT))
		{
			if (CheckHitKey(KEY_INPUT_F))
			{
				if (!isTriggerWindouMode)
				{
					isWindouwMode = !isWindouwMode;
					ChangeWindowMode(isWindouwMode);
					SetDrawScreen(DX_SCREEN_BACK);//�`�����Ē�`
				}
				isTriggerWindouMode = true;
			}
			else
			{
				SetDrawScreen(DX_SCREEN_BACK);//�`�����Ē�`
				isTriggerWindouMode = false;
			}
		}
#endif
		//����ʂ�\��ʂ����ւ���
		ScreenFlip();

		// esc�L�[����������I������
		if (CheckHitKey(KEY_INPUT_ESCAPE))	break;
		//isEnd��true�̂Ƃ��I������
		if (sceneManeger.GetIsEnd())	break;
		// fps��60�ɌŒ�
		while (GetNowHiPerformanceCount() - time < 16667){}
	}

	//�@���\�[�X�̉��
	if (RemoveFontResourceEx(font_path, FR_PRIVATE, NULL)) {
	}
	else {
		MessageBox(NULL, L"remove failure", L"", MB_OK);
	}
	DxLib_End();

	return 0;
}