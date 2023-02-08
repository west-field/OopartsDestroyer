#pragma once
#include "Secne.h"
#include "../game.h"

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
}

class InputState;
class SceneManager;
/// <summary>
/// 操作方法
/// </summary>
class OperateScene : public Scene
{
public:
	OperateScene(SceneManager& manager);
	virtual ~OperateScene();

	virtual void Update(const InputState& input) override;
	void Draw();
private:
	//フェードインの時のUpdate関数
	void Page1Draw(const InputState& input);
	//通常状態の時のUpdate関数
	void Page2Draw(const InputState& input);
	//フェードアウトの時のUpdate関数
	void Page3Draw(const InputState& input);

	//Draw用メンバ関数ポインタ
	void (OperateScene::* m_updateFunc)(const InputState& input);
};

