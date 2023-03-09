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

	void SetBlock();

	//Update�p�����o�֐��|�C���^
	void (MonologueScene::* m_updateFunc)(const InputState& input);

	int m_monoH = -1;
	int m_monoX = 0;

	int m_scroll = 0;

	std::shared_ptr<Player>m_player;
	std::shared_ptr<HpBar>m_hp;
	std::shared_ptr<ShotFactory>m_shot;
public:
	MonologueScene(SceneManager& manager);
	virtual ~MonologueScene();

	void Update(const InputState& input);
	virtual void Draw() override;
};
//x224,y160
