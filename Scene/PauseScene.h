#pragma once
#include "Secne.h"
#include "../game.h"

enum Item
{
	pauseKeyconfig,//キー設定
	pauseBack,//戻る

	pauseMax
};

struct Element
{
	int x, y;
	int color;
};

namespace
{
	constexpr int pw_width = 400;
	constexpr int pw_height = 300;
	constexpr int pw_start_x = (Game::kScreenWidth - pw_width) / 2;
	constexpr int pw_start_y = (Game::kScreenHeight - pw_height) / 2;

	constexpr int kPosX = pw_start_x + 10;
	constexpr int kPosY = pw_start_y + 30;
}

class InputState;
class SceneManager;
/// <summary>
/// ポーズシーン
/// </summary>
class PauseScene : public Scene
{
private:

public:
	PauseScene(SceneManager& manager);
	virtual ~PauseScene();

	virtual void Update(const InputState& input) override;
	void Draw();

	Element m_pauseMenu[pauseMax] = {
		{kPosX		,kPosY + 20 * (pauseKeyconfig+1),0xaaffaa},
		{kPosX + 10	,kPosY + 20 * (pauseBack+2),0xffffff},
	};
	int m_selectNum = 0;

	int m_posX = kPosX;
	int m_posY = kPosY;
};

