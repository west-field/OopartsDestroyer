#include "ShotChangeScene.h"
#include "../Util/InputState.h"
#include "SceneManager.h"

ShotChangeScene::ShotChangeScene(SceneManager& manager) : Scene(manager), m_updateFunc(&ShotChangeScene::FadeInUpdate)
{
}

ShotChangeScene::~ShotChangeScene()
{
}

void ShotChangeScene::Update(const InputState& input)
{
}

void ShotChangeScene::Draw()
{
}

void ShotChangeScene::FadeInUpdate(const InputState& input)
{
	m_fadeValue = 255 * static_cast<int>(m_fadeTimer) / static_cast<int>(kFadeInterval);
	if (--m_fadeTimer == 0)
	{
		m_updateFunc = &ShotChangeScene::NormalUpdate;
		m_fadeValue = 0;
	}
}

void ShotChangeScene::NormalUpdate(const InputState& input)
{
	if (input.IsTriggered(InputType::next))
	{
		m_updateFunc = &ShotChangeScene::FadeOutUpdate;
	}
}

void ShotChangeScene::FadeOutUpdate(const InputState& input)
{
	m_fadeValue = 255 * static_cast<int>(m_fadeTimer) / static_cast<int>(kFadeInterval);
	if (++m_fadeTimer == kFadeInterval)
	{
		m_manager.PopScene();
		return;
	}
}
