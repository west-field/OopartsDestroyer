#include "CutManStage.h"
#include <DxLib.h>
#include "../InputState.h"
#include "../Scene/SceneManager.h"
#include "../Scene/GameoverScene.h"
#include "../Game/Player.h"
#include "../Game/HpBar.h"
#include "../Map.h"
#include "../DrawFunctions.h"
#include "../Camera.h"

namespace
{
	constexpr float kJump_Power = 8.0f;
	constexpr float kJumpAcc = 6.0f;//�W�����v��
	constexpr float kMoveSpeed = 2.0f;
}

CutManStage::CutManStage(SceneManager& manager):Scene(manager),m_updateFunc(&CutManStage::FadeInUpdat)
{
	// �v���C���[�̍��W��������
	//player.x = 320.0f;
	//player.y = 240.0f;
	//// �v���C���[�̗������x��������
	//player.fallSpeed = 0.0f;
	//// �W�����v���t���O��|��
	//player.jumpFlag = FALSE;

	m_player = std::make_shared<Player>();
	m_player->Init({}, {});
	m_player->SetMain(this);

	m_mapData = std::make_shared<Map>();
	m_mapData->Load(my::MyLoadGraph(L"Data/mapchip.bmp"));
	m_mapData->readData(L"Data/mapdata.bin");

	for (auto& hp : m_hp)
	{
		hp = std::make_shared<HpBar>();
	}
	m_hp[Object_Player]->Init(my::MyLoadGraph(L"Data/hp.bmp"));
	m_hp[Object_Enemy]->Init(my::MyLoadGraph(L"Data/hp.bmp"));

	m_camera = std::make_shared<Camera>();
}

CutManStage::~CutManStage()
{
	m_mapData->Unload();
}

void CutManStage::Update(const InputState& input)
{
	// �v���C���[�̈ړ�����
	float MoveX, MoveY;

	// �ړ��ʂ̏�����
	MoveX = 0.0f;
	MoveY = 0.0f;

	if (input.IsPressed(InputType::left))
	{
		m_mapData->ScrollLeft();
		MoveX -= kMoveSpeed;
	}
	if (input.IsPressed(InputType::right))
	{
		m_mapData->ScrollRight();
		MoveX += kMoveSpeed;
	}
	//�n�ɑ��������Ă���,�����ɒ�q������ꍇ�͏�L�[�Œ�q������
	if (m_player->IsJump() == false &&
		m_mapData->GetMapChipParam(m_player->GetPos().x, m_player->GetPos().y ) == ladder1 && 
		input.IsPressed(InputType::up))
	{
		m_fallPlayerSpeed = -kJumpAcc;
	}
	// �n�ɑ��������Ă���ꍇ�̂݃W�����v�{�^��(�{�^���P or �y�L�[)������
	if (m_player->IsJump() == false && input.IsPressed(InputType::junp))
	{
		m_fallPlayerSpeed = -kJumpAcc;
		m_player->SetJump(true);
		m_mapData->ScrollJump();
	}

	// ��������
	m_fallPlayerSpeed += 0.3f;

	// �������x���ړ��ʂɉ�����
	MoveY = m_fallPlayerSpeed;

	// �ړ��ʂɊ�Â��ăL�����N�^�̍��W���ړ�
	MoveCharactor(MoveX, MoveY);

	m_player->Update(input);
	m_mapData->Update();
	m_camera->UpdateCamera(m_player->GetPos());
}

void CutManStage::Draw()
{
	m_mapData->Draw();
	
	m_player->Draw();

	m_hp[Object_Player]->Draw(true);
	m_hp[Object_Enemy]->Draw(false);
}

int CutManStage::MoveCharactor(float MoveX, float MoveY)
{
	float Dummy = 0.0f;
	float hsize, wsize;

	// �L�����N�^�̍���A�E��A�����A�E�������������蔻��̂���
	// �}�b�v�ɏՓ˂��Ă��邩���ׁA�Փ˂��Ă�����␳����

	// �����̃T�C�Y���Z�o
	wsize = m_player->GetColSize().w * 0.5f;
	hsize = m_player->GetColSize().h * 0.5f;

	// �����̃`�F�b�N�A�����u���b�N�̏�ӂɒ����Ă����痎�����~�߂�
	if (MapHitCheck(m_player->GetPos().x - wsize, m_player->GetPos().y + hsize, Dummy, MoveY) == 3)
	{
		m_fallPlayerSpeed = 0.0f;
	}

	// �E���̃`�F�b�N�A�����u���b�N�̏�ӂɒ����Ă����痎�����~�߂�
	if (MapHitCheck(m_player->GetPos().x + wsize, m_player->GetPos().y + hsize, Dummy, MoveY) == 3)
	{
		m_fallPlayerSpeed = 0.0f;
	}

	// ����̃`�F�b�N�A�����u���b�N�̉��ӂɓ������Ă����痎��������
	if (MapHitCheck(m_player->GetPos().x - wsize, m_player->GetPos().y - hsize, Dummy, MoveY) == 4)
	{
		m_fallPlayerSpeed *= -1.0f;
	}

	// �E��̃`�F�b�N�A�����u���b�N�̉��ӂɓ������Ă����痎��������
	if (MapHitCheck(m_player->GetPos().x + wsize, m_player->GetPos().y - hsize, Dummy, MoveY) == 4)
	{
		m_fallPlayerSpeed *= -1.0f;
	}

	// �␳���ꂽ�㉺�ړ����������Z���Ď��ۂɈړ��B�������ĂȂ������炻�̂܂܌v�Z�����
	m_player->AddPos({0.0f,MoveY});

	// ��ɍ��E�ړ����������Ń`�F�b�N
	// �����̃`�F�b�N
	MapHitCheck(m_player->GetPos().x - hsize, m_player->GetPos().y + hsize, MoveX, Dummy);

	// �E���̃`�F�b�N
	MapHitCheck(m_player->GetPos().x + hsize, m_player->GetPos().y + hsize, MoveX, Dummy);

	// ����̃`�F�b�N
	MapHitCheck(m_player->GetPos().x - hsize, m_player->GetPos().y - hsize, MoveX, Dummy);

	// �E��̃`�F�b�N
	MapHitCheck(m_player->GetPos().x + hsize, m_player->GetPos().y - hsize, MoveX, Dummy);

	// ���E�ړ����������Z
	m_player->AddPos({ MoveX,0.0f });

	// �ڒn����
		// �L�����N�^�̍����ƉE���̉��ɒn�ʂ����邩���ׂ�
	if ((m_mapData->GetMapChipParam(m_player->GetPos().x - m_player->GetColSize().w * 0.5f, m_player->GetPos().y + m_player->GetColSize().h * 0.5f + 1.0f) == sky ||
		m_mapData->GetMapChipParam(m_player->GetPos().x - m_player->GetColSize().w * 0.5f, m_player->GetPos().y + m_player->GetColSize().h * 0.5f + 1.0f) == no) &&

		(m_mapData->GetMapChipParam(m_player->GetPos().x + m_player->GetColSize().w * 0.5f, m_player->GetPos().y + m_player->GetColSize().h * 0.5f + 1.0f) == sky ||
		m_mapData->GetMapChipParam(m_player->GetPos().x + m_player->GetColSize().w * 0.5f, m_player->GetPos().y + m_player->GetColSize().h * 0.5f + 1.0f) == no))
	{
		// ���ꂪ����������W�����v���ɂ���
		m_player->SetJump(true);
	}
	else
	{
		// ���ꂪ�݂�����ڒn���ɂ���
		m_player->SetJump(false);
	}

	// �I��
	return 0;
}

int CutManStage::MapHitCheck(float X, float Y, float& MoveX, float& MoveY)
{
	float afterX, afterY;
	//�ړ��ʂ𑫂�
	afterX = X + MoveX;
	afterY = Y + MoveY;
	float blockLeftX = 0.0f, blockTopY = 0.0f, blockRightX = 0.0f, blockBottomY = 0.0f;

	//�������Ă�����ǂ��痣���������s���A�u���b�N�̍��E�㉺�̍��W���Z�o
	blockLeftX = static_cast<float>((int)afterX / Game::ChipSize) * Game::ChipSize;//���@X���W
	blockRightX = static_cast<float>((int)afterX / Game::ChipSize + 1) * Game::ChipSize;//�E�@X���W

	blockTopY = static_cast<float>((int)afterY / Game::ChipSize) * Game::ChipSize;//��@X���W
	blockBottomY = static_cast<float>((int)afterY / Game::ChipSize + 1) * Game::ChipSize;//���@X���W

	//�����蔻��̂���u���b�N�ɓ������Ă��邩
	if (m_mapData->GetMapChipParam(afterX, afterY) == terrain ||
		m_mapData->GetMapChipParam(afterX, afterY) == wall1 ||
		m_mapData->GetMapChipParam(afterX, afterY) == wall2 ||
		//m_mapData->GetMapChipParam(afterX, afterY) == ladder2 ||
		m_mapData->GetMapChipParam(afterX, afterY) == stone )
	{
		//��ɓ������Ă����ꍇ
		if (MoveY > 0.0f)
		{
			//�ړ��ʂ�␳����
			MoveY = blockTopY - Y - 1.0f;
			//��ӂɓ��������ƕԂ�
			return 3;
		}
		//���ɓ������Ă����ꍇ
		if (MoveY < 0.0f)
		{
			//�ړ��ʂ�␳����
			MoveY = blockBottomY - Y + 1.0f;
			//���ӂɓ��������ƕԂ�
			return 4;
		}
		//���ɓ������Ă����ꍇ
		if (MoveX > 0.0f)
		{
			//�ړ��ʂ�␳����
			MoveX = blockLeftX - X - 1.0f;
			//���ӂɓ��������ƕԂ�
			return 1;
		}
		//�E�ɓ������Ă����ꍇ
		if (MoveX < 0.0f)
		{
			//�ړ��ʂ�␳����
			MoveX = blockRightX - X + 1.0f;
			//�E�ӂɓ��������ƕԂ�
			return 2;
		}
		//�����ɗ�����K���Ȓl��Ԃ�
		return 4;
	}
	else if (m_mapData->GetMapChipParam(afterX, afterY) == ladder2)
	{
		//��ɓ������Ă����ꍇ
		if (MoveY > 0.0f)
		{
			//�ړ��ʂ�␳����
			MoveY = blockTopY - Y - 1.0f;
			//��ӂɓ��������ƕԂ�
			return 3;
		}
	}
	//�ǂ��ɂ�������Ȃ������ƕԂ�
	return 0;
}

void CutManStage::FadeInUpdat(const InputState& input)
{
	m_fadeValue = 255 * m_fadeTimer / kFadeInterval;
	if (--m_fadeTimer == 0)
	{
		m_updateFunc = &CutManStage::NormalUpdat;
		m_fadeValue = 0;
		return;
	}
}

void CutManStage::NormalUpdat(const InputState& input)
{
	float PlayerMoveX = 0.0f, PlayerMoveY = 0.0f;
	if (input.IsPressed(InputType::left))
	{
		{
			PlayerMoveX -= kMoveSpeed;
		}
		m_player->SetLeft(true);
	}
	//�E�Ɉړ�
	else if (input.IsPressed(InputType::right))
	{
		
		{
			PlayerMoveX += kMoveSpeed;
		}
		m_player->SetLeft(false);
	}
	//�v���C���[�W�����v����
	if (m_player->IsJump() == false && input.IsTriggered(InputType::junp))
	{
		m_fallPlayerSpeed = -kJumpAcc;
		m_player->SetJump(true);
	}

	if (input.IsTriggered(InputType::next))
	{
		m_updateFunc = &CutManStage::FadeOutUpdat;
		m_fadeColor = 0xff0000;
	}
}

void CutManStage::FadeOutUpdat(const InputState& input)
{
	m_fadeValue = 255 * m_fadeTimer / kFadeInterval;
	if (++m_fadeTimer == kFadeInterval)
	{
		m_manager.ChangeScene(new GameoverScene(m_manager));
		return;
	}
}
