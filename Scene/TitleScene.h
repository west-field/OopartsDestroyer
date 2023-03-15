#pragma once
#include "Secne.h"
#include "../game.h"
#include <memory>
#include <array>
#include "../Util/Geometry.h"

class EnemyMoveLeft;
class Player;
class HpBar;
class ShotFactory;

class InputState;
/// <summary>
/// �^�C�g���V�[��
/// </summary>
class TitleScene : public Scene
{
private:
	unsigned int m_color = 0x000000;
	//�t�F�[�h�C���̎���Update�֐�
	void FadeInUpdat(const InputState& input);
	//�ʏ��Ԃ̎���Update�֐�
	void NormalUpdat(const InputState& input);
	//�t�F�[�h�A�E�g�̎���Update�֐�
	void FadeOutUpdat(const InputState& input);

	//Update�p�����o�֐��|�C���^
	void (TitleScene::* m_updateFunc)(const InputState& input);

	int m_titleH = -1;
	int m_gearH = -1;
	int m_gearIdx = 0;

	static constexpr int kMenuFontSize = 50;//�����̃T�C�Y
	static constexpr int kOriginalPosX = Game::kScreenWidth / 3 + kMenuFontSize * 2;    //���j���[������x�ʒu
	static constexpr int kOriginalPosY = Game::kScreenHeight / 2 + kMenuFontSize * 2;    //���j���[������y�ʒu
	static constexpr int kMovedPosX = kOriginalPosX - kMenuFontSize;//���j���[�����̈ړ�����x�ʒu

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
		unsigned int color;//�F
		const wchar_t* name;//���O
	};
	MenuElement SelectMenu[menuNum] = {
		{ kMovedPosX, kOriginalPosY + kMenuFontSize * menuGameStart,0xffa0aa,L"�Q�[���X�^�[�g"},
		{ kOriginalPosX, kOriginalPosY + kMenuFontSize * menuConfig,0xaaa0ff,L"�����Ă�"},
		{ kOriginalPosX, kOriginalPosY + kMenuFontSize * menuGameEnd+5,0xaaa0ff,L"�����"}
	};
	int m_selectNum = 0;//�I�����Ă��郁�j���[����
	
	int m_scroll = 0;

	int m_frame;
	int m_fade;

	std::shared_ptr<Player>m_player;
	std::shared_ptr<HpBar>m_hp;
	std::shared_ptr<ShotFactory>m_shot;
	int m_enemyH = -1;
	std::shared_ptr<EnemyMoveLeft> m_enemy[3];
public:
	TitleScene(SceneManager& manager);
	virtual ~TitleScene();

	void Update(const InputState& input);
	virtual void Draw() override;
};
