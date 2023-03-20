#pragma once
#include "Secne.h"
#include "../game.h"

class InputState;
class SceneManager;
/// <summary>
/// ポーズシーン
/// </summary>
class PauseScene : public Scene
{
private:

public:
	PauseScene(SceneManager& manager, int soundH);
	virtual ~PauseScene();

	virtual void Update(const InputState& input) override;
	void Draw();

	enum Item
	{
		pauseKeyconfig,//キー設定
		pauseSound,//音量設定
		pauseBack,//戻る
		pauseTitle,//タイトルに戻る

		pauseMax
	};

	struct Element
	{
		int x, y;//座標
		int color;//色
		const wchar_t* name;//名前
	};
	static constexpr int pw_width = 400;
	static constexpr int pw_height = 300;
	static constexpr int pw_start_x = (Game::kScreenWidth - pw_width) / 2;
	static constexpr int pw_start_y = (Game::kScreenHeight - pw_height) / 2;

	static constexpr int kPosX = pw_start_x + 10;
	static constexpr int kPosY = pw_start_y + 30;

	Element m_pauseMenu[pauseMax] = {
		{kPosX		,kPosY + 20 * (pauseKeyconfig+1),0xaaffaa,L"キーせってい"},
		{kPosX + 10	,kPosY + 20 * (pauseSound +2),0xffffff,L"音量せってい"},
		{kPosX + 10	,kPosY + 20 * (pauseBack + 3),0xffffff, L"戻る"},
		{kPosX + 10	,kPosY + 20 * (pauseTitle + 4),0xffffff, L"タイトル戻る"},
	};
	int m_selectNum = 0;

	int m_posX = kPosX;
	int m_posY = kPosY;

	int m_soundH;
};

