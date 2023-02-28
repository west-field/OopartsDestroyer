#pragma once
#include "Secne.h"
#include "../game.h"
#include <array>
#include "../Util/Geometry.h"

//���j���[����
enum MenuItem
{
	menuGameStart,	//�Q�[���X�^�[�g
	menuConfig,		//�ݒ�
	menuGameEnd,	//�Q�[���I��

	menuNum			//���ڂ̐�
};
//���j���[�v�f
struct MenuElement
{
	int x;
	int y;				//���W
};

namespace
{
	constexpr int kTitleFontSize = 60;//�^�C�g���̃T�C�Y
	constexpr int kMenuFontSize = 50;//�����̃T�C�Y
	constexpr int kOriginalPosX = Game::kScreenWidth / 3 + kMenuFontSize;    //���j���[������x�ʒu
	constexpr int kOriginalPosY = Game::kScreenHeight / 2 + kMenuFontSize;    //���j���[������y�ʒu
	constexpr int kMovedPosX = kOriginalPosX - kMenuFontSize;//���j���[�����̈ړ�����x�ʒu
}

class InputState;
/// <summary>
/// �^�C�g���V�[��
/// </summary>
class TitleScene : public Scene
{
private:
	int m_selectNum = 0;//�I�����Ă��郁�j���[����

	//�t�F�[�h�C���̎���Update�֐�
	void FadeInUpdat(const InputState& input);
	//�ʏ��Ԃ̎���Update�֐�
	void NormalUpdat(const InputState& input);
	//�t�F�[�h�A�E�g�̎���Update�֐�
	void FadeOutUpdat(const InputState& input);

	//Update�p�����o�֐��|�C���^
	void (TitleScene::* m_updateFunc)(const InputState& input);

	MenuElement SelectMenu[menuNum] = {
		{ kMovedPosX, kOriginalPosY + kMenuFontSize * menuGameStart },
		{ kOriginalPosX, kOriginalPosY + kMenuFontSize * menuConfig},
		{ kOriginalPosX, kOriginalPosY + kMenuFontSize * menuGameEnd}
	};

	struct BackImg
	{
		int handle;//�摜�̃n���h��
		float scrollSpeed;//�X�N���[���X�s�[�h
		Size imgSize;
	};
	//�w�i
	static constexpr int bgNum = 5;
	std::array<BackImg, bgNum> m_bgImgs;
	int m_scroll = 0;
	float bg_scale;

public:
	TitleScene(SceneManager& manager);
	virtual ~TitleScene();

	void Update(const InputState& input);
	virtual void Draw() override;
};

