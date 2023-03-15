#pragma once
#include "Secne.h"
#include "../game.h"
#include <memory>
#include <array>
#include "../Util/Geometry.h"

class EnemyMoveLeft;
class Player;
class HpBar;
class ShotFactory;

class InputState;
/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene : public Scene
{
private:
	unsigned int m_color = 0x000000;
	//フェードインの時のUpdate関数
	void FadeInUpdat(const InputState& input);
	//通常状態の時のUpdate関数
	void NormalUpdat(const InputState& input);
	//フェードアウトの時のUpdate関数
	void FadeOutUpdat(const InputState& input);

	//Update用メンバ関数ポインタ
	void (TitleScene::* m_updateFunc)(const InputState& input);

	int m_titleH = -1;
	int m_gearH = -1;
	int m_gearIdx = 0;

	static constexpr int kMenuFontSize = 50;//文字のサイズ
	static constexpr int kOriginalPosX = Game::kScreenWidth / 3 + kMenuFontSize * 2;    //メニュー文字のx位置
	static constexpr int kOriginalPosY = Game::kScreenHeight / 2 + kMenuFontSize * 2;    //メニュー文字のy位置
	static constexpr int kMovedPosX = kOriginalPosX - kMenuFontSize;//メニュー文字の移動したx位置

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
		unsigned int color;//色
		const wchar_t* name;//名前
	};
	MenuElement SelectMenu[menuNum] = {
		{ kMovedPosX, kOriginalPosY + kMenuFontSize * menuGameStart,0xffa0aa,L"ゲームスタート"},
		{ kOriginalPosX, kOriginalPosY + kMenuFontSize * menuConfig,0xaaa0ff,L"せってい"},
		{ kOriginalPosX, kOriginalPosY + kMenuFontSize * menuGameEnd+5,0xaaa0ff,L"おわり"}
	};
	int m_selectNum = 0;//選択しているメニュー項目
	
	int m_scroll = 0;

	int m_frame;
	int m_fade;

	std::shared_ptr<Player>m_player;
	std::shared_ptr<HpBar>m_hp;
	std::shared_ptr<ShotFactory>m_shot;
	int m_enemyH = -1;
	std::shared_ptr<EnemyMoveLeft> m_enemy[3];
public:
	TitleScene(SceneManager& manager);
	virtual ~TitleScene();

	void Update(const InputState& input);
	virtual void Draw() override;
};
