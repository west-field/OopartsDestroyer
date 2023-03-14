#pragma once
#include "Secne.h"
#include "../game.h"
#include <memory>
#include <array>
#include "../Util/Geometry.h"

class InputState;
/// <summary>
/// モノローグシーン
/// </summary>
class MonologueScene : public Scene
{
private:
	int m_selectNum = 0;//選択しているメニュー項目
	unsigned int m_color = 0x000000;
	//フェードインの時のUpdate関数
	void FadeInUpdat(const InputState& input);
	//通常状態の時のUpdate関数
	void NormalUpdat(const InputState& input);
	//フェードアウトの時のUpdate関数
	void FadeOutUpdat(const InputState& input);

	void PlayerOnStage(const InputState& input);//画面に入ってくる
	void ButtonOn(const InputState& input);//二つボタンを押すまで
	void PlayerMoveUpdate(const InputState& input);//プレイヤー移動させる

	//Update用メンバ関数ポインタ
	void (MonologueScene::* m_updateFunc)(const InputState& input);

	int m_monoH = -1;
	int m_monoX = 0;

	int m_scroll = 0;

	Rect m_player = {};
	int m_playerH = -1;
	bool m_isPlayer = false;
	int m_frame = 0;
	int m_idxX = 0;
	int m_idxY = 0;

	bool m_isShotFire = true;
	int m_shotH = -1;
	Position2 m_shotPos = {};

	struct ButtonImg
	{
		Position2 buttonPos;//ボタンの位置
		int buttonIdxX = 0;
		int buttonIdxY = 0;
		int buttonFram = 0;
	};
	enum ButtomType
	{
		Buttom_X,
		Buttom_A,
		Buttom_Max,
	};
	ButtonImg m_button[Buttom_Max];
	int m_buttonH = -1;
	bool m_isButtonShot = false;
	bool m_isButtonJump = false;

public:
	MonologueScene(SceneManager& manager);
	virtual ~MonologueScene();

	void Update(const InputState& input);
	virtual void Draw() override;
};
//x224,y160
