#include "OptionScene.h"
#include <DxLib.h>
#include "../Util/InputState.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "KeyConfigScene.h"
#include "OperateScene.h"

OptionScene::OptionScene(SceneManager& manager) : Scene(manager), m_updateFunc(&OptionScene::FadeInUpdat)
{
}

OptionScene::~OptionScene()
{
}

void OptionScene::Update(const InputState& input)
{
	(this->*m_updateFunc)(input);
}
void OptionScene::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_MULA, 196);
	//ウィンドウセロファン
	DrawBox(kconfigStartX, kconfigStartY, kconfigStartX + kconfigWidth, kconfigStartY + kconfigHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//枠線
	DrawBox(kconfigStartX, kconfigStartY, kconfigStartX + kconfigWidth, kconfigStartY + kconfigHeight, 0xffffff, false);
	
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawTriangle(m_posX, m_posY, m_posX, m_posY + 20, m_posX + 10, m_posY + 10, 0xffffff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//メッセージ
	SetFontSize(kFontSize);
	DrawString(kconfigStartX+10, kconfigStartY+10, L"Option", 0xffff88);
	SetFontSize(0);
	DrawString(m_configMenu[configOperate].x, m_configMenu[configOperate].y, L"configOperate", m_configMenu[configOperate].color);
	DrawString(m_configMenu[configKeyconfig].x, m_configMenu[configKeyconfig].y, L"KeyConfig", m_configMenu[configKeyconfig].color);
	DrawString(m_configMenu[configBack].x, m_configMenu[configBack].y, L"戻る", m_configMenu[configBack].color);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeValue);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void OptionScene::FadeInUpdat(const InputState& input)
{
	m_fadeValue = 255 * static_cast<int>(m_fadeTimer) / static_cast<int>(kFadeInterval);
	if (--m_fadeTimer == 0)
	{
		m_updateFunc = &OptionScene::NormalUpdat;
		m_fadeValue = 0;
	}
}

void OptionScene::NormalUpdat(const InputState& input)
{
	bool isPress = false;
	if (input.IsTriggered(InputType::down))
	{
		m_selectNum = (m_selectNum + 1) % configMax;
		isPress = true;
	}
	else if (input.IsTriggered(InputType::up))
	{
		m_selectNum = (m_selectNum + (configMax - 1)) % configMax;
		isPress = true;
	}

	if (isPress)
	{
		for (int i = 0; i < configMax; i++)
		{
			if (i == m_selectNum)
			{
				m_posY = kPosY + 20 * i;
			}
		}
	}

	if (input.IsTriggered(InputType::next))
	{
		switch (m_selectNum)
		{
		case configOperate:
			m_manager.PushScene(new OperateScene(m_manager));
			return;
		case configKeyconfig:
			m_manager.PushScene(new KeyConfigScene(m_manager, input));
			return;
		case configBack:
			m_updateFunc = &OptionScene::FadeOutUpdat;
		}
	}
	if (input.IsTriggered(InputType::prev))
	{
		m_selectNum = configBack;
		m_updateFunc = &OptionScene::FadeOutUpdat;
	}
}

void OptionScene::FadeOutUpdat(const InputState& input)
{
	m_fadeValue = 255 * static_cast<int>(m_fadeTimer) / static_cast<int>(kFadeInterval);
	if (++m_fadeTimer == kFadeInterval)
	{
		m_manager.ChangeScene(new TitleScene(m_manager));
		return;
	}
}
