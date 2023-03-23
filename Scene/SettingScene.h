#pragma once
#include "Secne.h"
#include "../game.h"

class InputState;
class SceneManager;
/// <summary>
/// �|�[�Y�V�[��
/// </summary>
class SettingScene : public Scene
{
public:
	SettingScene(SceneManager& manager, int soundH);
	virtual ~SettingScene();

	virtual void Update(const InputState& input) override;
	void Draw();
private:
	enum Item
	{
		pauseKeyconfig,//�L�[�ݒ�
		pauseSound,//���ʐݒ�
		pauseBack,//�߂�

		pauseMax
	};

	struct Element
	{
		int x, y;//���W
		int color;//�F
		const wchar_t* name;//���O
	};
	static constexpr int pw_width = 400;
	static constexpr int pw_height = 300;
	static constexpr int pw_start_x = (Game::kScreenWidth - pw_width) / 2;
	static constexpr int pw_start_y = (Game::kScreenHeight - pw_height) / 2;

	static constexpr int kPosX = pw_start_x + 10;
	static constexpr int kPosY = pw_start_y + 30;

	Element m_pauseMenu[pauseMax] = {
		{kPosX		,kPosY + 20 * (pauseKeyconfig+1),0xaaffaa,L"�L�[�����Ă�"},
		{kPosX + 10	,kPosY + 20 * (pauseSound +2),0xffffff,L"���ʂ����Ă�"},
		{kPosX + 10	,kPosY + 20 * (pauseBack + 3),0xffffff, L"�߂�"},
	};
	int m_selectNum = 0;

	int m_posX = kPosX;
	int m_posY = kPosY;

	int m_soundH;
};

