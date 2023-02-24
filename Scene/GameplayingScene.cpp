#include "GameplayingScene.h"
#include <DxLib.h>
#include "SceneManager.h"
#include "TitleScene.h"
#include "PauseScene.h"
#include "GameStartCountScene.h"
#include "GameoverScene.h"
#include "GameclearScene.h"

#include "../game.h"
#include "../Map.h"
#include "../Util/Sound.h"
#include "../Util/InputState.h"
#include "../Util/DrawFunctions.h"

#include "../Game/Player.h"
#include "../Game/EnemyFactory.h"
#include "../Game/ShotFactory.h"
#include "../Game/HpBar.h"
#include "../Enemy/EnemyBase.h"
#include "../Shot/RockBuster.h"
/*
�X�[�p�[�J�b�^�[�@�l�p���@�B�̌������ʂɏo�Ă���͂��݁BHP5�A�U����4
�X�N�����[�h���C�o�[�@���@���ߕt���ƁA�n�ʂ���o�Ă��āA�T���������ɒe���񔭎˂���
*/
namespace
{
	constexpr float kPlayerMoveSpeed = 5.0f;//�v���C���[�̈ړ����x
	constexpr float kJumpAcc = 10.0f;//�W�����v��
	constexpr float kShotSpeed = 8.0f;//�V���b�g�X�s�[�h

	constexpr float kPullPos = 10.0f;
}

GameplayingScene::GameplayingScene(SceneManager& manager) : Scene(manager), m_updateFunc(&GameplayingScene::FadeInUpdat)
, m_add(), m_correction()
{
	for (auto& hp : m_hp)
	{
		hp = std::make_shared<HpBar>();
		hp->Init(my::MyLoadGraph(L"Data/hp.bmp"));
	}

	for (auto& shot : m_shots)
	{
		shot = std::make_shared<RockBuster>(my::MyLoadGraph(L"Data/rockBuster.png"));
	}

	m_player = std::make_shared<Player>(Position2{(Game::kMapScreenLeftX + Game::ChipSize*8),(Game::kMapScreenBottomY - Game::ChipSize*5)},m_hp[Object_Player]);//�v���C���[�̏����ʒu
	
	m_shotFactory = std::make_shared<ShotFactory>();

	m_enemyFactory = std::make_shared<EnemyFactory>(m_player, m_shotFactory);//�v���C���[�ƃV���b�g��n��

	m_map = std::make_shared<Map>(m_enemyFactory,0);
	m_map->Load(L"Data/map.fmf");

	Position2 pos = { Game::kMapScreenLeftX,((Game::kMapChipNumY * Game::ChipSize) - Game::kMapScreenBottomY) * -1.0f };
	m_map->Movement(pos);
	m_add = pos * -1.0f;

	m_handle = my::MyLoadGraph(L"Data/Background.png");
}

GameplayingScene::~GameplayingScene()
{
	DeleteGraph(m_handle);
	Sound::StopBgm(Sound::BgmMain);
}

void GameplayingScene::Update(const InputState& input)
{
	(this->*m_updateFunc)(input);
}

void GameplayingScene::Draw()
{
	DrawExtendGraph(0, 0, Game::kScreenWidth, Game::kScreenHeight, m_handle, false);
	
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
	m_shotFactory->Draw();//�V���b�g�\��

	m_hp[Object_Player]->Draw(true);//HP�o�[��\��

	//�g�����
	DrawBox(Game::kMapScreenLeftX, Game::kMapScreenTopY, Game::kMapScreenLeftX - Game::ChipSize, Game::kMapScreenBottomY, m_framecolor, true);//����
	DrawBox(Game::kMapScreenRightX, Game::kMapScreenTopY, Game::kMapScreenRightX + Game::ChipSize, Game::kMapScreenBottomY, m_framecolor, true);//�E��
	DrawBox(Game::kMapScreenLeftX - Game::ChipSize, Game::kMapScreenTopY - Game::ChipSize, Game::kMapScreenRightX + Game::ChipSize, Game::kMapScreenTopY, m_framecolor, true);//��
	DrawBox(Game::kMapScreenLeftX - Game::ChipSize, Game::kMapScreenBottomY + Game::ChipSize, Game::kMapScreenRightX + Game::ChipSize, Game::kMapScreenBottomY, m_framecolor, true);//��

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
	if (m_isPlayerMoveU)
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
	DrawFormatString(0, 160, 0xffffff, L"m_correction.x%3f,y.%3f", m_correction.x, m_correction.y);//��ʂ��ǂ̂��炢�ړ�������

	//�\���������}�b�v���
	DrawBox(Game::kMapScreenLeftX, Game::kMapScreenTopY, Game::kMapScreenRightX, Game::kMapScreenBottomY, 0xffffff, false);
	DrawFormatString(Game::kMapScreenLeftX, Game::kMapScreenTopY, 0xffffff, L"%d", Game::kMapScreenLeftX);
	DrawFormatString(Game::kMapScreenRightX, Game::kMapScreenTopY, 0xffffff, L"%d",Game::kMapScreenTopY);
	DrawFormatString(Game::kMapScreenLeftX, Game::kMapScreenBottomY, 0xffffff, L"%d",Game::kMapScreenBottomY);
	DrawFormatString(Game::kMapScreenRightX, Game::kMapScreenBottomY, 0xffffff, L"%d",Game::kMapScreenRightX);
	//���肷��͈�
	DrawBox(Game::kMapScreenLeftX, Game::kMapScreenTopY, Game::kMapScreenRightX, Game::kMapScreenBottomY - 1, 0xffaaaa, false);
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
			shot->Start(pos, {0.0f,0.0f}, isLeft);
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

	// �L�����N�^�̍���A�E��A�����A�E�������������蔻��̂���}�b�v�ɏՓ˂��Ă��邩���ׁA�Փ˂��Ă�����␳����
	// �����̃T�C�Y���Z�o
	wsize = m_player->GetRect().GetSize().w * 0.5f;
	hsize = m_player->GetRect().GetSize().h * 0.5f;

	// �����̃`�F�b�N�A�����u���b�N�̏�ӂɒ����Ă����痎�����~�߂� ���ʒu�A���ʒu
	if (MapHitCheck(m_player->GetRect().GetCenter().x - wsize + m_add.x, m_player->GetRect().GetCenter().y + hsize + m_add.y, Dummy, MoveY) == 3)
	{

		m_fallPlayerSpeed = 0.0f;
	}

	// �E���̃`�F�b�N�A�����u���b�N�̏�ӂɒ����Ă����痎�����~�߂� �E�ʒu�A���ʒu
	if (MapHitCheck(m_player->GetRect().GetCenter().x + wsize + m_add.x, m_player->GetRect().GetCenter().y + hsize + m_add.y, Dummy, MoveY) == 3)
	{
		m_fallPlayerSpeed = 0.0f;
	}

	// ����̃`�F�b�N�A�����u���b�N�̉��ӂɓ������Ă����痎�������� ���ʒu�A��ʒu
	if (MapHitCheck(m_player->GetRect().GetCenter().x - wsize + m_add.x, m_player->GetRect().GetCenter().y - hsize + m_add.y, Dummy, MoveY) == 4)
	{
		m_fallPlayerSpeed *= -1.0f;
	}

	// �E��̃`�F�b�N�A�����u���b�N�̉��ӂɓ������Ă����痎�������� �E�ʒu�A��ʒu
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

	// �ڒn���� �L�����N�^�̍����ƉE���̉��ɒn�ʂ����邩���ׂ�
	//�����蔻��̂���ꏊ�ɗ����特��炵�đ��ꂪ���锻��ɂ���
	if ((m_map->GetMapEventParam(m_add.x + m_player->GetRect().GetCenter().x - m_player->GetRect().GetSize().w * 0.5f, m_add.y + m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h * 0.5f + 1.0f) == MapEvent_hit) ||
		(m_map->GetMapEventParam(m_add.x + m_player->GetRect().GetCenter().x + m_player->GetRect().GetSize().w * 0.5f, m_add.y + m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h * 0.5f + 1.0f) == MapEvent_hit))
	{
		if (m_player->IsJump())	Sound::Play(Sound::BlockMove);
		//���ꂪ��������ݒu���ɂ���
		m_player->SetJump(false);
	}
	//��q���������瑫�ꂪ���锻��ɂ���
	else if ((m_map->GetMapEventParam(m_add.x + m_player->GetRect().GetCenter().x - m_player->GetRect().GetSize().w * 0.5f, m_add.y + m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h * 0.5f + 1.0f) == MapEvent_ladder)||
		(m_map->GetMapEventParam(m_add.x + m_player->GetRect().GetCenter().x + m_player->GetRect().GetSize().w * 0.5f, m_add.y + m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h * 0.5f + 1.0f) == MapEvent_ladder))
	{
		m_player->SetJump(false);
	}
	else
	{
		//�Ȃ��Ƃ��̓W�����v���ɂ���
		m_player->SetJump(true);
	}
	// �I��
	return;
}

void GameplayingScene::MoveEnemy(float MoveX, float MoveY)
{
	float Dummy = 0.0f;
	float hsize, wsize;
	float moveX = 4.0f;
	float moveY = 1.0f;

	for (auto& enemy : m_enemyFactory->GetEnemies())
	{
		if (!enemy->IsExist())	continue;
		// �����̃T�C�Y���Z�o
		wsize = enemy->GetRect().GetSize().w * 0.5f;
		hsize = enemy->GetRect().GetSize().h * 0.5f;
		
		enemy->Movement({ MoveX,MoveY });

		if (enemy->IsLeft())
		{
			enemy->GetChip(m_map->GetMapEventParam(m_add.x + enemy->GetRect().GetCenter().x-wsize - moveX, m_add.y + enemy->GetRect().GetCenter().y + hsize + moveY));
		}
		else
		{
			enemy->GetChip(m_map->GetMapEventParam(m_add.x + enemy->GetRect().GetCenter().x+wsize + moveX, m_add.y + enemy->GetRect().GetCenter().y + hsize + moveY));
		}

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

	int mapchip = m_map->GetMapEventParam(afterX, afterY);
	//�����蔻��̂���u���b�N�ɓ������Ă��邩
	if (mapchip  == MapEvent_hit)
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
			MoveY = blockBottomY - Y + 1.0f;
			//���ӂɓ��������ƕԂ�
			return 4;
		}
		//�u���b�N�̍��ɓ������Ă����ꍇ
		if (MoveX > 0.0f)
		{
			MoveX = blockLeftX - X - 1.0f;
			//���ӂɓ��������ƕԂ�
			return 1;
		}
		//�u���b�N�̉E�ɓ������Ă����ꍇ
		if (MoveX < 0.0f)
		{
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
	Position2 fieldCenterLeftUp =//�t�B�[���h�̒��S�ʒu�i����j
	{
		static_cast<float>(Game::kNumX / 2 * Game::ChipSize),
		static_cast<float>(Game::kNumY / 2 * Game::ChipSize)
	};
	Position2 fieldCenterRightBottom =//�t�B�[���h�̒��S�ʒu�i�E���j
	{
		fieldCenterLeftUp.x + Game::ChipSize,
		fieldCenterLeftUp.y + Game::ChipSize
	};

	m_isPlayerCenterLR = false;
	m_isPlayerMoveU = false;
	m_isPlayerMoveD = false;
	m_isPlayerMoveW = false;

	if (m_player->GetRect().GetCenter().x > fieldCenterLeftUp.x && m_player->GetRect().GetCenter().x < fieldCenterRightBottom.x)
	{
		m_isPlayerCenterLR = true;
	}
	//�c�ړ��ł���ꏊ�ɂ�����ړ������� �E���̉��Ə�
	if ((m_map->GetMapEventParam(m_add.x + m_player->GetRect().GetCenter().x + m_player->GetRect().GetSize().w / 2 - kPullPos, m_add.y + m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h / 2 + 1.0f) == MapEvent_screenMoveW) &&
		(m_map->GetMapEventParam(m_add.x + m_player->GetRect().GetCenter().x + m_player->GetRect().GetSize().w / 2 - kPullPos, m_add.y + m_player->GetRect().GetCenter().y - m_player->GetRect().GetSize().h / 2 + 2.0f) == MapEvent_screenMoveW))
	{
		m_isPlayerMoveW = true;
	}
	//�v���C���[�̏���W���t�B�[���h�̏�ɓ������Ă�����ړ��ł���	
	if ((m_map->GetMapEventParam(m_add.x + m_player->GetRect().GetCenter().x - m_player->GetRect().GetSize().w / 2, m_add.y + m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h / 2 + 1.0f) == MapEvent_screenMoveU)&&
		(m_map->GetMapEventParam(m_add.x + m_player->GetRect().GetCenter().x + m_player->GetRect().GetSize().w / 2 - kPullPos, m_add.y + m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h / 2 + 2.0f) == MapEvent_screenMoveU))
	{
		m_isPlayerMoveU = true;
	}
	//�v���C���[�̉����W���c�ړ��ł���ꏊ�ɂ�����ړ�������
	if (!m_isPlayerMoveU &&
		(m_map->GetMapEventParam(m_add.x + m_player->GetRect().GetCenter().x - m_player->GetRect().GetSize().w / 2, m_add.y + m_player->GetRect().GetCenter().y - m_player->GetRect().GetSize().h / 2 - 1.0f) == MapEvent_screenMoveD) &&
		(m_map->GetMapEventParam(m_add.x + m_player->GetRect().GetCenter().x + m_player->GetRect().GetSize().w / 2 - kPullPos, m_add.y + m_player->GetRect().GetCenter().y - m_player->GetRect().GetSize().h / 2 - 2.0f) == MapEvent_screenMoveD))
	{
		m_isPlayerMoveD = true;
	}

	if (m_isPlayerMoveU || m_isPlayerMoveD || m_isPlayerMoveW)
	{
		//�V���b�g���폜����
		for (auto& shot : m_shots)
		{
			shot->SetExist(false);
		}
		for (auto& shot : m_shotFactory->GetShot())
		{
			if (!shot->IsExist())	continue;
			shot->SetExist(false);
		}
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
		return;
	}
}

void GameplayingScene::NormalUpdat(const InputState& input)
{
	PlayerCenter();//�v���C���[���Z���^�[�ɋ��邩�ǂ���
	m_player->Action(ActionType::grah_idle);
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
			PlayerMoveX -= kPlayerMoveSpeed;
		
			m_player->SetLeft(true);
			m_player->Action(ActionType::grah_walk);
		}
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
			//�v���C���[����ʒ����ɂ��違�}�b�v�̕\���ł���͈͈ȓ��̎��͓�������
			if (m_isPlayerCenterLR &&
				m_map->GetMapEventParam(m_add.x + Game::kMapScreenRightX  , m_add.y + Game::kMapScreenTopY) != MapEvent_screen &&
				m_map->GetMapEventParam(m_add.x + Game::kMapScreenRightX , m_add.y + Game::kMapScreenBottomY - 1.0f ) != MapEvent_screen)
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
	}

	float posX = m_add.x + m_player->GetRect().GetCenter().x;
	float posY = m_add.y + m_player->GetRect().GetCenter().y;
	//��q�ړ�
	//��L�[�Œ�q���オ���@�オ��q�܂��͉�����q
	if (input.IsPressed(InputType::up))
	{
		m_player->SetJump(false);
		m_fallPlayerSpeed = 0.0f;
		PlayerMoveY = 0.0f;
		if ((m_map->GetMapEventParam(posX + m_player->GetRect().GetSize().w / 2 - kPullPos, posY + m_player->GetRect().GetSize().h / 2 + 1.0f) == MapEvent_screenMoveU) ||
			(m_map->GetMapEventParam(posX + m_player->GetRect().GetSize().w / 2 - kPullPos, posY + m_player->GetRect().GetSize().h / 2 + 1.0f) == MapEvent_ladder))
		{
			PlayerMoveY -= kPlayerMoveSpeed;
			//���ɉ����Ȃ��Ƃ��@�͈ړ����Ȃ��悤�ɂ���
			if ((m_map->GetMapEventParam(posX + m_player->GetRect().GetSize().w / 2 - kPullPos, posY + m_player->GetRect().GetSize().h / 2) == MapEvent_no))
			{
				PlayerMoveY += kPlayerMoveSpeed;
			}
		}
	}
	//���L�[�Œ�q���������@������q
	else if (input.IsPressed(InputType::down))
	{
		m_player->SetJump(false);
		m_fallPlayerSpeed = 0.0f;
		PlayerMoveY = 0.0f;
		if ((m_map->GetMapEventParam(posX - m_player->GetRect().GetSize().w / 2 + kPullPos, posY - m_player->GetRect().GetSize().h / 2 + 2.0f) == MapEvent_ladder) ||
			(m_map->GetMapEventParam(posX + m_player->GetRect().GetSize().w / 2 - kPullPos, posY + m_player->GetRect().GetSize().h / 2 + 1.5f) == MapEvent_ladder))
		{
			PlayerMoveY += kPlayerMoveSpeed;
		}
	}
	//�v���C���[�W�����v����
	else if (m_player->IsJump() == false && input.IsTriggered(InputType::junp))
	{
		m_fallPlayerSpeed = -kJumpAcc;
		m_player->Action(ActionType::grah_jump);
		m_player->SetJump(true);
		Sound::Play(Sound::PlayerJump);
	}

	//�v���C���[���W�����v���Ă�����
	if (m_player->IsJump())
	{
		// ��������
		m_fallPlayerSpeed += 0.4f;
		// �������x���ړ��ʂɉ�����
		PlayerMoveY = m_fallPlayerSpeed;
	}

	if (m_isPlayerMoveU || m_isPlayerMoveD || m_isPlayerMoveW)
	{
		m_updateFunc = &GameplayingScene::MoveMapUpdat;
	}

	// �ړ��ʂɊ�Â��ăL�����N�^�̍��W���ړ�
	MovePlayer(PlayerMoveX, PlayerMoveY);

	m_map->Update();		//	�}�b�v�X�V
	m_player->Update();		//	�v���C���[�X�V
	//�G�l�~�[
	float MoveX = m_correction.x, MoveY = m_correction.y;
	MoveEnemy(MoveX, MoveY);
	m_enemyFactory->Update();

	m_shotFactory->Update();//�V���b�g�X�V

	//�V���b�g
	if (input.IsTriggered(InputType::shot))//shot����������e�����
	{
		createShot(m_player->GetRect().GetCenter(), true, m_player->IsLeft());
		Sound::Play(Sound::PlayeyShot);
		m_player->Action(ActionType::grah_attack);
	}

	for (int i = 0; i < kShot; i++)
	{
		if (m_shots[i]->IsExist())//���݂��Ă���e�����X�V����
		{
			m_shots[i]->Update();
			m_shots[i]->Movement({ kShotSpeed, m_correction.y });
		}
	}
	m_shotFactory->Movement({ kShotSpeed, m_correction.y });

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
				enemy->Damage(shot->AttackPower());
				Sound::Play(Sound::PlayeyShotHit);
				break;
			}
		}
	}
	//�G�̒e�ƁA�v���C���[�̓����蔻��
	if (m_player->IsCollidable())
	{
		for (auto& shot : m_shotFactory->GetShot())
		{
			if (!shot->IsExist())	continue;
			if (shot->GetPlayerShot())	continue;
			//�V���b�g�ƃv���C���[�̓����蔻��
			if (shot->GetRect().IsHit(m_player->GetRect()))
			{
				shot->SetExist(false);
				m_player->Damage(shot->AttackPower());
				Sound::Play(Sound::PlayeyShotHit);
				return;
			}
		}
	}
	//�v���C���[�ƁA�G�̓����蔻��
	if (m_player->IsCollidable())
	{
		for (auto& enemy : m_enemyFactory->GetEnemies())
		{
			if (!enemy->IsExist())	continue;
			//�G�ƃv���C���[����������
			if (enemy->GetRect().IsHit(m_player->GetRect()))
			{
				m_player->Damage(enemy->TouchAttackPower());
				Sound::Play(Sound::PlayeyShotHit);
				break;
			}
		}
	}

	//�Q�[���I�[�o�[����
	{
		//�v���C���[��HP���O�ɂȂ�����Q�[���I�[�o�[�ɂ���
		if (m_hp[Object_Player]->GetHp() <= 0)
		{
			m_updateFunc = &GameplayingScene::FadeOutUpdat;
			m_fadeColor = 0xff0000;
			m_crea = 1;
		}
		//�v���C���[�̏���W+10���炢���@death����̕����ɐG�ꂽ��
		if ((m_map->GetMapEventParam(posX - m_player->GetRect().GetSize().w * 0.5f, posY + m_player->GetRect().GetSize().h * 0.5f + 10.0f) == MapEvent_death) &&
			(m_map->GetMapEventParam(posX + m_player->GetRect().GetSize().w * 0.5f, posY + m_player->GetRect().GetSize().h * 0.5f + 10.0f) == MapEvent_death))
		{
			m_player->Action(ActionType::grah_hit);
			m_updateFunc = &GameplayingScene::FadeOutUpdat;
			m_fadeColor = 0xff0000;
			m_crea = 1;
		}
	}
	//�|�[�Y���
	if (input.IsTriggered(InputType::pause))
	{
		Sound::Play(Sound::MenuOpen);
		m_manager.PushScene(new PauseScene(m_manager));
		return;
	}
}

void GameplayingScene::MoveMapUpdat(const InputState& input)
{
	//�v���C���[�ɂ͑����āA�v���C���[�̉�ʏ�̉����W����ʂ̉����W�̈ʒu�ɂȂ�悤��
	//�}�b�v�ɂ̓v���C���[�����ɂȂ�܂œ�������������
	//��ʉ��ɗ�����

	//������ꏊ����ʂ̉��ɂȂ�悤�Ɉړ�������

	m_map->Update();
	
	float moveY = (Game::kMapNumY * Game::ChipSize) / 120.0f;
	float moveX = ((Game::kMapNumX * Game::ChipSize) / 120.0f) * -1.0f;

	//��Ɉړ�����Ƃ�
	if (m_isPlayerMoveU)
	{
		//�v���C���[�̉����W���t�B�[���h�̉����W�����������Ƃ�
		if (m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h / 2 < Game::kMapScreenBottomY)
		{
			m_map->Movement({ 0.0f,moveY });//map���ړ�������
			MoveEnemy( 0.0f,moveY);//�G�l�~�[���ړ�������
			MovePlayer(0.0f, moveY - 0.1f);//�v���C���[���ړ�������
			m_correction.y += moveY;//�ǂ̂��炢�}�b�v���ړ�������
			moveY *= -1.0f;
			m_add.y += moveY;//�ǂ̂��炢�v���C���[���ړ�������
		}
		else
		{
			m_updateFunc = &GameplayingScene::NormalUpdat;
			m_isPlayerMoveU = false;
			return;
		}
	}
	//���Ɉړ�����Ƃ�
	else if (m_isPlayerMoveD)
	{
		moveY *= -1.0f;
		//�v���C���[�̏���W���t�B�[���h�̏���W�����傫���Ƃ�
		if (m_player->GetRect().GetCenter().y - m_player->GetRect().GetSize().h / 2 > Game::kMapScreenTopY)
		{
			m_map->Movement({ 0.0f,moveY });
			MoveEnemy(0.0f, moveY);
			m_player->Movement({ 0.0f,moveY + 0.1f });
			moveY *= -1.0f;
			m_add.y += moveY;
		}
		else
		{
			m_updateFunc = &GameplayingScene::NormalUpdat;
			m_isPlayerMoveD = false;
			return;
		}
	}
	//�E�Ɉړ�
	else if (m_isPlayerMoveW)
	{
		//�v���C���[�̍����W���t�B�[���h�̍����W�����傫���Ƃ�
		if (m_player->GetRect().GetCenter().x - m_player->GetRect().GetSize().w / 2 > Game::kMapScreenLeftX)
		{
			m_map->Movement({ moveX,0.0f });
			MoveEnemy(moveX, 0.0f);
			m_player->Movement({ moveX + 0.1f,0.0f });
			moveX *= -1.0f;
			m_add.x += moveX;
		}
		else
		{
			//�{�X��Ɉړ�����O�ɃG�l�~�[�����ׂď���

			m_updateFunc = &GameplayingScene::BossUpdate;
			m_isPlayerMoveW = false;
			return;
		}
	}
}

void GameplayingScene::BossUpdate(const InputState& input)
{
	m_player->Action(ActionType::grah_idle);
	float PlayerMoveX = 0.0f, PlayerMoveY = 0.0f;//�v���C���[�̈ړ�

	//���Ɉړ�
	if (input.IsPressed(InputType::left))
	{
		if (input.IsTriggered(InputType::left))
		{
			m_player->SetLeft(true);
		}
		else
		{
			PlayerMoveX -= kPlayerMoveSpeed;

			m_player->SetLeft(true);
			m_player->Action(ActionType::grah_walk);
		}
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
			//�v���C���[����ʒ����ɂ��違�}�b�v�̕\���ł���͈͈ȓ��̎��͓�������
			if (m_isPlayerCenterLR &&
				m_map->GetMapEventParam(m_add.x + Game::kMapScreenRightX, m_add.y + Game::kMapScreenTopY) != MapEvent_screen &&
				m_map->GetMapEventParam(m_add.x + Game::kMapScreenRightX, m_add.y + Game::kMapScreenBottomY - 1.0f) != MapEvent_screen)
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
	}
	//�v���C���[�W�����v����
	if (m_player->IsJump() == false && input.IsTriggered(InputType::junp))
	{
		m_fallPlayerSpeed = -kJumpAcc;
		m_player->Action(ActionType::grah_jump);
		m_player->SetJump(true);
		Sound::Play(Sound::PlayerJump);
	}
	//�v���C���[���W�����v���Ă�����
	if (m_player->IsJump())
	{
		// ��������
		m_fallPlayerSpeed += 0.4f;
		float camera = m_map->GetPos().y + Game::kMapChipNumY * Game::ChipSize;
		// �������x���ړ��ʂɉ�����
		PlayerMoveY = m_fallPlayerSpeed;
	}
	// �ړ��ʂɊ�Â��ăL�����N�^�̍��W���ړ�
	MovePlayer(PlayerMoveX, PlayerMoveY);
	m_player->Update();		//	�v���C���[�X�V
	m_enemyFactory->Update();//�G�l�~�[�X�V
	m_shotFactory->Update();//�V���b�g�X�V

	//�V���b�g
	if (input.IsTriggered(InputType::shot))//shot����������e�����
	{
		createShot(m_player->GetRect().GetCenter(), true, m_player->IsLeft());
		Sound::Play(Sound::PlayeyShot);
		m_player->Action(ActionType::grah_attack);
	}
	for (int i = 0; i < kShot; i++)
	{
		if (m_shots[i]->IsExist())//���݂��Ă���e�����X�V����
		{
			m_shots[i]->Update();
			m_shots[i]->Movement({ kShotSpeed, m_correction.y });
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
				enemy->Damage(shot->AttackPower());
				break;
			}
		}
	}
	//�G�̒e�ƁA�v���C���[�̓����蔻��
	if (m_player->IsCollidable())
	{
		for (auto& shot : m_shotFactory->GetShot())
		{
			if (!shot->IsExist())	continue;
			if (shot->GetPlayerShot())	continue;
			//�V���b�g�ƃv���C���[�̓����蔻��
			if (shot->GetRect().IsHit(m_player->GetRect()))
			{
				shot->SetExist(false);
				m_player->Damage(shot->AttackPower());
				return;
			}
		}
	}
	//�v���C���[�ƁA�G�̓����蔻��
	if (m_player->IsCollidable())
	{
		for (auto& enemy : m_enemyFactory->GetEnemies())
		{
			if (!enemy->IsExist())	continue;
			//�G�ƃv���C���[����������
			if (enemy->GetRect().IsHit(m_player->GetRect()))
			{
				m_player->Damage(enemy->TouchAttackPower());
				break;
			}
		}
	}

	//�v���C���[��HP���O�ɂȂ�����Q�[���I�[�o�[�ɂ���
	if (m_hp[Object_Player]->GetHp() <= 0)
	{
		m_updateFunc = &GameplayingScene::FadeOutUpdat;
		m_fadeColor = 0xff0000;
		m_crea = 1;
		return;
	}
	//�G�l�~�[��HP���O�ɂȂ�����Q�[���N���A
	if (m_hp[Object_EnemyBoss]->GetHp() <= 0)
	{
		m_updateFunc = &GameplayingScene::FadeOutUpdat;
		m_fadeColor = 0xff0000;
		m_crea = 0;
		return;
	}
	//�|�[�Y���
	if (input.IsTriggered(InputType::pause))
	{
		Sound::Play(Sound::MenuOpen);
		m_manager.PushScene(new PauseScene(m_manager));
		return;
	}
}

void GameplayingScene::FadeOutUpdat(const InputState& input)
{
	m_fadeValue = 255 * m_fadeTimer / kFadeInterval;
	if(++m_fadeTimer == kFadeInterval)
	{
		switch (m_crea)
		{
		case 0:
			m_manager.ChangeScene(new GameclearScene(m_manager));
			return;
		case 1:
			m_manager.ChangeScene(new GameoverScene(m_manager));
		default:
			return;
		}
	}
}

