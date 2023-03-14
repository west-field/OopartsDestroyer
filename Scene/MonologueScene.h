#pragma once
#include "Secne.h"
#include "../game.h"
#include <memory>
#include <array>
#include "../Util/Geometry.h"

class InputState;
/// <summary>
/// ���m���[�O�V�[��
/// </summary>
class MonologueScene : public Scene
{
private:
	int m_selectNum = 0;//�I�����Ă��郁�j���[����
	unsigned int m_color = 0x000000;
	//�t�F�[�h�C���̎���Update�֐�
	void FadeInUpdat(const InputState& input);
	//�ʏ��Ԃ̎���Update�֐�
	void NormalUpdat(const InputState& input);
	//�t�F�[�h�A�E�g�̎���Update�֐�
	void FadeOutUpdat(const InputState& input);

	void PlayerOnStage(const InputState& input);//��ʂɓ����Ă���
	void ButtonOn(const InputState& input);//��{�^���������܂�
	void PlayerMoveUpdate(const InputState& input);//�v���C���[�ړ�������

	//Update�p�����o�֐��|�C���^
	void (MonologueScene::* m_updateFunc)(const InputState& input);

	int m_monoH = -1;
	int m_monoX = 0;

	int m_scroll = 0;

	Rect m_player = {};
	int m_playerH = -1;
	bool m_isPlayer = false;
	int m_frame = 0;
	int m_idxX = 0;
	int m_idxY = 0;

	bool m_isShotFire = true;
	int m_shotH = -1;
	Position2 m_shotPos = {};

	struct ButtonImg
	{
		Position2 buttonPos;//�{�^���̈ʒu
		int buttonIdxX = 0;
		int buttonIdxY = 0;
		int buttonFram = 0;
	};
	enum ButtomType
	{
		Buttom_X,
		Buttom_A,
		Buttom_Max,
	};
	ButtonImg m_button[Buttom_Max];
	int m_buttonH = -1;
	bool m_isButtonShot = false;
	bool m_isButtonJump = false;

public:
	MonologueScene(SceneManager& manager);
	virtual ~MonologueScene();

	void Update(const InputState& input);
	virtual void Draw() override;
};
//x224,y160
