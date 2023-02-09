#include "GameplayingScene.h"
#include <DxLib.h>
#include "../game.h"
#include "../Util/Sound.h"
#include "../Util/InputState.h"
#include "../Util/DrawFunctions.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "PauseScene.h"
#include "GameStartCountScene.h"
#include "GameoverScene.h"
#include "../Shot/RockBuster.h"

#include "../Game/Player.h"
#include "../Game/EnemyFactory.h"
#include "../Game/EnemyBase.h"
#include "../Game/HpBar.h"
#include "../Map.h"

namespace
{
	constexpr float kPlayerMoveSpeed = 5.0f;//�v���C���[�̈ړ����x
	constexpr float kEnemyMoveSpeed = -4.0f;//�G�l�~�[�̈ړ����x
	constexpr float kJumpAcc = 10.0f;//�W�����v��
	constexpr float kShotSpeed = 8.0f;//�V���b�g�X�s�[�h

	constexpr float kPullPos = 10.0f;
}

GameplayingScene::GameplayingScene(SceneManager& manager) : Scene(manager), m_updateFunc(&GameplayingScene::FadeInUpdat)
, m_add(), m_correction()
{
	m_test = MakeScreen( Game::kScreenWidth,Game::kScreenHeight, true);

	m_map = std::make_shared<Map>();
	
	m_player = std::make_shared<Player>(Position2{(Game::kMapScreenLeftX + Game::ChipSize*8),(Game::kMapScreenBottomY - 6*Game::ChipSize)});//�v���C���[�̏����ʒu
	m_enemyFactory = std::make_shared<EnemyFactory>(m_player);//�v���C���[��n��

	m_map->Movement({ Game::kMapScreenLeftX,((Game::kMapChipNumY * Game::ChipSize) - Game::kMapScreenBottomY) * -1.0f });//�\���ʒu���w��
	m_add = { -Game::kMapScreenLeftX ,(Game::kMapChipNumY * Game::ChipSize) - Game::kMapScreenBottomY};

	for (auto& hp : m_hp)
	{
		hp = std::make_shared<HpBar>();
		hp->Init(my::MyLoadGraph(L"Data/hp.bmp"));
	}

	for (auto& shot : m_shots)
	{
		shot = std::make_shared<RockBuster>();
	}
}

GameplayingScene::~GameplayingScene()
{
	DeleteGraph(m_test);
	Sound::StopBgm(Sound::BgmMain);
}

void
GameplayingScene::Update(const InputState& input)
{
	(this->*m_updateFunc)(input);
}

void
GameplayingScene::Draw()
{
	//SetDrawScreen(m_test);
	
	m_map->Draw();

	m_player->Draw();
	m_enemyFactory->Draw();//�G�l�~�[��\��
	int num = 0;
	for (auto& shot : m_shots)//�V���b�g��\��
	{
		if (shot->IsExist())
		{
			shot->Draw();
			num++;
		}
	}

	m_hp[Object_Player]->Draw(true);//HP�o�[��\��

	bool ismieru = false;
	//�����Ȃ��ꏊ�����
	DrawBox(0, 0, Game::kMapScreenLeftX, Game::kScreenHeight, 0x000000, ismieru);//����
	DrawBox(Game::kMapScreenRightX, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, ismieru);//�E��
	DrawBox(Game::kMapScreenLeftX, 0, Game::kMapScreenRightX, Game::kMapScreenTopY, 0x000000, ismieru);//�㑤
	DrawBox(Game::kMapScreenLeftX, Game::kMapScreenBottomY, Game::kMapScreenRightX, Game::kScreenHeight, 0x000000, ismieru);//����

#ifdef _DEBUG
	//��q�ɏ���͈�
	DrawBoxAA(m_player->GetRect().GetCenter().x - m_player->GetRect().GetSize().w / 2 + kPullPos, m_player->GetRect().GetCenter().y - m_player->GetRect().GetSize().h / 2 - 1.0f,
		m_player->GetRect().GetCenter().x + m_player->GetRect().GetSize().w / 2 - kPullPos, m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h / 2 + 1.0f, 0xffffff, false);

	DrawFormatString(150, 20, 0xffffff, L"�e�̐�%d", num);
	if (m_isPlayerCenterLR)
	{
		DrawFormatString(150, 40, 0xff00ff, L"lR,true");
	}
	else
	{
		DrawFormatString(150, 40, 0xff00ff, L"lR,false");
	}
	if (m_isPlayerCenterUD)
	{
		DrawFormatString(150, 60, 0xff00ff, L"uD,true");
	}
	else
	{
		DrawFormatString(150, 60, 0xff00ff, L"uD,false");
	}
	int centeridxX = Game::kNumX / 2;
	int centeridxY = Game::kNumY / 2;
	Position2 fieldCenterLeftUp =
	{
		static_cast<float>(centeridxX * Game::ChipSize),
		static_cast<float>(centeridxY * Game::ChipSize)
	};
	Position2 fieldCenterRightBottom =
	{
		fieldCenterLeftUp.x + Game::ChipSize,
		fieldCenterLeftUp.y + Game::ChipSize
	};
	DrawBoxAA(fieldCenterLeftUp.x, fieldCenterLeftUp.y, fieldCenterRightBottom.x, fieldCenterRightBottom.y, 0xaaffaa, false);//��ʂ̒��S�ʒu

	DrawFormatString(0, 80, 0xffffff, L"player.x%3f,y.%3f", m_player->GetRect().center.x+m_add.x, m_player->GetRect().center.y+m_add.y);//�v���C���[�Ƒ������W
	DrawFormatString(0, 100, 0xffffff, L"player.x%3f,y.%3f", m_player->GetRect().center.x, m_player->GetRect().center.y);//�v���C���[���W
	DrawFormatString(0, 120, 0xffffff, L"add.x%3f,y.%3f",m_add.x, m_add.y);//��ʂ��ǂ̂��炢�ړ�������

	//�\���������}�b�v���
	DrawBox(Game::kMapScreenLeftX, Game::kMapScreenTopY, Game::kMapScreenRightX, Game::kMapScreenBottomY, 0xffffff, false);
	//���肷��͈�
	DrawBox(Game::kMapScreenLeftX, Game::kMapScreenTopY, Game::kMapScreenRightX, Game::kMapScreenBottomY - 1.0f, 0xffaaaa, false);
#endif
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeValue);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, m_fadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

bool GameplayingScene::createShot(Position2 pos, bool isPlayer, bool isLeft)
{
	for (auto& shot : m_shots)
	{
		if (!shot->IsExist())
		{
			shot->Start(pos,isLeft);
			shot->SetHandle(my::MyLoadGraph(L"Data/rockBuster.bmp"));
			shot->PlayerShot(isPlayer);
			shot->SetExist(true);
			return true;
		}
	}

	return false;
}

void GameplayingScene::MovePlayer(float MoveX, float MoveY)
{
	float Dummy = 0.0f;
	float hsize, wsize;

	// �L�����N�^�̍���A�E��A�����A�E�������������蔻��̂���
	// �}�b�v�ɏՓ˂��Ă��邩���ׁA�Փ˂��Ă�����␳����

	// �����̃T�C�Y���Z�o
	wsize = m_player->GetRect().GetSize().w * 0.5f;
	hsize = m_player->GetRect().GetSize().h * 0.5f;

	// �����̃`�F�b�N�A�����u���b�N�̏�ӂɒ����Ă����痎�����~�߂�
	//���ʒu�A���ʒu
	if (MapHitCheck(m_player->GetRect().GetCenter().x - wsize + m_add.x, m_player->GetRect().GetCenter().y + hsize + m_add.y, Dummy, MoveY) == 3)
	{

		m_fallPlayerSpeed = 0.0f;
	}

	// �E���̃`�F�b�N�A�����u���b�N�̏�ӂɒ����Ă����痎�����~�߂�
	//�E�ʒu�A���ʒu
	if (MapHitCheck(m_player->GetRect().GetCenter().x + wsize + m_add.x, m_player->GetRect().GetCenter().y + hsize + m_add.y, Dummy, MoveY) == 3)
	{
		m_fallPlayerSpeed = 0.0f;
	}

	// ����̃`�F�b�N�A�����u���b�N�̉��ӂɓ������Ă����痎��������
	//���ʒu�A��ʒu
	if (MapHitCheck(m_player->GetRect().GetCenter().x - wsize + m_add.x, m_player->GetRect().GetCenter().y - hsize + m_add.y, Dummy, MoveY) == 4)
	{
		m_fallPlayerSpeed *= -1.0f;
	}

	// �E��̃`�F�b�N�A�����u���b�N�̉��ӂɓ������Ă����痎��������
	//�E�ʒu�A��ʒu
	if (MapHitCheck(m_player->GetRect().GetCenter().x + wsize + m_add.x, m_player->GetRect().GetCenter().y - hsize + m_add.y, Dummy, MoveY) == 4)
	{
		m_fallPlayerSpeed *= -1.0f;
	}

	m_player->Movement({ 0.0f,MoveY });

	// ��ɍ��E�ړ����������Ń`�F�b�N
	// �����̃`�F�b�N
	MapHitCheck(m_add.x + m_player->GetRect().GetCenter().x - wsize, m_player->GetRect().GetCenter().y + hsize + m_add.y, MoveX, Dummy);

	// �E���̃`�F�b�N
	MapHitCheck(m_add.x + m_player->GetRect().GetCenter().x + wsize, m_player->GetRect().GetCenter().y + hsize + m_add.y, MoveX, Dummy);

	// ����̃`�F�b�N
	MapHitCheck(m_add.x + m_player->GetRect().GetCenter().x - wsize, m_player->GetRect().GetCenter().y - hsize + m_add.y, MoveX, Dummy);

	// �E��̃`�F�b�N
	MapHitCheck(m_add.x + m_player->GetRect().GetCenter().x + wsize, m_player->GetRect().GetCenter().y - hsize + m_add.y, MoveX, Dummy);

	// ���E�ړ����������Z
	m_player->Movement({ MoveX,0.0f });

	// �ڒn����
	// �L�����N�^�̍����ƉE���̉��ɒn�ʂ����邩���ׂ�
	if ((m_map->GetMapChipParam(m_add.x + m_player->GetRect().GetCenter().x - m_player->GetRect().GetSize().w * 0.5f, m_add.y + m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h * 0.5f + 1.0f) == no)||
		(m_map->GetMapChipParam(m_add.x + m_player->GetRect().GetCenter().x - m_player->GetRect().GetSize().w * 0.5f, m_add.y + m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h * 0.5f + 1.0f) == sky) &&
		(m_map->GetMapChipParam(m_add.x + m_player->GetRect().GetCenter().x + m_player->GetRect().GetSize().w * 0.5f, m_add.y + m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h * 0.5f + 1.0f) == no)||
		(m_map->GetMapChipParam(m_add.x + m_player->GetRect().GetCenter().x + m_player->GetRect().GetSize().w * 0.5f, m_add.y + m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h * 0.5f + 1.0f) == sky))
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
	return;
}

void GameplayingScene::MoveEnemy(float MoveX, float MoveY)
{
	float Dummy = 0.0f;
	float hsize, wsize;

	for (auto& enemy : m_enemyFactory->GetEnemies())
	{
		if (!enemy->IsExist())	continue;
		// �����̃T�C�Y���Z�o
		wsize = enemy->GetRect().GetSize().w * 0.5f;
		hsize = enemy->GetRect().GetSize().h * 0.5f;
		
		//�ړ�������
		enemy->Movement({ MoveX,MoveY });

		//��ʂ̍��[�ɏ�������
		if (enemy->GetRect().GetCenter().x + wsize < Game::kMapScreenLeftX)
		{
			enemy->SetExist(false);
			break;
		}
		//��ʂ̉��[�ɏ�������
		if (enemy->GetRect().GetCenter().y - hsize > Game::kMapScreenBottomY)
		{
			enemy->SetExist(false);
			break;
		}
		//��ʂ̏�[�ɏ�������
		else if (enemy->GetRect().GetCenter().y + hsize < Game::kMapScreenTopY)
		{
			enemy->SetExist(false);
			break;
		}
	}

	// �I��
	return;
}

void GameplayingScene::MoveShot(int i ,float MoveX, float MoveY)
{
	float Dummy = 0.0f;
	float hsize, wsize;

	// �����̃T�C�Y���Z�o
	wsize = m_shots[i]->GetRect().GetSize().w * 0.5f;
	hsize = m_shots[i]->GetRect().GetSize().h * 0.5f;

	if (m_shots[i]->IsLeft())//���Ɍ������e���ǂ���
	{
		//���ɑł����ꍇ�͋t�����Ɉړ��ł���悤��
		MoveX *= -1.0f;
	}

	// ���E�ړ����������Z
	m_shots[i]->Movement({ MoveX,MoveY });

	//��ʂ̊O�ɏo���������
	//���[
	if (m_shots[i]->GetRect().GetCenter().x + wsize < Game::kMapScreenLeftX - wsize*2)
	{
		m_shots[i]->SetExist(false);
	}
	//�E�[
	if (m_shots[i]->GetRect().GetCenter().x - wsize > Game::kMapScreenRightX + wsize*2)
	{
		m_shots[i]->SetExist(false);
	}

	// �I��
	return ;
}

int GameplayingScene::MapHitCheck(float X, float Y, float& MoveX, float& MoveY)
{
	float afterX, afterY;
	//�ړ��ʂ𑫂�
	afterX = X + MoveX;
	afterY = Y + MoveY;
	float blockLeftX = 0.0f, blockTopY = 0.0f, blockRightX = 0.0f, blockBottomY = 0.0f;

	//�����l�֕ϊ�
	int noX = static_cast<int>(afterX / Game::ChipSize);
	int noY = static_cast<int>(afterY / Game::ChipSize);

	//�������Ă�����ǂ��痣���������s���A�u���b�N�̍��E�㉺�̍��W���Z�o
	blockLeftX = static_cast<float>(noX * Game::ChipSize);//���@X���W
	blockRightX = static_cast<float>((noX + 1) * Game::ChipSize);//�E�@X���W

	blockTopY = static_cast<float>(noY * Game::ChipSize);//��@Y���W
	blockBottomY = static_cast<float>((noY + 1) * Game::ChipSize);//���@Y���W

	int mapchip = m_map->GetMapChipParam(afterX, afterY);
	//�����蔻��̂���u���b�N�ɓ������Ă��邩
	if (mapchip  == terrain ||
		mapchip == wall1 ||
		mapchip == wall2 ||
		mapchip == stone)
	{
		//�u���b�N�̏�ɓ������Ă����ꍇ
		if (MoveY > 0.0f)
		{
			//�ړ��ʂ�␳����
			MoveY = blockTopY - Y  - 1.0f;
			//��ӂɓ��������ƕԂ�
			return 3;
		}
		//�u���b�N�̉��ɓ������Ă����ꍇ
		if (MoveY < 0.0f)
		{
			//�ړ��ʂ�␳����
			MoveY = blockBottomY - Y + 1.0f;
			//���ӂɓ��������ƕԂ�
			return 4;
		}
		//�u���b�N�̍��ɓ������Ă����ꍇ
		if (MoveX > 0.0f)
		{
			//�ړ��ʂ�␳����
			MoveX = blockLeftX - X - 1.0f;
			//���ӂɓ��������ƕԂ�
			return 1;
		}
		//�u���b�N�̉E�ɓ������Ă����ꍇ
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
	//�ǂ��ɂ�������Ȃ������ƕԂ�
	return 0;
}

void GameplayingScene::PlayerCenter()
{
	//��ʂ̑傫���̔���
	int centeridxX = Game::kNumX / 2;
	int centeridxY = Game::kNumY / 2;

	Position2 fieldCenterLeftUp =
	{
		static_cast<float>(centeridxX * Game::ChipSize),
		static_cast<float>(centeridxY * Game::ChipSize)
	};
	Position2 fieldCenterRightBottom =
	{
		fieldCenterLeftUp.x + Game::ChipSize,
		fieldCenterLeftUp.y + Game::ChipSize
	};

	m_isPlayerCenterUD = false;
	m_isPlayerCenterLR = false;

	if (m_player->GetRect().GetCenter().x > fieldCenterLeftUp.x && m_player->GetRect().GetCenter().x < fieldCenterRightBottom.x)
	{
		m_isPlayerCenterLR = true;
	}
	if (m_player->GetRect().GetCenter().y > fieldCenterLeftUp.y && m_player->GetRect().GetCenter().y < fieldCenterRightBottom.y)
	{
		m_isPlayerCenterUD = true;
	}

	return;
}

void GameplayingScene::MoveMap(float MoveX, float MoveY)
{
	float Dummy = 0.0f;
	float wsize = m_player->GetRect().GetSize().w * 0.5f;
	float hsize = m_player->GetRect().GetSize().h * 0.5f;

	MoveX *= -1.0f;
	MoveY *= -1.0f;

	// �����̃`�F�b�N�A�����u���b�N�̏�ӂɒ����Ă����痎�����~�߂�
	if (MapHitCheck(m_player->GetRect().GetCenter().x - wsize + m_add.x, m_player->GetRect().GetCenter().y + hsize + m_add.y, Dummy, MoveY) == 3)
	{

		m_fallPlayerSpeed = 0.0f;
	}
	// �E���̃`�F�b�N�A�����u���b�N�̏�ӂɒ����Ă����痎�����~�߂�
	if (MapHitCheck(m_player->GetRect().GetCenter().x + wsize + m_add.x, m_player->GetRect().GetCenter().y + hsize + m_add.y, Dummy, MoveY) == 3)
	{
		m_fallPlayerSpeed = 0.0f;
	}
	// ����̃`�F�b�N�A�����u���b�N�̉��ӂɓ������Ă����痎��������
	if (MapHitCheck(m_player->GetRect().GetCenter().x - wsize + m_add.x, m_player->GetRect().GetCenter().y - hsize + m_add.y, Dummy, MoveY) == 4)
	{
		m_fallPlayerSpeed *= -1.0f;
	}
	// �E��̃`�F�b�N�A�����u���b�N�̉��ӂɓ������Ă����痎��������
	if (MapHitCheck(m_player->GetRect().GetCenter().x + wsize + m_add.x, m_player->GetRect().GetCenter().y - hsize + m_add.y, Dummy, MoveY) == 4)
	{
		m_fallPlayerSpeed *= -1.0f;
	}
	
	m_add += {0.0f, MoveY};
	MoveY *= -1.0f;
	m_map->Movement({ 0.0f,MoveY });
	m_correction.y += MoveY;

	// �����̃`�F�b�N
	MapHitCheck(m_player->GetRect().GetCenter().x - wsize + m_add.x, m_player->GetRect().GetCenter().y + hsize + m_add.y, MoveX, Dummy);
	// �E���̃`�F�b�N
	MapHitCheck(m_player->GetRect().GetCenter().x + wsize + m_add.x, m_player->GetRect().GetCenter().y + hsize + m_add.y, MoveX, Dummy);
	// ����̃`�F�b�N
	MapHitCheck(m_player->GetRect().GetCenter().x - wsize + m_add.x, m_player->GetRect().GetCenter().y - hsize + m_add.y, MoveX, Dummy);
	// �E��̃`�F�b�N
	MapHitCheck(m_player->GetRect().GetCenter().x + wsize + m_add.x, m_player->GetRect().GetCenter().y - hsize + m_add.y, MoveX, Dummy);

	m_add += { MoveX, 0.0f };
	MoveX *= -1.0f;
	m_map->Movement({ MoveX,0.0f });
	m_correction.x += MoveX;
	//�I��
	return;
}

void GameplayingScene::FadeInUpdat(const InputState& input)
{
	m_fadeValue = 255 * m_fadeTimer / kFadeInterval;
	if (--m_fadeTimer == 0)
	{
		Sound::StartBgm(Sound::BgmMain);
		m_updateFunc = &GameplayingScene::NormalUpdat;
		m_fadeValue = 0;
		//m_manager.PushScene(new GameStartCountScene(m_manager));
		return;
	}
}

void GameplayingScene::NormalUpdat(const InputState& input)
{
	PlayerCenter();//�v���C���[���Z���^�[�ɋ��邩�ǂ���

	float PlayerMoveX = 0.0f, PlayerMoveY = 0.0f;//�v���C���[�̈ړ�
	m_correction = {0.0f,0.0f};
	//���Ɉړ�
	if (input.IsPressed(InputType::left))
	{
		if (input.IsTriggered(InputType::left))
		{
			m_player->SetLeft(true);
		}
		else
		{
			//�}�b�v��\�������ʂ̍���A�����ɉ����Ȃ���Ԃ������Ƃ��̓}�b�v���ړ��ł��Ȃ�
			//�}�b�v��\�������ʂ̍���A�����ɉ����\���ł�����̂�����Ƃ��͈ړ��ł���
			if (m_isPlayerCenterLR &&
				m_map->GetMapChipParam(m_add.x + Game::kMapScreenLeftX, m_add.y + Game::kMapScreenTopY) != no &&
				m_map->GetMapChipParam(m_add.x + Game::kMapScreenLeftX, m_add.y + Game::kMapScreenBottomY - 1.0f) != no)
			{
				MoveMap(kPlayerMoveSpeed, 0.0f);
			}
			else
			{
				PlayerMoveX -= kPlayerMoveSpeed;
			}
			m_player->SetLeft(true);
			m_player->Action(ActionType::grah_walk);
		}
		m_player->Action(ActionType::grah_idle);
	}
	//�E�Ɉړ�
	else if (input.IsPressed(InputType::right))
	{
		if (input.IsTriggered(InputType::right))
		{
			m_player->SetLeft(false);
		}
		else
		{
			//�}�b�v��\�������ʂ̉E��A�E���ɉ����Ȃ���Ԃ������Ƃ��̓}�b�v���ړ��ł��Ȃ�
			//�}�b�v��\�������ʂ̉E��A�����ɉ����\���ł�����̂�����Ƃ��͈ړ��ł���
			if (m_isPlayerCenterLR &&
				m_map->GetMapChipParam(m_add.x + Game::kMapScreenRightX  , m_add.y + Game::kMapScreenTopY) != no &&
				m_map->GetMapChipParam(m_add.x + Game::kMapScreenRightX , m_add.y + Game::kMapScreenBottomY -1.0f ) != no)
			{
				MoveMap(-kPlayerMoveSpeed, 0.0f);
			}
			else
			{
				PlayerMoveX += kPlayerMoveSpeed;
			}
			m_player->SetLeft(false);
			m_player->Action(ActionType::grah_walk);
		}
		m_player->Action(ActionType::grah_idle);
	}

	//�v���C���[�W�����v����
	if (m_player->IsJump() == false && input.IsTriggered(InputType::junp))
	{
		m_fallPlayerSpeed = -kJumpAcc;
		m_player->Action(ActionType::grah_jump);
		m_player->SetJump(true);
	}

	//�v���C���[���W�����v���Ă�����
	if (m_player->IsJump())
	{
		// ��������
		m_fallPlayerSpeed += 0.4f;
		float camera = m_map->GetPos().y + Game::kMapChipNumY * Game::ChipSize;
		if (m_isPlayerCenterUD && camera > Game::kMapScreenBottomY)//���[�����傫���ꍇ
		{
			MoveMap(0.0f, -m_fallPlayerSpeed);
		}
		else
		{
			// �������x���ړ��ʂɉ�����
			PlayerMoveY = m_fallPlayerSpeed;
		}
	}

	//��q�ړ�
	//�v���C���[�̏㉺ �̏ꏊ�ɒ�q�����邩�@�v���C���[�̏�ɒ�q�P�����A�v���C���[�̏㉺�ɒ�q�Q������Ƃ�
	if ((m_map->GetMapChipParam(m_add.x + m_player->GetRect().GetCenter().x- m_player->GetRect().GetSize().w / 2+ kPullPos, m_add.y + m_player->GetRect().GetCenter().y - m_player->GetRect().GetSize().h / 2 - 2.0f) == ladder1)||
		(m_map->GetMapChipParam(m_add.x + m_player->GetRect().GetCenter().x- m_player->GetRect().GetSize().w / 2+ kPullPos, m_add.y + m_player->GetRect().GetCenter().y - m_player->GetRect().GetSize().h / 2 - 2.0f) == ladder2) &&
		(m_map->GetMapChipParam(m_add.x + m_player->GetRect().GetCenter().x+ m_player->GetRect().GetSize().w / 2- kPullPos, m_add.y + m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h / 2 + 2.0f) == ladder1) ||
		(m_map->GetMapChipParam(m_add.x + m_player->GetRect().GetCenter().x+ m_player->GetRect().GetSize().w / 2- kPullPos, m_add.y + m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h / 2 + 2.0f) == ladder2))
	{
		DrawString(400, 40, L"��q", 0xffffff);
		//��L�[�Œ�q���オ���
		if (input.IsPressed(InputType::up))
		{
			if (m_isPlayerCenterUD && m_map->GetPos().y < Game::kMapScreenTopY)//��[�����������ꍇ
			{
				MoveMap(0.0f, kPlayerMoveSpeed);
			}
			else
			{
				PlayerMoveY -= kPlayerMoveSpeed;
			}
		}
		//���L�[�Œ�q���������
		if (input.IsPressed(InputType::down))
		{
			float camera = m_map->GetPos().y + Game::kMapChipNumY * Game::ChipSize;
			if (m_isPlayerCenterUD && camera > Game::kMapScreenBottomY)//���[�����傫���ꍇ
			{
				MoveMap(0.0f, -kPlayerMoveSpeed);
			}
			else
			{
				PlayerMoveY += kPlayerMoveSpeed;
			}
		}
	}

	// �ړ��ʂɊ�Â��ăL�����N�^�̍��W���ړ�
	MovePlayer(PlayerMoveX, PlayerMoveY);

	m_map->Update();		//	�}�b�v�X�V
	m_player->Update();		//	�v���C���[�X�V
	
	//�G�l�~�[
	float MoveX = kEnemyMoveSpeed + m_correction.x, MoveY = m_correction.y;
	MoveEnemy(MoveX, MoveY);
	m_enemyFactory->Update();

	//�V���b�g
	if (input.IsTriggered(InputType::shot))//shot����������e�����
	{
		createShot(m_player->GetRect().GetCenter(), true, m_player->IsLeft());
		m_player->Action(ActionType::grah_attack);
	}

	for (int i = 0; i < kShot; i++)
	{
		if (m_shots[i]->IsExist())//���݂��Ă���e�����X�V����
		{
			m_shots[i]->Update();
			MoveShot(i, kShotSpeed, m_correction.y);
		}
	}

	//���@�̒e�ƁA�G�@�̓����蔻��
	for (auto& shot : m_shots)
	{
		if (!shot->IsExist())	continue;
		if (!shot->GetPlayerShot())	continue;//�v���C���[���������e�ł͂Ȃ������珈�����Ȃ�
		for (auto& enemy : m_enemyFactory->GetEnemies())
		{
			if (!enemy->IsExist())	continue;
			//�G�ɒe���q�b�g����
			if (shot->GetRect().IsHit(enemy->GetRect()))
			{
				shot->SetExist(false);
				enemy->Damage(1);
				break;
			}
		}
	}

	//�G�̒e�ƁA�v���C���[�̓����蔻��
	for (auto& shot : m_shots)
	{
		if (!shot->IsExist())	continue;
		if (shot->GetPlayerShot())	continue;//�v���C���[���������e�������珈�����Ȃ�
		
		if (shot->GetRect().IsHit(m_player->GetRect()))
		{
			shot->SetExist(false);
			m_hp[Object_Player]->Damage(1);
			m_player->Action(ActionType::grah_hit);
			break;
		}
	}

	//�v���C���[�ƁA�G�̓����蔻��
	for (auto& enemy : m_enemyFactory->GetEnemies())
	{
		if (!enemy->IsExist())	continue;
		//�G�ƃv���C���[����������
		if (enemy->GetRect().IsHit(m_player->GetRect()))
		{
			enemy->Damage(1);
			m_hp[Object_Player]->Damage(1);
			m_player->Action(ActionType::grah_hit);
			break;
		}
	}

	//�Q�[���I�[�o�[����
	{
		//�v���C���[��HP���O�ɂȂ�����Q�[���I�[�o�[�ɂ���
		if (m_hp[Object_Player]->GetHp() <= 0)
		{
			m_updateFunc = &GameplayingScene::FadeOutUpdat;
			m_fadeColor = 0xff0000;
		}
		//�j�̂��镔���ɏ���Ă�����
		if ((m_map->GetMapChipParam(m_add.x + m_player->GetRect().GetCenter().x - m_player->GetRect().GetSize().w * 0.5f, m_add.y + m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h * 0.5f + 1.0f) == thorn) &&
			(m_map->GetMapChipParam(m_add.x + m_player->GetRect().GetCenter().x + m_player->GetRect().GetSize().w * 0.5f, m_add.y + m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h * 0.5f + 1.0f) == thorn))
		{
			m_player->Action(ActionType::grah_hit);
			m_updateFunc = &GameplayingScene::FadeOutUpdat;
			m_fadeColor = 0xff0000;
		}
	}
	
	if (input.IsTriggered(InputType::next))
	{
		for (auto& shot : m_shots)
		{
			shot->SetExist(false);
		}

		m_updateFunc = &GameplayingScene::FadeOutUpdat;
		m_fadeColor = 0xff0000;
	}
	if (input.IsTriggered(InputType::pause))
	{
		m_manager.PushScene(new PauseScene(m_manager));
		return;
	}
}

void GameplayingScene::FadeOutUpdat(const InputState& input)
{
	m_fadeValue = 255 * m_fadeTimer / kFadeInterval;
	if(++m_fadeTimer == kFadeInterval)
	{
		m_manager.ChangeScene(new GameoverScene(m_manager));
		return;
	}
}

