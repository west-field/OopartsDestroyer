#include "KeyConfigScene.h"
#include <DxLib.h>
#include "../game.h"
#include "../Util/InputState.h"
#include "SceneManager.h"

KeyConfigScene::KeyConfigScene(SceneManager& manager, const InputState& input) : 
	m_inputState(input),
	Scene(manager),
	m_currentInputIndex(0)
{
}

KeyConfigScene::~KeyConfigScene()
{
	m_inputState.SaveKeyInfo();
}

void KeyConfigScene::Update(const InputState& input)
{
	auto& configInput = const_cast<InputState&>(input);
	if (!m_isEditing)
	{
		if (input.IsTriggered(InputType::keyconf))
		{
			configInput.RollbackChangedInputInfo();
			m_manager.PopScene();
			return;
		}

		const int nameCount = input.m_inputNameTable.size() + 2;

		if (input.IsTriggered(InputType::up))
		{
			m_currentInputIndex = ((m_currentInputIndex - 1) + nameCount) % nameCount;
		}
		else if (input.IsTriggered(InputType::down))
		{
			m_currentInputIndex = (m_currentInputIndex + 1) % nameCount;
		}
	}

	//この時もう、「確定します」を選択している
	if (m_currentInputIndex == input.m_inputNameTable.size())
	{
		if (input.IsTriggered(InputType::next))
		{
			configInput.CommitChangedInputInfo();
			m_manager.PopScene();
			return;
		}
	}
	if (m_currentInputIndex == input.m_inputMapTable.size() + 1)
	{
		if (input.IsTriggered(InputType::next))
		{
			configInput.ResetInputInfo();
			return;
		}
	}
	//nextボタンでエディット中かそうじゃないかを決定する
	if (input.IsTriggered(InputType::next))
	{
		m_isEditing = !m_isEditing;
		return;
	}

	if (m_isEditing)
	{
		char keystate[256];
		GetHitKeyStateAll(keystate);
		auto padState = GetJoypadInputState(DX_INPUT_PAD1);
		auto mouseState = GetMouseInput();

		int idx = 0;
		InputType currentType = InputType::max;
		for (const auto& name : m_inputState.m_inputNameTable)
		{
			if (m_currentInputIndex == idx)
			{
				currentType = name.first;
				break;
			}
			++idx;
		}

		for (int i = 0; i < 256; i++)
		{
			if (keystate[i])
			{
				configInput.RewriteInputInfo(currentType, InputCategory::keybd, i);
				break;
			}
		}

		if (padState != 0)
		{
			configInput.RewriteInputInfo(currentType, InputCategory::pad, padState);
		}
		if (mouseState != 0)
		{
			configInput.RewriteInputInfo(currentType, InputCategory::mouse, mouseState);
		}
	}
}

void KeyConfigScene::Draw()
{
	int pw_width = 450;
	int pw_height = 350;
	int pw_start_x = (640 - pw_width) / 2 + 50;
	int pw_start_y = (480 - pw_height) / 2 + 50;

	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0x008800, true);
	DrawString(pw_start_x + 10, pw_start_y + 10, L"keyconfig...", 0xffffaa);

	auto y = pw_start_y + 30;
	int idx = 0;
	bool isInputTypeSelected = false;
	for (const auto& name : m_inputState.m_inputNameTable)
	{
		int offset = 0;
		unsigned int color = 0xffffff;
		if (m_currentInputIndex == idx)
		{
			isInputTypeSelected = true;
			offset = 10;
			if (m_isEditing)
			{
				color = 0xff0000;
			}
		}

		//各キーの表示
		if (m_currentInputIndex == idx)
		{
			offset = 10;
			if (m_isEditing)
			{
				color = 0xff0000;
			}
			DrawString(pw_start_x + 10, y, L"▶", 0xff0000);
		}
		int x = pw_start_x + 20 + offset;
		DrawString(x, y, name.second.c_str(), color);

		auto type = name.first;
		auto it = m_inputState.tempMapTable_.find(type);

		x += 64;
		DrawString(x, y, L":", color);
		x += 10;
		for (auto& elem : it->second)
		{
			if (elem.cat == InputCategory::keybd)
			{
				DrawFormatString(x, y, color, L"key=%d", elem.id);
			}
			else if (elem.cat == InputCategory::pad)
			{
				DrawFormatString(x, y, color, L"pad=%d", elem.id);
			}
			else if (elem.cat == InputCategory::mouse)
			{
				DrawFormatString(x, y, color, L"mouse=%d", elem.id);
			}
			x += 100;
		}

		y += 18;
		idx++;
	}
	y += 20;
	if (!isInputTypeSelected)
	{
		int yoffset = 0;
		if (m_currentInputIndex == m_inputState.m_inputNameTable.size() + 1)
		{
			yoffset = 20;
		}
		DrawString(pw_start_x + 90, y + yoffset, L"▶", 0xff0000);
	}
	//各キーの表示
	DrawString(pw_start_x + 100, y, L"確定します", 0xffffff);

	y += 20;
	DrawString(pw_start_x + 100, y, L"キーリセット", 0xffffff);

	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0xffffff, false);
}
