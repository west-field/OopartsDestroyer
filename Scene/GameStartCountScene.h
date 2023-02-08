#pragma once
#include "Secne.h"

class InputState;

enum Level
{
	Easy,
	Normal,
	Hard,

	Max
};

struct LevelElement
{
	int x;
	int y;
};

namespace
{
	constexpr int kLevelFontSize = 50;
	constexpr int kStartMenuPosX = 200;
	constexpr int kMovePosX = kStartMenuPosX - kLevelFontSize;
	constexpr int kStartMenuPosY = 150;
}

/// <summary>
/// ゲーム開始シーン
/// </summary>
class GameStartCountScene : public Scene
{
public:
	GameStartCountScene(SceneManager& manager);
	~GameStartCountScene();

	virtual void Update(const InputState& input);
	void Draw();
private:
	//通常状態の時のUpdate関数
	void NormalUpdat(const InputState& input);
	//カウントダウンの時のUpdate関数
	void CountDownUpdat(const InputState& input);

	//Update用メンバ関数ポインタ
	void (GameStartCountScene::* m_updateFunc)(const InputState& input);

	//選択しているレベル
	int m_selectLevel = 0;

	LevelElement m_SelectLevel[Max] = {
		{kMovePosX,kStartMenuPosY},
		{kStartMenuPosX,kStartMenuPosY + Normal * kLevelFontSize},
		{kStartMenuPosX,kStartMenuPosY + Hard * kLevelFontSize}
	};

	float m_count = 240.0f;

	bool m_isTuru = false;
};

