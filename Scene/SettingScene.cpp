#include "SettingScene.h"
#include <DxLib.h>
#include "../game.h"
#include "../InputState.h"
#include "../Util/Sound.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "KeyConfigScene.h"
#include "SoundSettingScene.h"

SettingScene::SettingScene(SceneManager& manager,int soundH) : Scene(manager), m_soundH(soundH)
{

}

SettingScene::~SettingScene()
{

}

void SettingScene::Update(const InputState& input)
{
	bool isPress = false;
	if (input.IsTriggered(InputType::down))
	{
		m_selectNum = (m_selectNum + 1) % pauseMax;
		isPress = true;
	}
	else if (input.IsTriggered(InputType::up))
	{
		m_selectNum = (m_selectNum + (pauseMax - 1)) % pauseMax;
		isPress = true;
	}

	if (isPress)
	{
		SoundManager::GetInstance().Play(SoundId::Cursor);
		for (int i = 0; i < pauseMax; i++)
		{
			if (i == m_selectNum)
			{
				m_pauseMenu[i].x = pw_start_x + 10;
				m_pauseMenu[i].color = 0xaaffaa;
			}
			else
			{
				m_pauseMenu[i].x = pw_start_x + 20;
				m_pauseMenu[i].color = 0xffffff;
			}
		}
	}

	if (input.IsTriggered(InputType::next))
	{
		SoundManager::GetInstance().Play(SoundId::Determinant);
		switch (m_selectNum)
		{
		case pauseKeyconfig:
			m_manager.PushScene(new KeyConfigScene(m_manager, input));
			return;
		case pauseSound:
			m_manager.PushScene(new SoundSettingScene(m_manager,m_soundH));
			return;
		case pauseBack:
			m_manager.PopScene();
			return;
		default:
			break;
		}
	}
	if (input.IsTriggered(InputType::prev))
	{
		SoundManager::GetInstance().Play(SoundId::Determinant);
		m_selectNum = pauseBack;
		m_manager.PopScene();
		return;
	}
}

void SettingScene::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_MULA, 196);
	//せっていウィンドウセロファン
	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//せってい中メッセージ
	DrawString(pw_start_x + 10, pw_start_y + 10, L"せってい", 0xffff88);
	for (auto& menu : m_pauseMenu)
	{
		DrawString(menu.x, menu.y, menu.name, menu.color);
	}
	//せっていウィンドウ枠線
	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0xffffff, false);
}
