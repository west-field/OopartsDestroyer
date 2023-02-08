#pragma once
#include "Secne.h"
#include "../game.h"

enum Item
{
	pauseKeyconfig,
	pauseBack,
	pauseTitle,

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
		{pw_start_x + 20,pw_start_y + 30 + 20 * pauseKeyconfig,0xffffff},
		{pw_start_x + 10,pw_start_y + 30 + 20 * pauseBack,0xffffff},
		{pw_start_x + 10,pw_start_y + 30 + 20 * pauseTitle,0xffffff},
	};
	int m_selectNum = 0;

	int m_posX = kPosX;
	int m_posY = kPosY;
};

