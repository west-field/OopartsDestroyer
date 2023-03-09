#include "GameplayingScene.h"
#include <DxLib.h>
#include <cassert>
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
#include "../Util/Graph.h"

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
	constexpr float kPlayerMoveSpeed = 4.0f;//�v���C���[�̈ړ����x
	constexpr float kJumpAcc = 9.0f;//�W�����v��
	constexpr float kShotSpeed = 8.0f;//�V���b�g�X�s�[�h

	constexpr float kPullPos = 10.0f;//��q�����͈͂����߂�
}

GameplayingScene::GameplayingScene(SceneManager& manager) :
	Scene(manager), m_updateFunc(&GameplayingScene::FadeInUpdat),m_drawFunc(&GameplayingScene::NormalDraw),
	m_add(), m_correction(), m_playerPosUp(), m_playerPosBottom()
{
	int se = 0, sh = 0, bit = 0;
	GetScreenState(&se, &sh, &bit);
	tempScreenH_ = MakeScreen(se, sh);
	assert(tempScreenH_ >= 0);

	//HP�o�[�̃O���t�B�b�N
	for (auto& hp : m_hp)
	{
		hp = std::make_shared<HpBar>();
		hp->Init(my::MyLoadGraph(L"Data/hp.bmp"));
	}
	//�v���C���[�̒e�̃O���t�B�b�N
	for (auto& shot : m_shots)
	{
		shot = std::make_shared<RockBuster>(my::MyLoadGraph(L"Data/rockBuster.png"));
	}
	//�v���C���[
	m_player = std::make_shared<Player>(Position2{(Game::kMapScreenLeftX- Game::ChipSize/2),(Game::kMapScreenBottomY - Game::ChipSize*5)},m_hp[Object_Player]);//�v���C���[�̏����ʒu
	//�G�̒e�H��
	m_shotFactory = std::make_shared<ShotFactory>();
	//�G�H��
	m_enemyFactory = std::make_shared<EnemyFactory>(m_player, m_shotFactory);//�v���C���[�ƃV���b�g�ƓG��|��������n��
	//�}�b�v
	m_map = std::make_shared<Map>(m_enemyFactory,0);
	m_map->Load(L"Data/map/map.fmf");

	//�J�n�ʒu
	//Position2 pos = { Game::kMapScreenLeftX,((Game::kMapChipNumY * Game::ChipSize) - Game::kMapScreenBottomY) * -1.0f };
	//Position2 pos = { -3026.0f,178.0f };//���ړ���q
	Position2 pos = { -5351.0f,-1235.0f };//�{�X��O
	m_map->Movement(pos);
	m_add = pos * -1.0f;
	//�w�i
	Graph::Init();

	m_BgmH = LoadSoundMem(L"Sound/Disital_Delta.mp3");
	m_bossBgm = LoadSoundMem(L"Sound/arabiantechno.mp3");
	ChangeVolumeSoundMem(0, m_BgmH);
	PlaySoundMem(m_BgmH, DX_PLAYTYPE_LOOP, true);
}

GameplayingScene::~GameplayingScene()
{
	DeleteSoundMem(m_BgmH);
	DeleteSoundMem(m_bossBgm);
	DeleteGraph(tempScreenH_);
}

void GameplayingScene::Update(const InputState& input)
{
	//�w�i���ړ�������
	Graph::BgUpdate();
	(this->*m_updateFunc)(input);
}

void GameplayingScene::Draw()
{
	//���H�p�X�N���[���n���h��
	SetDrawScreen(tempScreenH_);
	Graph::Bg();//�w�i�̈ꕔ��\��
	m_map->Draw();//�}�b�v��\��

	m_player->Draw();//�v���C���[��\��
	m_enemyFactory->Draw();//�G�l�~�[��\��
	for (auto& shot : m_shots)//�V���b�g��\��
	{
		if (shot->IsExist())
		{
			shot->Draw();
		}
	}
	m_shotFactory->Draw();//�G�V���b�g�\��

	(this->*m_drawFunc)();//HP�o�[�̕\��

	//�g�����
	DrawBox(Game::kMapScreenLeftX, Game::kMapScreenTopY, Game::kMapScreenLeftX - Game::ChipSize, Game::kMapScreenBottomY, m_framecolor, true);//����
	DrawBox(Game::kMapScreenRightX, Game::kMapScreenTopY, Game::kMapScreenRightX + Game::ChipSize, Game::kMapScreenBottomY, m_framecolor, true);//�E��
	DrawBox(Game::kMapScreenLeftX - Game::ChipSize, Game::kMapScreenTopY - Game::ChipSize, Game::kMapScreenRightX + Game::ChipSize, Game::kMapScreenTopY, m_framecolor, true);//��
	DrawBox(Game::kMapScreenLeftX - Game::ChipSize, Game::kMapScreenBottomY + Game::ChipSize, Game::kMapScreenRightX + Game::ChipSize, Game::kMapScreenBottomY, m_framecolor, true);//��

	//�|�����G�̐���\��
	//DrawFormatString(Game::kScreenWidth/2, Game::kScreenHeight/3, 0x000000, L"%d", m_enemyKill);

#ifdef _DEBUG
	for (auto& enemy : m_enemyFactory->GetEnemies())
	{
		if (!enemy->IsExist())	continue;

		DrawFormatString(500, 0, 0x00aaff, L"x%3f,y%3f", enemy->GetRect().GetCenter().x, enemy->GetRect().GetCenter().y);
	}
	
	//��q�ɏ���͈�
	DrawBoxAA(
		 m_player->GetRect().GetCenter().x - m_player->GetRect().GetSize().w / 2 + kPullPos
		,m_player->GetRect().GetCenter().y - m_player->GetRect().GetSize().h / 2 + 2.0f
		,m_player->GetRect().GetCenter().x + m_player->GetRect().GetSize().w / 2 - kPullPos
		,m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h / 2 + 1.5f
		, 0x0000ff, true);

	int num = 0;
	for (auto& shot : m_shots)//�V���b�g��\��
	{
		if (shot->IsExist())
		{
			num++;
		}
	}
	int color = 0x000000;
	DrawFormatString(150, 20, color, L"�e�̐�%d", num);
	if (m_isPlayerCenterLR)
	{
		DrawFormatString(150, 40, 0xff00ff, L"lR,true");
	}
	else
	{
		DrawFormatString(150, 40, 0xff00ff, L"lR,false");
	}
	if (m_isScreenMoveUp)
	{
		DrawFormatString(150, 60, 0xff00ff, L"uD,true");
	}
	else
	{
		DrawFormatString(150, 60, 0xff00ff, L"uD,false");
	}
	if (m_isLadder)
	{
		DrawFormatString(300, 80, 0xff00ff, L"Ladder,true");
	}
	else 
	{
		DrawFormatString(300, 80, 0xff00ff, L"Ladder,false"); 
	}
	if (m_isLadderFirst)
	{
		DrawFormatString(300, 100, 0xff00ff, L"LadderFirst,true");
	}
	else
	{
		DrawFormatString(300, 100, 0xff00ff, L"LadderFirst,false");
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

	DrawFormatString(0, 80, color, L"player.x%3f,y.%3f", m_player->GetRect().center.x+m_add.x, m_player->GetRect().center.y+m_add.y);//�v���C���[�Ƒ������W
	DrawFormatString(0, 100, color, L"player.x%3f,y.%3f", m_player->GetRect().center.x, m_player->GetRect().center.y);//�v���C���[���W
	DrawFormatString(0, 120, color, L"add.x%3f,y.%3f",m_add.x, m_add.y);//��ʂ��ǂ̂��炢�ړ�������
	DrawFormatString(0, 160, color, L"m_correction.x%3f,y.%3f", m_correction.x, m_correction.y);//��ʂ��ǂ̂��炢�ړ�������

	//�\���������}�b�v���
	DrawBox(Game::kMapScreenLeftX, Game::kMapScreenTopY, Game::kMapScreenRightX, Game::kMapScreenBottomY, color, false);
	DrawFormatString(Game::kMapScreenLeftX, Game::kMapScreenTopY, color, L"%d", Game::kMapScreenLeftX);
	DrawFormatString(Game::kMapScreenRightX, Game::kMapScreenTopY, color, L"%d",Game::kMapScreenTopY);
	DrawFormatString(Game::kMapScreenLeftX, Game::kMapScreenBottomY, color, L"%d",Game::kMapScreenBottomY);
	DrawFormatString(Game::kMapScreenRightX, Game::kMapScreenBottomY, color, L"%d",Game::kMapScreenRightX);
	//���肷��͈�
	DrawBox(Game::kMapScreenLeftX, Game::kMapScreenTopY, Game::kMapScreenRightX, Game::kMapScreenBottomY - 1, 0xffaaaa, false);


	float posX = m_add.x + m_player->GetRect().GetCenter().x;
	float posY = m_add.y + m_player->GetRect().GetCenter().y;
	//��q�̈ʒu
	Vector2 lader = m_map->GetMapChipPos(posX + m_player->GetRect().GetSize().w / 2 - kPullPos, posY);
	//�ړ��������
	Vector2 aling = { (lader.x + m_map->GetPos().x) - (m_player->GetRect().GetCenter().x - m_player->GetRect().GetSize().w / 2 - 1.0f),
						(lader.y + m_map->GetPos().y) - (m_player->GetRect().GetCenter().y) };
	DrawBoxAA(lader.x + m_map->GetPos().x, lader.y+ Game::ChipSize + m_map->GetPos().y,
		lader.x + Game::ChipSize + m_map->GetPos().x, lader.y+ Game::ChipSize + Game::ChipSize + m_map->GetPos().y, 0xaaffaa, false);//���݂���ꏊ(�}�b�v)
	DrawBoxAA(lader.x + m_map->GetPos().x, lader.y + m_map->GetPos().y,
		lader.x + Game::ChipSize + m_map->GetPos().x, lader.y + Game::ChipSize + m_map->GetPos().y, 0xaaffaa, false);//���݂���ꏊ(�}�b�v)��1���̃}�X
	DrawFormatString(500, 100, 0x000000, L"��q.x%3f,y%3f", lader.x, lader.y);
	DrawFormatString(500, 120, 0x000000, L"�ړ�.x%3f,y%3f", aling.x, aling.y);

	DrawFormatString(500, 500, 0x000000, L"playerPosUp.%3f", m_playerPosUp);
	DrawFormatString(500, 520, 0x000000, L"playerPosBottom.%3f", m_playerPosBottom);
#endif
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeValue);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, m_fadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//����ʂɖ߂�
	SetDrawScreen(DX_SCREEN_BACK);

	DrawGraph(quakeX_, 0, tempScreenH_, true);
	//if (quakeTimer_ > 0)
	//{
	//	//GraphFilter(tempScreenH_, DX_GRAPH_FILTER_GAUSS, 16, 1400);//��ʂ��ڂ���
	//	SetDrawBlendMode(DX_BLENDMODE_ADD, 192);//���Z����
	//	DrawGraph(quakeX_, 0, tempScreenH_, true);
	//	//GraphFilter(tempScreenH_, DX_GRAPH_FILTER_GAUSS, 32, 2000);//��ʂ��ڂ���
	//	//SetDrawBlendMode(DX_BLENDMODE_ADD, 192);//���Z����
	//	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//���Ƃɖ߂�
	//}
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
	// �����̃T�C�Y
	wsize = m_player->GetRect().GetSize().w * 0.5f;
	hsize = m_player->GetRect().GetSize().h * 0.5f;

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
	m_player->Movement({ 0.0f,MoveY });//�ړ�

	// ��ɍ��E�ړ����������Ń`�F�b�N
	// �����̃`�F�b�N
	MapHitCheck(m_add.x + m_player->GetRect().GetCenter().x - wsize, m_player->GetRect().GetCenter().y + hsize + m_add.y, MoveX, Dummy);

	// �E���̃`�F�b�N
	MapHitCheck(m_add.x + m_player->GetRect().GetCenter().x + wsize, m_player->GetRect().GetCenter().y + hsize + m_add.y, MoveX, Dummy);

	// ����̃`�F�b�N
	MapHitCheck(m_add.x + m_player->GetRect().GetCenter().x - wsize, m_player->GetRect().GetCenter().y - hsize + m_add.y, MoveX, Dummy);

	// �E��̃`�F�b�N
	MapHitCheck(m_add.x + m_player->GetRect().GetCenter().x + wsize, m_player->GetRect().GetCenter().y - hsize + m_add.y, MoveX, Dummy);

	m_player->Movement({ MoveX,0.0f });//�ړ�

	// �ڒn���� �L�����N�^�̍����ƉE���̉��ɒn�ʂ����邩���ׂ�
	if ((m_map->GetMapEventParam(m_add.x + m_player->GetRect().GetCenter().x - m_player->GetRect().GetSize().w * 0.5f, m_add.y + m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h * 0.5f + 1.0f) == MapEvent_hit) ||
		(m_map->GetMapEventParam(m_add.x + m_player->GetRect().GetCenter().x + m_player->GetRect().GetSize().w * 0.5f, m_add.y + m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h * 0.5f + 1.0f) == MapEvent_hit))
	{
		//�����蔻��̂���ꏊ�ɗ����特��炷
		if (m_player->IsJump())
		{
			Sound::Play(Sound::BlockMove);
		}

		//���ꂪ��������ݒu���ɂ���
		m_fallPlayerSpeed = 0.0f;
		m_isFall = false;
		m_player->SetJump(false);
		m_isLadderFirst = false;
	}
	//��q���������瑫�ꂪ���锻��ɂ���
	else if (	(m_map->GetMapEventParam(m_add.x + m_player->GetRect().GetCenter().x + m_player->GetRect().GetSize().w / 2 - kPullPos, m_add.y + m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h / 2 + 1.0f) == MapEvent_ladder)||
				(m_map->GetMapEventParam(m_add.x + m_player->GetRect().GetCenter().x - m_player->GetRect().GetSize().w / 2 + kPullPos, m_add.y + m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h / 2 + 1.0f) == MapEvent_ladder))
	{
		m_isFall = false;
		m_fallPlayerSpeed = 0.0f;
		m_player->SetJump(false);
	}
	else
	{
		//�Ȃ��Ƃ��̓W�����v���ɂ���
		m_player->SetJump(true);
	}
	
	return;
}

void GameplayingScene::MoveEnemy(float MoveX, float MoveY)
{
	float Dummy = 0.0f;
	float hsize, wsize;
	float moveX = 0.0f;
	float moveY = 1.0f;

	for (auto& enemy : m_enemyFactory->GetEnemies())
	{
		if (!enemy->IsExist())	continue;
		// �����̃T�C�Y���Z�o
		wsize = enemy->GetRect().GetSize().w * 0.5f;
		hsize = enemy->GetRect().GetSize().h * 0.5f;
		//�ړ���
		moveX = enemy->GetVec().x;

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


		//�����Ă�������œ������Ă��邩���肷��
		if (enemy->IsLeft())
		{
			enemy->GetChip(m_map->GetMapEventParam(m_add.x + enemy->GetRect().GetCenter().x-wsize - moveX, m_add.y + enemy->GetRect().GetCenter().y + hsize - moveY));
		}
		else
		{
			enemy->GetChip(m_map->GetMapEventParam(m_add.x + enemy->GetRect().GetCenter().x+wsize + moveX, m_add.y + enemy->GetRect().GetCenter().y + hsize - moveY));
		}

		
	}

	// �I��
	return;
}

void GameplayingScene::MoveBoss(float MoveX, float MoveY)
{
	float Dummy = 0.0f;
	float PosX, PosY;
	float hsize, wsize;
	float moveX = 0.0f;
	float moveY = 0.0f;

	for (auto& enemy : m_enemyFactory->GetEnemies())
	{
		if (!enemy->IsExist())	continue;

		//�v���C���[�̒��S�ʒu
		PosX = enemy->GetRect().GetCenter().x + m_add.x;
		PosY = enemy->GetRect().GetCenter().y + m_add.y;
		// �����̃T�C�Y���Z�o
		wsize = enemy->GetRect().GetSize().w * 0.5f;
		hsize = enemy->GetRect().GetSize().h * 0.5f;

		moveX = enemy->GetVec().x;
		moveY = enemy->GetVec().y;

		enemy->Movement({ MoveX,MoveY });//��ʈړ�

		// �����̃`�F�b�N�A�����u���b�N�̏�ӂɒ����Ă����痎�����~�߂�
		if (MapHitCheck(PosX - wsize , PosY + hsize , Dummy, moveY) == 3)
		{
			m_fallPlayerSpeed = 0.0f;
		}
		// �E���̃`�F�b�N�A�����u���b�N�̏�ӂɒ����Ă����痎�����~�߂�
		if (MapHitCheck(PosX + wsize, PosY + hsize , Dummy, moveY) == 3)
		{
			m_fallPlayerSpeed = 0.0f;
		}
		// ����̃`�F�b�N�A�����u���b�N�̉��ӂɓ������Ă����痎��������
		if (MapHitCheck(PosX - wsize , PosY - hsize , Dummy, moveY) == 4)
		{
			m_fallPlayerSpeed *= -1.0f;
		}
		// �E��̃`�F�b�N�A�����u���b�N�̉��ӂɓ������Ă����痎��������
		if (MapHitCheck(PosX + wsize , PosY - hsize , Dummy, moveY) == 4)
		{
			m_fallPlayerSpeed *= -1.0f;
		}
		enemy->EnemyMovement({ 0.0f,moveY });

		//�W�����v���Ă���Ƃ��������Ɉړ�����
		if (enemy->IsJump())
		{
			// ��ɍ��E�ړ����������Ń`�F�b�N
			// �����̃`�F�b�N
			MapHitCheck(PosX - wsize, PosY + hsize , moveX, Dummy);

			// �E���̃`�F�b�N
			MapHitCheck(PosX + wsize, PosY + hsize , moveX, Dummy);

			// ����̃`�F�b�N
			MapHitCheck(PosX - wsize, PosY - hsize , moveX, Dummy);

			// �E��̃`�F�b�N
			MapHitCheck(PosX + wsize, PosY - hsize , moveX, Dummy);
			//���E�ړ����������Z
			enemy->EnemyMovement({ moveX,0.0f });
		}
		
		// �ڒn���� �L�����N�^�̍����ƉE���̉��ɒn�ʂ����邩���ׂ�
		//�����蔻��̂���ꏊ�ɗ����特��炵�đ��ꂪ���锻��ɂ���
		if ((m_map->GetMapEventParam(PosX - wsize, PosY + hsize + 1.0f) == MapEvent_hit) ||
			(m_map->GetMapEventParam(PosX + wsize, PosY + hsize + 1.0f) == MapEvent_hit))
		{
			//���ꂪ��������ݒu���ɂ���
			enemy->SetJump(false);
		}
		else
		{
			//�Ȃ��Ƃ��̓W�����v���ɂ���
			enemy->SetJump(true);
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

void GameplayingScene::ScreenMove()
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
	m_isScreenMoveUp = false;
	m_isScreenMoveDown = false;
	m_isScreenMoveWidth = false;

	if (m_player->GetRect().GetCenter().x > fieldCenterLeftUp.x && m_player->GetRect().GetCenter().x < fieldCenterRightBottom.x)
	{
		m_isPlayerCenterLR = true;
	}

	//�c�ړ��ł���ꏊ�ɂ�����ړ������� �E���̉��Ə�
	if ((m_map->GetMapEventParam(m_add.x + m_player->GetRect().GetCenter().x + m_player->GetRect().GetSize().w / 2 - kPullPos, m_add.y + m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h / 2 + 1.0f) == MapEvent_screenMoveW) &&
		(m_map->GetMapEventParam(m_add.x + m_player->GetRect().GetCenter().x + m_player->GetRect().GetSize().w / 2 - kPullPos, m_add.y + m_player->GetRect().GetCenter().y - m_player->GetRect().GetSize().h / 2 + 2.0f) == MapEvent_screenMoveW))
	{
		m_isScreenMoveWidth = true;
	}
	//�v���C���[�̏���W���t�B�[���h�̏�ɓ������Ă�����ړ��ł���
	if (m_player->GetRect().GetCenter().y < fieldCenterRightBottom.y &&
		(m_map->GetMapEventParam(m_add.x + m_player->GetRect().GetCenter().x - m_player->GetRect().GetSize().w / 2, m_add.y + m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h / 2 + 1.0f) == MapEvent_screenMoveU)&&
		(m_map->GetMapEventParam(m_add.x + m_player->GetRect().GetCenter().x + m_player->GetRect().GetSize().w / 2 - kPullPos, m_add.y + m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h / 2 + 1.0f) == MapEvent_screenMoveU))
	{
		m_isScreenMoveUp = true;
	}
	//�v���C���[�̉����W���c�ړ��ł���ꏊ�ɂ�����ړ�������
	else if (/*!m_isScreenMoveUp &&*/
		(m_map->GetMapEventParam(m_add.x + m_player->GetRect().GetCenter().x - m_player->GetRect().GetSize().w / 2, m_add.y + m_player->GetRect().GetCenter().y - m_player->GetRect().GetSize().h / 2 - 1.0f) == MapEvent_screenMoveD) &&
		(m_map->GetMapEventParam(m_add.x + m_player->GetRect().GetCenter().x + m_player->GetRect().GetSize().w / 2 - kPullPos, m_add.y + m_player->GetRect().GetCenter().y - m_player->GetRect().GetSize().h / 2 - 1.0f) == MapEvent_screenMoveD))
	{
		m_isScreenMoveDown = true;
	}

	if (!m_isScreenMoveUp && !m_isScreenMoveDown && !m_isScreenMoveWidth)
	{
		m_isFirst = false;
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

void GameplayingScene::Ladder(const InputState& input)
{
	float posX = m_add.x + m_player->GetRect().GetCenter().x;
	float posY = m_add.y + m_player->GetRect().GetCenter().y;
	float PlayerMoveY = 0.0f;

	LadderAlign();
	Vector2 lader = m_map->GetMapChipPos(posX + m_player->GetRect().GetSize().w / 2 - kPullPos, posY);
	//��L�[�Œ�q���オ���
	if (input.IsPressed(InputType::up))
	{
		//���ɒ�q�@�@�E-10�Ɖ�+1 ��-10�Ɖ�+1
		if ((m_map->GetMapEventParam(lader.x, lader.y) == MapEvent_screenMoveU) ||
			(m_map->GetMapEventParam(lader.x, lader.y) == MapEvent_ladder))
		{
			LadderAlign();//��q�ƃv���C���[�̈ʒu�����킹��

			m_isLadder = true;//��q�ɏ���Ă���
			
			PlayerMoveY = 0.0f;//�ړ��ʏ�����
			PlayerMoveY -= kPlayerMoveSpeed;//��������Ă���Ƃ��͏�Ɉړ�����
		}

		//���݂̈ʒu����q�ł͂Ȃ��Ƃ�������Ɉړ�����
		if (m_isLadder && m_map->GetMapEventParam(lader.x, lader.y) == MapEvent_no)
		{
			m_isLadder = false;
			Vector2 aling = { (lader.x + m_map->GetPos().x) - (m_player->GetRect().GetCenter().x - m_player->GetRect().GetSize().w / 2 - 1.0f),
					(lader.y + m_map->GetPos().y) - (m_player->GetRect().GetCenter().y) };//�ړ��������
			MovePlayer(0.0f, aling.y);//�ړ�
		}
	}
	//���L�[�Œ�q���������@
	else if (input.IsPressed(InputType::down))
	{
		//�㉺�ǂ��炩�ɒ�q������Ƃ�
		if ((m_map->GetMapEventParam(lader.x, lader.y) == MapEvent_ladder) ||
			(m_map->GetMapEventParam(lader.x, lader.y) == MapEvent_screenMoveD) ||
			(m_map->GetMapEventParam(lader.x, lader.y + Game::ChipSize) == MapEvent_ladder)||
			(m_map->GetMapEventParam(lader.x, lader.y + Game::ChipSize) == MapEvent_screenMoveD))
		{
			m_isLadder = true;
			PlayerMoveY = 0.0f;
			PlayerMoveY += kPlayerMoveSpeed;
		}
		if (m_isLadder && m_map->GetMapEventParam(lader.x, lader.y + Game::ChipSize) != MapEvent_ladder)
		{
			m_isLadder = false;
			PlayerMoveY = 0.0f;
			PlayerMoveY += kPlayerMoveSpeed;
		}
	}
	else
	{
		m_isFall = true;
	}

	MovePlayer(0.0f, PlayerMoveY);
}

void GameplayingScene::LadderAlign()
{
	if (!m_isLadder)	return;

	float posX = m_add.x + m_player->GetRect().GetCenter().x;
	float posY = m_add.y + m_player->GetRect().GetCenter().y;
	//�v���C���[�̈ʒu���q�̈ʒu�ɂ���
	//�ړ�����ʁ��v���C���[�̈ʒu�[��q�̈ʒu
	//��q�̈ʒu
	Vector2 lader = m_map->GetMapChipPos(posX + m_player->GetRect().GetSize().w / 2 - kPullPos, posY);
	//�ړ��������
	Vector2 aling = { (lader.x + m_map->GetPos().x) - (m_player->GetRect().GetCenter().x - m_player->GetRect().GetSize().w / 2 - 1.0f),
						(lader.y + m_map->GetPos().y) - (m_player->GetRect().GetCenter().y) };
	//�ړ�
	MovePlayer(aling.x, 0.0f);
}

void GameplayingScene::PlayerOnScreen(const InputState& input)
{
	float PlayerMoveY = 0.0f, PlayerMoveX = kPlayerMoveSpeed / 2;
	m_player->Update();

	if (!m_isLadder && !m_player->IsJump()&&!m_isFall)
	{
		PlayerMoveY -= 2.0f;
		m_player->Action(ActionType::grah_jump);
		m_player->SetJump(true);
		Sound::Play(Sound::PlayerJump);
	}
	//�v���C���[���W�����v���Ă�����
	if (m_player->IsJump() && !m_isLadder)
	{
		// ��������
		m_fallPlayerSpeed += 0.4f;
		// �������x���ړ��ʂɉ�����
		PlayerMoveY = m_fallPlayerSpeed;
	}

	// �ړ��ʂɊ�Â��ăL�����N�^�̍��W���ړ�
	MovePlayer(PlayerMoveX, PlayerMoveY);

	if (!m_player->IsJump())
	{
		m_updateFunc = &GameplayingScene::NormalUpdat;
	}
}

void GameplayingScene::FadeInUpdat(const InputState& input)
{
	m_fadeValue = 255 * m_fadeTimer / kFadeInterval;
	ChangeVolumeSoundMem(255 - m_fadeValue, m_BgmH);
	if (--m_fadeTimer == 0)
	{
		m_updateFunc = &GameplayingScene::PlayerOnScreen;
		m_fadeValue = 0;
		return;
	}
}

void GameplayingScene::NormalUpdat(const InputState& input)
{
	ScreenMove();//�v���C���[���Z���^�[�ɋ��邩�ǂ���
	//Button::Update();

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
				m_map->GetMapEventParam(m_add.x + Game::kMapScreenRightX, m_add.y + Game::kMapScreenTopY) != MapEvent_screen &&
				m_map->GetMapEventParam(m_add.x + Game::kMapScreenRightX, m_add.y + Game::kMapScreenBottomY - 1.0f) != MapEvent_screen &&
				m_map->GetMapEventParam(m_add.x + Game::kMapScreenRightX, m_add.y + Game::kMapScreenTopY) != MapEvent_pause &&
				m_map->GetMapEventParam(m_add.x + Game::kMapScreenRightX, m_add.y + Game::kMapScreenBottomY - 1.0f) != MapEvent_pause)
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

	//��q�ړ�
	Ladder(input);
	
	//�v���C���[�W�����v����
	if (!m_isFall&&!m_isLadder && !m_player->IsJump() && input.IsTriggered(InputType::junp))
	{
		//Button::PushButton(Button::ButtonType_A);

		m_fallPlayerSpeed = -kJumpAcc;
		m_player->Action(ActionType::grah_jump);
		m_player->SetJump(true);
		Sound::Play(Sound::PlayerJump);
	}

	//�v���C���[���W�����v���Ă�����
	if (m_player->IsJump() && !m_isLadder)
	{
		// ��������
		m_fallPlayerSpeed += 0.4f;
		// �������x���ړ��ʂɉ�����
		PlayerMoveY = m_fallPlayerSpeed;
	}

	if (m_isScreenMoveUp || m_isScreenMoveDown || m_isScreenMoveWidth)
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
			if (!enemy->IsCollidable())continue;//�����蔻��ΏۂɂȂ��Ă��Ȃ������瓖����Ȃ�
			//�G�ɒe���q�b�g����
			if (shot->GetRect().IsHit(enemy->GetRect()))
			{
				shot->SetExist(false);
				enemy->Damage(shot->AttackPower());
				Sound::Play(Sound::EnemyHit);
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
			if (shot->GetRect().IsHit(m_player->GetRect()))
			{
				shot->SetExist(false);
				m_player->Damage(shot->AttackPower());
				Sound::Play(Sound::PlayeyHit);
				quakeX_ = 5.0f;
				quakeTimer_ = 40;
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
			if (!enemy->IsCollidable())continue;
			if (enemy->GetRect().IsHit(m_player->GetRect()))
			{
				m_player->Damage(enemy->TouchAttackPower());
				Sound::Play(Sound::PlayeyHit);
				quakeX_ = 5.0f;
				quakeTimer_ = 40;
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
			return;
		}
		float posX = m_add.x + m_player->GetRect().GetCenter().x;
		float posY = m_add.y + m_player->GetRect().GetCenter().y;
		//�v���C���[�̏���W+10���炢���@death����̕����ɐG�ꂽ��
		if ((m_map->GetMapEventParam(posX - m_player->GetRect().GetSize().w * 0.5f, posY - m_player->GetRect().GetSize().h - 10.0f) == MapEvent_death) &&
			(m_map->GetMapEventParam(posX + m_player->GetRect().GetSize().w * 0.5f, posY - m_player->GetRect().GetSize().h - 10.0f) == MapEvent_death))
		{
			m_updateFunc = &GameplayingScene::FadeOutUpdat;
			m_fadeColor = 0xff0000;
			m_crea = 1;
			return;
		}
	}
	//�|�[�Y���
	if (input.IsTriggered(InputType::pause))
	{
		//Button::PushButton(Button::ButtonType_BACK);
		Sound::Play(Sound::MenuOpen);
		m_manager.PushScene(new PauseScene(m_manager));
		return;
	}

	if (quakeTimer_ > 0)
	{
		quakeX_ = -quakeX_;
		quakeX_ *= 0.95f;
		quakeTimer_--;
	}
	else
	{
		quakeX_ = 0.0f;
	}
}

void GameplayingScene::MoveMapUpdat(const InputState& input)
{
	//�v���C���[�̍����W�Ɖ����W�Ə���W
	float playerPosLeft = 0.0f;
	//�ړ�����O�ɂ��ׂď���
	if (!m_isFirst)
	{
		m_isFirst = true;
		//�G�l�~�[���폜����
		for (auto& enemy : m_enemyFactory->GetEnemies())
		{
			if (!enemy->IsExist())	continue;
			enemy->SetExist(false);
		}
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
		m_correction = { 0.0f,0.0f };
		m_playerPosUp =(m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h / 2 );
		m_playerPosBottom= (m_player->GetRect().GetCenter().y - m_player->GetRect().GetSize().h / 2 );
	}

	//������ꏊ����ʂ̉��ɂȂ�悤�Ɉړ�������
	m_map->Update();
	
	float moveY = (Game::kMapNumY * Game::ChipSize) / 120.0f;
	float moveX = ((Game::kMapNumX * Game::ChipSize) / 120.0f) * -1.0f;

	//��Ɉړ�����Ƃ�
	if (m_isScreenMoveUp)
	{
		//�v���C���[�̉����W���t�B�[���h�̉����W�����������Ƃ�
		if (m_playerPosBottom+ m_correction.y < Game::kMapScreenBottomY)
		{
			m_map->Movement({ 0.0f,moveY });//map���ړ�������
			MoveEnemy( 0.0f,moveY);//�G�l�~�[���ړ�������
			MovePlayer(0.0f, moveY - 0.5f);//�v���C���[���ړ�������
			m_correction.y += moveY;
			moveY *= -1.0f;
			m_add.y += moveY;//�ǂ̂��炢�v���C���[���ړ�������
		}
		else
		{
			m_player->Movement({ 0.0f,0.1f });
			m_updateFunc = &GameplayingScene::NormalUpdat;
			m_isScreenMoveUp = false;
			return;
		}
	}
	//���Ɉړ�����Ƃ�
	else if (m_isScreenMoveDown)
	{
		moveY *= -1.0f;
		//�v���C���[�̏���W���t�B�[���h�̏���W�����傫���Ƃ�
		if (m_playerPosUp+ m_correction.y > Game::kMapScreenTopY)
		{
			m_map->Movement({ 0.0f,moveY });
			MoveEnemy(0.0f, moveY);
			m_player->Movement({ 0.0f,moveY + 0.5f });
			m_correction.y += moveY;
			moveY *= -1.0f;
			m_add.y += moveY;
		}
		else
		{
			m_player->Movement({ 0.0f,0.1f });
			m_updateFunc = &GameplayingScene::NormalUpdat;
			m_isScreenMoveDown = false;
			return;
		}
	}
	//�E�Ɉړ�
	else if (m_isScreenMoveWidth)
	{
		//�v���C���[�̍����W���t�B�[���h�̍����W�����傫���Ƃ�
		//if (m_playerPosBottom > Game::kMapScreenLeftX)
		//�[�ɂ��Ă��Ȃ��Ƃ��͈ړ��ł���
		if((m_map->GetMapEventParam(m_add.x + Game::kMapScreenRightX, m_add.y - Game::kMapScreenTopY) != MapEvent_screen &&
			m_map->GetMapEventParam(m_add.x + Game::kMapScreenRightX, m_add.y + Game::kMapScreenBottomY - 1.0f) != MapEvent_screen))
		{
			m_map->Movement({ moveX,0.0f });
			MoveBoss(moveX, 0.0f);
			m_player->Movement({ moveX + 0.5f,0.0f });
			moveX *= -1.0f;
			m_add.x += moveX;
		}
		else
		{
			m_soundVolume -= 4;
			if (m_soundVolume <= 0)
			{
				m_soundVolume = 0;
				//�{�X��
				m_updateFunc = &GameplayingScene::BossUpdate;
				m_drawFunc = &GameplayingScene::BossDraw;
				m_isScreenMoveWidth = false;
				//�{�X��BGM�Ɠ���ւ���
				StopSoundMem(m_BgmH);
				ChangeVolumeSoundMem(0, m_bossBgm);
				PlaySoundMem(m_bossBgm, DX_PLAYTYPE_LOOP, true);
				return;
			}
			ChangeVolumeSoundMem(m_soundVolume, m_BgmH);
		}
	}
}

void GameplayingScene::BossUpdate(const InputState& input)
{
	if (m_soundVolume++ >= 255)
	{
		m_soundVolume = 255;
	}
	else
	{
		ChangeVolumeSoundMem(m_soundVolume, m_bossBgm);
	}
	ScreenMove();
	//Button::Update();
	float PlayerMoveX = 0.0f, PlayerMoveY = 0.0f;//�v���C���[�̈ړ�
	m_correction = { 0.0f,0.0f };
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
				m_map->GetMapEventParam(m_add.x + Game::kMapScreenRightX, m_add.y + Game::kMapScreenBottomY - 1.0f) != MapEvent_screen &&
				m_map->GetMapEventParam(m_add.x + Game::kMapScreenRightX, m_add.y + Game::kMapScreenTopY) != MapEvent_pause &&
				m_map->GetMapEventParam(m_add.x + Game::kMapScreenRightX, m_add.y + Game::kMapScreenBottomY - 1.0f) != MapEvent_pause)
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
	if (!m_player->IsJump() && input.IsTriggered(InputType::junp))
	{
		//Button::PushButton(Button::ButtonType_A);
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
	//�G�l�~�[
	MoveBoss(m_correction.x, m_correction.y);
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
			if (!enemy->IsCollidable())continue;//�����蔻��ΏۂɂȂ��Ă��Ȃ������瓖����Ȃ�
			//�G�ɒe���q�b�g����
			if (shot->GetRect().IsHit(enemy->GetRect()))
			{
				shot->SetExist(false);
				//enemy->Damage(shot->AttackPower());
				m_hp[Object_EnemyBoss]->Damage(shot->AttackPower());
				enemy->Damage(shot->AttackPower());
				Sound::Play(Sound::EnemyHit);
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
				Sound::Play(Sound::PlayeyHit);
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
			if (!enemy->IsCollidable())continue;//�����蔻��ΏۂɂȂ��Ă��Ȃ������瓖����Ȃ�
			//�G�ƃv���C���[����������
			if (enemy->GetRect().IsHit(m_player->GetRect()))
			{
				m_player->Damage(enemy->TouchAttackPower());
				Sound::Play(Sound::PlayeyHit);
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
	for (auto& enemy : m_enemyFactory->GetEnemies())
	{
		if (!enemy->IsExist())
		{
			m_updateFunc = &GameplayingScene::FadeOutUpdat;
			m_fadeColor = 0x000000;
			m_crea = 0;
			return;
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

void GameplayingScene::FadeOutUpdat(const InputState& input)
{
	m_fadeValue = 255 * m_fadeTimer / kFadeInterval;
	ChangeVolumeSoundMem(255 - m_fadeValue, m_BgmH);
	ChangeVolumeSoundMem(255 - m_fadeValue, m_bossBgm);
	if(++m_fadeTimer == kFadeInterval)
	{
		switch (m_crea)
		{
		case 0:
			m_manager.ChangeScene(new GameclearScene(m_manager, m_player));
			return;
		case 1:
			m_manager.ChangeScene(new GameoverScene(m_manager,m_player));
		default:
			return;
		}
	}
}

void GameplayingScene::NormalDraw()
{
	m_hp[Object_Player]->Draw(true);//HP�o�[��\��
}

void GameplayingScene::BossDraw()
{
	//HP�o�[��\��
	m_hp[Object_Player]->Draw(true);
	m_hp[Object_EnemyBoss]->Draw(false);
}

