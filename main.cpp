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
	// windowモード設定
	ChangeWindowMode(Game::kWindowMode);
#if true
	// windowモードを変更するため
	SetChangeScreenModeGraphicsSystemResetFlag(false);
#endif
	// ウインドウ名設定
	SetMainWindowText(Game::kTitleText);
	// 画面サイズの設定
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorDepth);

	if (DxLib_Init() == -1)
	{
		return -1;
	}
	auto& soundManager = SoundManager::GetInstance();
	auto& background = Background::GetInstance();

	//フォントを読み込む
	LPCWSTR font_path = L"Font/PixelMplus10-Regular.ttf"; //　読み込むフォントファイルのパス
	if (AddFontResourceEx(font_path, FR_PRIVATE, NULL) > 0) 
	{
	}
	else
	{
		//　フォント読込エラー処理
		MessageBox(NULL, L"フォント読込失敗", L"", MB_OK);
	}
	ChangeFont(L"PixelMplus10", DX_CHARSET_DEFAULT);

	SetDrawScreen(DX_SCREEN_BACK);

	InputState input;
	SceneManager sceneManeger;
	sceneManeger.ChangeScene(new TitleScene(sceneManeger));
#if true
	bool isTriggerWindouMode = false;//ALTとFを押しているか
	bool isWindouwMode = Game::kWindowMode;//ウィンドウモードを変更する
#endif
	while (ProcessMessage() != -1)
	{
		LONGLONG  time = GetNowHiPerformanceCount();
		// 画面のクリア
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
					SetDrawScreen(DX_SCREEN_BACK);//描画先を再定義
				}
				isTriggerWindouMode = true;
			}
			else
			{
				SetDrawScreen(DX_SCREEN_BACK);//描画先を再定義
				isTriggerWindouMode = false;
			}
		}
#endif
		//裏画面を表画面を入れ替える
		ScreenFlip();

		// escキーを押したら終了する
		if (CheckHitKey(KEY_INPUT_ESCAPE))	break;
		//isEndがtrueのとき終了する
		if (sceneManeger.GetIsEnd())	break;
		// fpsを60に固定
		while (GetNowHiPerformanceCount() - time < 16667){}
	}

	//　リソースの解放
	if (RemoveFontResourceEx(font_path, FR_PRIVATE, NULL)) {
	}
	else {
		MessageBox(NULL, L"remove failure", L"", MB_OK);
	}
	DxLib_End();

	return 0;
}