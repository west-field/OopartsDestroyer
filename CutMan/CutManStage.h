#pragma once
#include "../Scene/Secne.h"
#include "../game.h"
#include <memory>
#include <array>

class Player;
class Map;
class HpBar;
class Camera;

enum MapChip
{
	no,			//�Ȃ�
	sky,		//��
	terrain,	//�n��
	wall1,		//��1
	wall2,		//��2
	ladder1,	//��q1
	ladder2,	//��q2
	stone,		//��
	thorn,		//�Ƃ�

	max
};

class CutManStage : public Scene
{
public:
	CutManStage(SceneManager& manager);
	virtual ~CutManStage();

	virtual void Update(const InputState& input);
	virtual void Draw();
private:
	//�L�����N�^�[�ƃ}�b�v�̓����蔻��
	int MoveCharactor(float MoveX, float MoveY);
	//�}�b�v�Ƃ̓����蔻��
	int MapHitCheck(float X, float Y, float& MoveX, float& MoveY);

	virtual void FadeInUpdat(const InputState& input);
	virtual void NormalUpdat(const InputState& input);
	virtual void FadeOutUpdat(const InputState& input);

	unsigned int m_fadeColor = 0x000000;//�t�F�[�h�̐F�i��
	//UpdateFunc�̌^
	using UpdateFunc_t = void (CutManStage::*)(const InputState& input);
	UpdateFunc_t m_updateFunc = nullptr;
private:
	static constexpr int ChipSize = 16;
	static constexpr int MapWidth = (Game::kScreenWidth/ ChipSize);
	static constexpr int MapHeight = (Game::kScreenHeight / ChipSize);

private:
	//�v���C���[
	std::shared_ptr<Player> m_player;
	float	m_fallPlayerSpeed;	// �v���C���[�̗������x

	std::shared_ptr<Map> m_mapData;//�}�b�v

	enum
	{
		Object_Player,
		Object_Enemy,

		Object_Max
	};

	std::array<std::shared_ptr<HpBar>, Object_Max> m_hp;//HP
	std::shared_ptr<Camera>m_camera;//�J�����ړ�
};

