#pragma once
#include "Secne.h"
#include "../game.h"

enum ItemConfig
{
	configOperate,
	configKeyconfig,
	configBack,

	configMax
};

struct ElementConfig
{
	int x, y;
	int color;
};

namespace
{
	constexpr int kconfigWidth = 500;
	constexpr int kconfigHeight = 400;
	constexpr int kconfigStartX = (Game::kScreenWidth - kconfigWidth) / 2;
	constexpr int kconfigStartY = (Game::kScreenHeight - kconfigHeight) / 2;

	constexpr int kFontSize = 50;

	constexpr int kPosX = kconfigStartX + 10;
	constexpr int kPosY = kconfigStartY + kFontSize + 10;
	
}

class InputState;
class SceneManager;
/// <summary>
/// オプションシーン
/// </summary>
class OptionScene : public Scene
{
public:
	//◇同時に継承元のコンストラクタが呼ばれる.Scene(SceneManager& manager)
	OptionScene(SceneManager& manager);
	~OptionScene();

	void Update(const InputState& input);
	void Draw();

	ElementConfig m_configMenu[configMax] = {
		{kconfigStartX + 30,kconfigStartY + kFontSize+10 + 20 * configOperate,0xffffff},
		{kconfigStartX + 30,kconfigStartY + kFontSize+10 + 20 * configKeyconfig,0xffffff},
		{kconfigStartX + 30,kconfigStartY + kFontSize+10 + 20 * configBack,0xffffff},
	};
	int m_selectNum = 0;
	int m_posX = kPosX;
	int m_posY = kPosY;
private:
	//フェードインの時のUpdate関数
	void FadeInUpdat(const InputState& input);
	//通常状態の時のUpdate関数
	void NormalUpdat(const InputState& input);
	//フェードアウトの時のUpdate関数
	void FadeOutUpdat(const InputState& input);

	//Update用メンバ関数ポインタ
	void (OptionScene::* m_updateFunc)(const InputState& input);
};

