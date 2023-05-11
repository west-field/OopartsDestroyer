#include "KeyConfigScene.h"
#include <DxLib.h>
#include "../game.h"
#include "../InputState.h"
#include "../Util/Sound.h"
#include "SceneManager.h"

KeyConfigScene::KeyConfigScene(SceneManager& manager, const InputState& input) :
	m_inputState(input), Scene(manager), m_currentInputIndex(0)
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
		const int nameCount = static_cast<int>(input.m_inputNameTable.size() + 2);

		if (input.IsTriggered(InputType::up))
		{
			SoundManager::GetInstance().Play(SoundId::Cursor);
			m_currentInputIndex = ((m_currentInputIndex - 1) + nameCount) % nameCount;
		}
		else if (input.IsTriggered(InputType::down))
		{
			SoundManager::GetInstance().Play(SoundId::Cursor);
			m_currentInputIndex = (m_currentInputIndex + 1) % nameCount;
		}
	}

	//この時もう、「決定」を選択している
	if (m_currentInputIndex == input.m_inputNameTable.size())
	{
#ifdef _DEBUG
		m_numSize = static_cast<int>(input.m_inputMapTable.size());
#endif
		if (input.IsTriggered(InputType::next))
		{
			SoundManager::GetInstance().Play(SoundId::Determinant);
			configInput.CommitChangedInputInfo();
			m_manager.PopScene();
			return;
		}
	}
	if (m_currentInputIndex == input.m_inputMapTable.size() - 1)
	{
#ifdef _DEBUG
		m_numSize = static_cast<int>(input.m_inputMapTable.size() - 1);
#endif
		if (input.IsTriggered(InputType::next))
		{
			SoundManager::GetInstance().Play(SoundId::Determinant);
			configInput.ResetInputInfo();
			return;
		}
	}
	//nextボタンでエディット中かそうじゃないかを決定する
	if (input.IsTriggered(InputType::next))
	{
		SoundManager::GetInstance().Play(SoundId::Determinant);
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
	int pw_height = 450;
	int pw_start_x = (640 - pw_width) / 2 + 50;
	int pw_start_y = (480 - pw_height) / 2 + 50;

	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0x008800, true);
	DrawString(pw_start_x + 10, pw_start_y + 10, L"キー設定...", 0xffffaa);

	auto y = pw_start_y + 40;
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
			color = 0xffa0aa;
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
		auto it = m_inputState.m_tempMapTable.find(type);

		x += 64;
		DrawString(x, y, L":", color);
		x += 10;
		for (auto& elem : it->second)
		{
			if (elem.cat == InputCategory::keybd)
			{
				//DrawFormatString(x, y, color, L"key=%d", elem.id);
				DrawFormatString(x, y, color, KeyName( elem.id).c_str());
			}
			else if (elem.cat == InputCategory::pad)
			{
				//DrawFormatString(x, y, color, L"pad=%d", elem.id);
				DrawFormatString(x, y, color, PadName(elem.id).c_str());
			}
			else if (elem.cat == InputCategory::mouse)
			{
				//DrawFormatString(x, y, color, L"mouse=%d", elem.id);
				DrawFormatString(x, y, color, MouseName(elem.id).c_str());
			}
			x += 100;
		}

		y += 50;
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
	DrawString(pw_start_x + 110, y, L"決定(戻る)", 0xffffff);

	y += 20;
	DrawString(pw_start_x + 110, y, L"キーリセット", 0xffffff);

	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0xffffff, false);

#ifdef _DEBUG
	DrawFormatString(0, 0, 0x000000, L"位置%d", m_currentInputIndex);
	DrawFormatString(0, 20, 0x000000, L"    %d", m_numSize);
#endif
}

std::wstring KeyConfigScene::PadName(int id)
{
	std::wstring path = L"pad=";

	switch (id)
	{
	case static_cast<int>(PAD_INPUT_1):
		path += L"A";
		break;
	case static_cast<int>(PAD_INPUT_2):
		path += L"B";
		break;
	case static_cast<int>(PAD_INPUT_3):
		path += L"X";
		break;
	case static_cast<int>(PAD_INPUT_4):
		path += L"Y";
		break;
	case static_cast<int>(PAD_INPUT_LEFT):
		path += L"←";
		break;
	case static_cast<int>(PAD_INPUT_RIGHT):
		path += L"→";
		break;
	case static_cast<int>(PAD_INPUT_UP):
		path += L"↑";
		break;
	case static_cast<int>(PAD_INPUT_DOWN):
		path += L"↓";
		break;
	case static_cast<int>(PAD_INPUT_L):
		path += L"BACK";
		break;
	case static_cast<int>(PAD_INPUT_R):
		path += L"START";
		break;

	default:
		path += id;
		break;
	}

	return path;
}

std::wstring KeyConfigScene::KeyName(int id)
{
	std::wstring path = L"key=";

	switch (id)
	{
	case static_cast<int>(KEY_INPUT_A):
		path += L"A";
		break;// Ａキー
	case static_cast<int>(KEY_INPUT_B):
		path += L"B";
		break;// Ｂキー
	case static_cast<int>(KEY_INPUT_C):
		path += L"C";
		break;	// Ｃキー
	case static_cast<int>(KEY_INPUT_D):
		path += L"D";
		break;// Ｄキー
	case static_cast<int>(KEY_INPUT_E):
		path += L"E";
		break;// Ｅキー
	case static_cast<int>(KEY_INPUT_F):
		path += L"F";
		break;// Ｆキー
	case static_cast<int>(KEY_INPUT_G):
		path += L"G";
		break;// Ｇキー
	case static_cast<int>(KEY_INPUT_H):
		path += L"H";
		break;// Ｈキー
	case static_cast<int>(KEY_INPUT_I):
		path += L"I";
		break;// Ｉキー
	case static_cast<int>(KEY_INPUT_J):
		path += L"J";
		break;// Ｊキー
	case static_cast<int>(KEY_INPUT_K):
		path += L"K";
		break;// Ｋキー
	case static_cast<int>(KEY_INPUT_L):
		path += L"L";
		break;// Ｌキー
	case static_cast<int>(KEY_INPUT_M):
		path += L"M";
		break;// Ｍキー
	case static_cast<int>(KEY_INPUT_N):
		path += L"N";
		break;// Ｎキー
	case static_cast<int>(KEY_INPUT_O):
		path += L"O";
		break;// Ｏキー
	case static_cast<int>(KEY_INPUT_P):
		path += L"P";
		break;	// Ｐキー
	case static_cast<int>(KEY_INPUT_Q):
		path += L"Q";
		break;// Ｑキー
	case static_cast<int>(KEY_INPUT_R):
		path += L"R";
		break;// Ｒキー
	case static_cast<int>(KEY_INPUT_S):
		path += L"S";
		break;	// Ｓキー
	case static_cast<int>(KEY_INPUT_T):
		path += L"T";
		break;// Ｔキー
	case static_cast<int>(KEY_INPUT_U):
		path += L"U";
		break;// Ｕキー
	case static_cast<int>(KEY_INPUT_V):
		path += L"V";
		break;// Ｖキー
	case static_cast<int>(KEY_INPUT_W):
		path += L"W";
		break;// Ｗキー
	case static_cast<int>(KEY_INPUT_X):	
		path += L"X";
		break;// Ｘキー
	case static_cast<int>(KEY_INPUT_Y):
		path += L"Y";
		break;// Ｙキー
	case static_cast<int>(KEY_INPUT_Z):	
		path += L"Z";
		break;// Ｚキー
	case static_cast<int>(KEY_INPUT_ESCAPE):
		path += L"ESC";
		break;
	case static_cast<int>(KEY_INPUT_LEFT):
		path += L"←";
		break;
	case static_cast<int>(KEY_INPUT_RIGHT):
		path += L"→";
		break;
	case static_cast<int>(KEY_INPUT_UP):
		path += L"↑";
		break;
	case static_cast<int>(KEY_INPUT_DOWN):
		path += L"↓";
		break;
	case static_cast<int>(KEY_INPUT_SPACE):
		path += L"SPACE";
		break;
	case static_cast<int>(KEY_INPUT_RETURN):
		path += L"ENTER";
		break;

	default:
		path += id;
		break;
	}

	return path;
}

std::wstring KeyConfigScene::MouseName(int id)
{
	std::wstring path = L"mouse=";

	switch (id)
	{
	case static_cast<int>(MOUSE_INPUT_LEFT):
		path += L"左クリック";
		break;
	case static_cast<int>(MOUSE_INPUT_RIGHT):
		path += L"右クリック";
		break;
	default:
		path += id;
		break;
	}

	return path;
}


