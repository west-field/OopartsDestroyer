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
ボス戦
弾をかわす
フィールドをマップチップでつくる

*/

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// windowモード設定
	ChangeWindowMode(Game::kWindowMode);
	// ウインドウ名設定
	SetMainWindowText(Game::kTitleText);
	// 画面サイズの設定
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
		// 画面のクリア
		ClearDrawScreen();

		input.Update();
		sceneManeger.Update(input);
		sceneManeger.Draw();
		
		//裏画面を表画面を入れ替える
		ScreenFlip();

		// escキーを押したら終了する
		if (CheckHitKey(KEY_INPUT_ESCAPE))	break;
		//isEndがtrueのとき終了する
		if (sceneManeger.GetIsEnd())	break;
		// fpsを60に固定
		while (GetNowHiPerformanceCount() - time < 16667){}
	}
	Sound::Unload();
	Button::Unload();
	Font::Unload();
	DxLib_End();

	return 0;
}