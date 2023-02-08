#pragma once
#include "Secne.h"

class InputState;
class SceneManager;

class ShotChangeScene : public Scene
{
public:
	ShotChangeScene(SceneManager& manager);
	virtual ~ShotChangeScene();

	void Update(const InputState& input);
	void Draw();
private:
	void FadeInUpdate(const InputState& input);
	void NormalUpdate(const InputState& input);
	void FadeOutUpdate(const InputState& input);

	void (ShotChangeScene::* m_updateFunc)(const InputState& input);
};

