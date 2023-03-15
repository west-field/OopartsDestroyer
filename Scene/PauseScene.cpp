#include "PauseScene.h"
#include <DxLib.h>
#include "../game.h"
#include "../Util/InputState.h"
#include "../Util/Sound.h"
#include "SceneManager.h"
#include "KeyConfigScene.h"

PauseScene::PauseScene(SceneManager& manager) : Scene(manager)
{

}

PauseScene::~PauseScene()
{

}

void PauseScene::Update(const InputState& input)
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

void PauseScene::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_MULA, 196);
	//ポーズウィンドウセロファン
	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//ポーズ中メッセージ
	DrawString(pw_start_x + 10, pw_start_y + 10, L"ポーズ", 0xffff88);
	DrawString(m_pauseMenu[pauseKeyconfig].x, m_pauseMenu[pauseKeyconfig].y, L"キーせってい", m_pauseMenu[pauseKeyconfig].color);
	DrawString(m_pauseMenu[pauseBack].x, m_pauseMenu[pauseBack].y, L"戻る", m_pauseMenu[pauseBack].color);
	//ポーズウィンドウ枠線
	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0xffffff, false);
}
