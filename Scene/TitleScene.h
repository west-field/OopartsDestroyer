#pragma once
#include "Secne.h"
#include "../game.h"
#include <array>
#include "../Util/Geometry.h"

//メニュー項目
enum MenuItem
{
	menuGameStart,	//ゲームスタート
	menuConfig,		//設定
	menuGameEnd,	//ゲーム終了

	menuNum			//項目の数
};
//メニュー要素
struct MenuElement
{
	int x;
	int y;				//座標
};

namespace
{
	constexpr int kTitleFontSize = 60;//タイトルのサイズ
	constexpr int kMenuFontSize = 50;//文字のサイズ
	constexpr int kOriginalPosX = Game::kScreenWidth / 3 + kMenuFontSize;    //メニュー文字のx位置
	constexpr int kOriginalPosY = Game::kScreenHeight / 2 + kMenuFontSize;    //メニュー文字のy位置
	constexpr int kMovedPosX = kOriginalPosX - kMenuFontSize;//メニュー文字の移動したx位置
}

class InputState;
/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene : public Scene
{
private:
	int m_selectNum = 0;//選択しているメニュー項目

	//フェードインの時のUpdate関数
	void FadeInUpdat(const InputState& input);
	//通常状態の時のUpdate関数
	void NormalUpdat(const InputState& input);
	//フェードアウトの時のUpdate関数
	void FadeOutUpdat(const InputState& input);

	//Update用メンバ関数ポインタ
	void (TitleScene::* m_updateFunc)(const InputState& input);

	MenuElement SelectMenu[menuNum] = {
		{ kMovedPosX, kOriginalPosY + kMenuFontSize * menuGameStart },
		{ kOriginalPosX, kOriginalPosY + kMenuFontSize * menuConfig},
		{ kOriginalPosX, kOriginalPosY + kMenuFontSize * menuGameEnd}
	};

	struct BackImg
	{
		int handle;//画像のハンドル
		float scrollSpeed;//スクロールスピード
		Size imgSize;
	};
	//背景
	static constexpr int bgNum = 5;
	std::array<BackImg, bgNum> m_bgImgs;
	int m_scroll = 0;
	float bg_scale;

public:
	TitleScene(SceneManager& manager);
	virtual ~TitleScene();

	void Update(const InputState& input);
	virtual void Draw() override;
};

