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

	void SetBlock();

	//Update用メンバ関数ポインタ
	void (MonologueScene::* m_updateFunc)(const InputState& input);

	int m_monoH = -1;
	int m_monoX = 0;

	int m_scroll = 0;

	std::shared_ptr<Player>m_player;
	std::shared_ptr<HpBar>m_hp;
	std::shared_ptr<ShotFactory>m_shot;
public:
	MonologueScene(SceneManager& manager);
	virtual ~MonologueScene();

	void Update(const InputState& input);
	virtual void Draw() override;
};
//x224,y160
