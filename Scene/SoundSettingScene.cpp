#include "SoundSettingScene.h"
#include <Dxlib.h>
#include "../InputState.h"
#include "../Util/Sound.h"
#include "SceneManager.h"

SoundSettingScene::SoundSettingScene(SceneManager& manager,int soundH) :Scene(manager), m_soundH(soundH), m_updateFunc(&SoundSettingScene::NormalUpdate)
{
}

SoundSettingScene::~SoundSettingScene()
{
}

void SoundSettingScene::Update(const InputState& input)
{
	(this->*m_updateFunc)(input);
}

void SoundSettingScene::Draw()
{
	//サウンドセッティングウィンドウ
	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0x884444, true);
	//サウンドセッティング中メッセージ
	DrawString(pw_start_x + 10, pw_start_y + 10, L"SoundSetting...", 0xffffaa);
	//BGMの設定
	auto& soundMgr = SoundManager::GetInstance();
	DrawFormatString(m_soundChange[soundTypeBGM].x, m_soundChange[soundTypeBGM].y, m_soundChange[soundTypeBGM].color,
		m_soundChange[soundTypeBGM].name.c_str(), soundMgr.GetBGMVolume());
	//SEの設定
	DrawFormatString(m_soundChange[soundTypeSE].x, m_soundChange[soundTypeSE].y, m_soundChange[soundTypeSE].color,
		m_soundChange[soundTypeSE].name.c_str(), soundMgr.GetSEVolume());

	DrawFormatString(m_soundChange[soundTypeBack].x, m_soundChange[soundTypeBack].y, m_soundChange[soundTypeBack].color,
		m_soundChange[soundTypeBack].name.c_str());
	//サウンドセッティングウィンドウ枠線
	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0xffffff, false);
}

void SoundSettingScene::NormalUpdate(const InputState& input)
{
	bool isPress = false;
	if (input.IsTriggered(InputType::down))
	{
		m_selectNum = (m_selectNum + 1) % soundTypeMax;
		isPress = true;
	}
	else if (input.IsTriggered(InputType::up))
	{
		m_selectNum = (m_selectNum + (soundTypeMax - 1)) % soundTypeMax;
		isPress = true;
	}

	if (isPress)
	{
		SoundManager::GetInstance().Play(SoundId::Cursor);
		for (int i = 0; i < soundTypeMax; i++)
		{
			if (i == m_selectNum)
			{
				m_soundChange[i].x = pw_start_x + 10;
				m_soundChange[i].color = 0xaaffaa;
			}
			else
			{
				m_soundChange[i].x = pw_start_x + 20;
				m_soundChange[i].color = 0xffffff;
			}
		}
	}

	if (input.IsTriggered(InputType::next))
	{
		SoundManager::GetInstance().Play(SoundId::Determinant);
		switch (m_selectNum)
		{
		case soundTypeBGM:
			m_soundChange[soundTypeBGM].color = 0xffa000;
			m_updateFunc = &SoundSettingScene::BGMVolumeChange;
			return;
		case soundTypeSE:
			m_soundChange[soundTypeSE].color = 0xffa000;
			m_updateFunc = &SoundSettingScene::SEVolumeChange;
			return;
		case soundTypeBack:
			m_manager.PopScene();
			return;
		default:
			break;
		}
	}
	if (input.IsTriggered(InputType::prev))
	{
		SoundManager::GetInstance().Play(SoundId::Determinant);
		m_selectNum = soundTypeBack;
		m_manager.PopScene();
		return;
	}
}

void SoundSettingScene::BGMVolumeChange(const InputState& input)
{
	auto& soundMgr = SoundManager::GetInstance();
	int soundVolume = 0;
	//BGM
	if (input.IsPressed(InputType::up))
	{
		if (input.IsTriggered(InputType::up))
		{
			m_puressTime = 0;
			m_waitInterval = 60;
		}
		if (m_puressTime % m_waitInterval == 0)
		{
			soundVolume = soundMgr.GetBGMVolume() + 1;
			if (soundVolume >= 255)
			{
				soundVolume = 255;
			}
			soundMgr.SetBGMVolume(soundVolume, m_soundH);
		}
		if (m_puressTime % 10 == 0)
		{
			if (m_waitInterval-- <= 3)
			{
				m_waitInterval = 3;
			}
		}
		m_puressTime++;
	}
	if (input.IsPressed(InputType::down))
	{
		if (input.IsTriggered(InputType::down))
		{
			m_puressTime = 0;
			m_waitInterval = 60;
		}
		if (m_puressTime % m_waitInterval == 0)
		{
			soundVolume = soundMgr.GetBGMVolume() - 1;
			if (soundVolume <= 0)
			{
				soundVolume = 0;
			}
			soundMgr.SetBGMVolume(soundVolume, m_soundH);
		}
		if (m_puressTime % 10 == 0)
		{
			if (m_waitInterval-- <= 3)
			{
				m_waitInterval = 3;
			}
		}
		m_puressTime++;
	}
	if (input.IsTriggered(InputType::next) || input.IsTriggered(InputType::prev))
	{
		SoundManager::GetInstance().Play(SoundId::Determinant);
		m_soundChange[soundTypeBGM].color = 0xaaffaa;
		m_updateFunc = &SoundSettingScene::NormalUpdate;
		return;
	}
}

void SoundSettingScene::SEVolumeChange(const InputState& input)
{
	auto& soundMgr = SoundManager::GetInstance();
	int soundVolume = 0;

	//SE
	if (input.IsPressed(InputType::up))
	{
		if (input.IsTriggered(InputType::up))
		{
			m_puressTime = 0;
			m_waitInterval = 60;
		}
		if (m_puressTime % m_waitInterval == 0)
		{
			soundVolume = soundMgr.GetSEVolume() + 1;
			if (soundVolume >= 255)
			{
				soundVolume = 255;
			}
			SoundManager::GetInstance().Play(SoundId::Cursor);
			soundMgr.SetSEVolume(soundVolume);
		}
		if (m_puressTime % 10 == 0)
		{
			if (m_waitInterval-- <= 3)
			{
				m_waitInterval = 3;
			}
		}
		m_puressTime++;
	}
	if (input.IsPressed(InputType::down))
	{
		if (input.IsTriggered(InputType::down))
		{
			m_puressTime = 0;
			m_waitInterval = 60;
		}
		if (m_puressTime % m_waitInterval == 0)
		{
			soundVolume = soundMgr.GetSEVolume() - 1;
			if (soundVolume <= 0)
			{
				soundVolume = 0;
			}
			SoundManager::GetInstance().Play(SoundId::Cursor);
			soundMgr.SetSEVolume(soundVolume);
		}
		if (m_puressTime % 10 == 0)
		{
			if (m_waitInterval-- <= 3)
			{
				m_waitInterval = 3;
			}
		}
		m_puressTime++;
	}
	if (input.IsTriggered(InputType::next) || input.IsTriggered(InputType::prev))
	{
		SoundManager::GetInstance().Play(SoundId::Determinant);
		m_soundChange[soundTypeSE].color = 0xaaffaa;
		m_updateFunc = &SoundSettingScene::NormalUpdate;
		return;
	}
}

