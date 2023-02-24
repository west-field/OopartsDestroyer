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
スーパーカッター　四角い機械の穴から大量に出てくるはさみ。HP5、攻撃力4
スクリュードライバー　自機が近付くと、地面から出てきて、５方向同時に弾を二回発射する
*/
namespace
{
	constexpr float kPlayerMoveSpeed = 5.0f;//プレイヤーの移動速度
	constexpr float kJumpAcc = 10.0f;//ジャンプ力
	constexpr float kShotSpeed = 8.0f;//ショットスピード

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

	m_player = std::make_shared<Player>(Position2{(Game::kMapScreenLeftX + Game::ChipSize*8),(Game::kMapScreenBottomY - Game::ChipSize*5)},m_hp[Object_Player]);//プレイヤーの初期位置
	
	m_shotFactory = std::make_shared<ShotFactory>();

	m_enemyFactory = std::make_shared<EnemyFactory>(m_player, m_shotFactory);//プレイヤーとショットを渡す

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
	m_enemyFactory->Draw();//エネミーを表示
	int num = 0;
	for (auto& shot : m_shots)//ショットを表示
	{
		if (shot->IsExist())
		{
			shot->Draw();
			num++;
		}
	}
	m_shotFactory->Draw();//ショット表示

	m_hp[Object_Player]->Draw(true);//HPバーを表示

	//枠を作る
	DrawBox(Game::kMapScreenLeftX, Game::kMapScreenTopY, Game::kMapScreenLeftX - Game::ChipSize, Game::kMapScreenBottomY, m_framecolor, true);//左側
	DrawBox(Game::kMapScreenRightX, Game::kMapScreenTopY, Game::kMapScreenRightX + Game::ChipSize, Game::kMapScreenBottomY, m_framecolor, true);//右側
	DrawBox(Game::kMapScreenLeftX - Game::ChipSize, Game::kMapScreenTopY - Game::ChipSize, Game::kMapScreenRightX + Game::ChipSize, Game::kMapScreenTopY, m_framecolor, true);//上
	DrawBox(Game::kMapScreenLeftX - Game::ChipSize, Game::kMapScreenBottomY + Game::ChipSize, Game::kMapScreenRightX + Game::ChipSize, Game::kMapScreenBottomY, m_framecolor, true);//下

#ifdef _DEBUG
	//梯子に上れる範囲
	DrawBoxAA(m_player->GetRect().GetCenter().x - m_player->GetRect().GetSize().w / 2 + kPullPos, m_player->GetRect().GetCenter().y - m_player->GetRect().GetSize().h / 2 - 1.0f,
		m_player->GetRect().GetCenter().x + m_player->GetRect().GetSize().w / 2 - kPullPos, m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h / 2 + 1.0f, 0xffffff, false);

	DrawFormatString(150, 20, 0xffffff, L"弾の数%d", num);
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
	DrawBoxAA(fieldCenterLeftUp.x, fieldCenterLeftUp.y, fieldCenterRightBottom.x, fieldCenterRightBottom.y, 0xaaffaa, false);//画面の中心位置

	DrawFormatString(0, 80, 0xffffff, L"player.x%3f,y.%3f", m_player->GetRect().center.x+m_add.x, m_player->GetRect().center.y+m_add.y);//プレイヤーと足す座標
	DrawFormatString(0, 100, 0xffffff, L"player.x%3f,y.%3f", m_player->GetRect().center.x, m_player->GetRect().center.y);//プレイヤー座標
	DrawFormatString(0, 120, 0xffffff, L"add.x%3f,y.%3f",m_add.x, m_add.y);//画面がどのくらい移動したか
	DrawFormatString(0, 160, 0xffffff, L"m_correction.x%3f,y.%3f", m_correction.x, m_correction.y);//画面がどのくらい移動したか

	//表示したいマップ画面
	DrawBox(Game::kMapScreenLeftX, Game::kMapScreenTopY, Game::kMapScreenRightX, Game::kMapScreenBottomY, 0xffffff, false);
	DrawFormatString(Game::kMapScreenLeftX, Game::kMapScreenTopY, 0xffffff, L"%d", Game::kMapScreenLeftX);
	DrawFormatString(Game::kMapScreenRightX, Game::kMapScreenTopY, 0xffffff, L"%d",Game::kMapScreenTopY);
	DrawFormatString(Game::kMapScreenLeftX, Game::kMapScreenBottomY, 0xffffff, L"%d",Game::kMapScreenBottomY);
	DrawFormatString(Game::kMapScreenRightX, Game::kMapScreenBottomY, 0xffffff, L"%d",Game::kMapScreenRightX);
	//判定する範囲
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

	// キャラクタの左上、右上、左下、右下部分が当たり判定のあるマップに衝突しているか調べ、衝突していたら補正する
	// 半分のサイズを算出
	wsize = m_player->GetRect().GetSize().w * 0.5f;
	hsize = m_player->GetRect().GetSize().h * 0.5f;

	// 左下のチェック、もしブロックの上辺に着いていたら落下を止める 左位置、下位置
	if (MapHitCheck(m_player->GetRect().GetCenter().x - wsize + m_add.x, m_player->GetRect().GetCenter().y + hsize + m_add.y, Dummy, MoveY) == 3)
	{

		m_fallPlayerSpeed = 0.0f;
	}

	// 右下のチェック、もしブロックの上辺に着いていたら落下を止める 右位置、下位置
	if (MapHitCheck(m_player->GetRect().GetCenter().x + wsize + m_add.x, m_player->GetRect().GetCenter().y + hsize + m_add.y, Dummy, MoveY) == 3)
	{
		m_fallPlayerSpeed = 0.0f;
	}

	// 左上のチェック、もしブロックの下辺に当たっていたら落下させる 左位置、上位置
	if (MapHitCheck(m_player->GetRect().GetCenter().x - wsize + m_add.x, m_player->GetRect().GetCenter().y - hsize + m_add.y, Dummy, MoveY) == 4)
	{
		m_fallPlayerSpeed *= -1.0f;
	}

	// 右上のチェック、もしブロックの下辺に当たっていたら落下させる 右位置、上位置
	if (MapHitCheck(m_player->GetRect().GetCenter().x + wsize + m_add.x, m_player->GetRect().GetCenter().y - hsize + m_add.y, Dummy, MoveY) == 4)
	{
		m_fallPlayerSpeed *= -1.0f;
	}

	m_player->Movement({ 0.0f,MoveY });

	// 後に左右移動成分だけでチェック
	// 左下のチェック
	MapHitCheck(m_add.x + m_player->GetRect().GetCenter().x - wsize, m_player->GetRect().GetCenter().y + hsize + m_add.y, MoveX, Dummy);

	// 右下のチェック
	MapHitCheck(m_add.x + m_player->GetRect().GetCenter().x + wsize, m_player->GetRect().GetCenter().y + hsize + m_add.y, MoveX, Dummy);

	// 左上のチェック
	MapHitCheck(m_add.x + m_player->GetRect().GetCenter().x - wsize, m_player->GetRect().GetCenter().y - hsize + m_add.y, MoveX, Dummy);

	// 右上のチェック
	MapHitCheck(m_add.x + m_player->GetRect().GetCenter().x + wsize, m_player->GetRect().GetCenter().y - hsize + m_add.y, MoveX, Dummy);

	// 左右移動成分を加算
	m_player->Movement({ MoveX,0.0f });

	// 接地判定 キャラクタの左下と右下の下に地面があるか調べる
	//当たり判定のある場所に来たら音を鳴らして足場がある判定にする
	if ((m_map->GetMapEventParam(m_add.x + m_player->GetRect().GetCenter().x - m_player->GetRect().GetSize().w * 0.5f, m_add.y + m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h * 0.5f + 1.0f) == MapEvent_hit) ||
		(m_map->GetMapEventParam(m_add.x + m_player->GetRect().GetCenter().x + m_player->GetRect().GetSize().w * 0.5f, m_add.y + m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h * 0.5f + 1.0f) == MapEvent_hit))
	{
		if (m_player->IsJump())	Sound::Play(Sound::BlockMove);
		//足場があったら設置中にする
		m_player->SetJump(false);
	}
	//梯子があったら足場がある判定にする
	else if ((m_map->GetMapEventParam(m_add.x + m_player->GetRect().GetCenter().x - m_player->GetRect().GetSize().w * 0.5f, m_add.y + m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h * 0.5f + 1.0f) == MapEvent_ladder)||
		(m_map->GetMapEventParam(m_add.x + m_player->GetRect().GetCenter().x + m_player->GetRect().GetSize().w * 0.5f, m_add.y + m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h * 0.5f + 1.0f) == MapEvent_ladder))
	{
		m_player->SetJump(false);
	}
	else
	{
		//ないときはジャンプ中にする
		m_player->SetJump(true);
	}
	// 終了
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
		// 半分のサイズを算出
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

		//画面の左端に消えたら
		if (enemy->GetRect().GetCenter().x + wsize < Game::kMapScreenLeftX)
		{
			enemy->SetExist(false);
			break;
		}
		//画面の下端に消えたら
		if (enemy->GetRect().GetCenter().y - hsize > Game::kMapScreenBottomY)
		{
			enemy->SetExist(false);
			break;
		}
		//画面の上端に消えたら
		else if (enemy->GetRect().GetCenter().y + hsize < Game::kMapScreenTopY)
		{
			enemy->SetExist(false);
			break;
		}
	}

	// 終了
	return;
}

int GameplayingScene::MapHitCheck(float X, float Y, float& MoveX, float& MoveY)
{
	float afterX, afterY;
	//移動量を足す
	afterX = X + MoveX;
	afterY = Y + MoveY;
	float blockLeftX = 0.0f, blockTopY = 0.0f, blockRightX = 0.0f, blockBottomY = 0.0f;

	//整数値へ変換
	int noX = static_cast<int>(afterX / Game::ChipSize);
	int noY = static_cast<int>(afterY / Game::ChipSize);

	//当たっていたら壁から離す処理を行う、ブロックの左右上下の座標を算出
	blockLeftX = static_cast<float>(noX * Game::ChipSize);//左　X座標
	blockRightX = static_cast<float>((noX + 1) * Game::ChipSize);//右　X座標
	blockTopY = static_cast<float>(noY * Game::ChipSize);//上　Y座標
	blockBottomY = static_cast<float>((noY + 1) * Game::ChipSize);//下　Y座標

	int mapchip = m_map->GetMapEventParam(afterX, afterY);
	//当たり判定のあるブロックに当たっているか
	if (mapchip  == MapEvent_hit)
	{
		//ブロックの上に当たっていた場合
		if (MoveY > 0.0f)
		{
			//移動量を補正する
			MoveY = blockTopY - Y  - 1.0f;
			//上辺に当たったと返す
			return 3;
		}
		//ブロックの下に当たっていた場合
		if (MoveY < 0.0f)
		{
			MoveY = blockBottomY - Y + 1.0f;
			//下辺に当たったと返す
			return 4;
		}
		//ブロックの左に当たっていた場合
		if (MoveX > 0.0f)
		{
			MoveX = blockLeftX - X - 1.0f;
			//左辺に当たったと返す
			return 1;
		}
		//ブロックの右に当たっていた場合
		if (MoveX < 0.0f)
		{
			MoveX = blockRightX - X + 1.0f;
			//右辺に当たったと返す
			return 2;
		}
		//ここに来たら適当な値を返す
		return 4;
	}
	//どこにも当たらなかったと返す
	return 0;
}

void GameplayingScene::PlayerCenter()
{
	Position2 fieldCenterLeftUp =//フィールドの中心位置（左上）
	{
		static_cast<float>(Game::kNumX / 2 * Game::ChipSize),
		static_cast<float>(Game::kNumY / 2 * Game::ChipSize)
	};
	Position2 fieldCenterRightBottom =//フィールドの中心位置（右下）
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
	//縦移動できる場所についたら移動させる 右側の下と上
	if ((m_map->GetMapEventParam(m_add.x + m_player->GetRect().GetCenter().x + m_player->GetRect().GetSize().w / 2 - kPullPos, m_add.y + m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h / 2 + 1.0f) == MapEvent_screenMoveW) &&
		(m_map->GetMapEventParam(m_add.x + m_player->GetRect().GetCenter().x + m_player->GetRect().GetSize().w / 2 - kPullPos, m_add.y + m_player->GetRect().GetCenter().y - m_player->GetRect().GetSize().h / 2 + 2.0f) == MapEvent_screenMoveW))
	{
		m_isPlayerMoveW = true;
	}
	//プレイヤーの上座標がフィールドの上に当たっていたら移動できる	
	if ((m_map->GetMapEventParam(m_add.x + m_player->GetRect().GetCenter().x - m_player->GetRect().GetSize().w / 2, m_add.y + m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h / 2 + 1.0f) == MapEvent_screenMoveU)&&
		(m_map->GetMapEventParam(m_add.x + m_player->GetRect().GetCenter().x + m_player->GetRect().GetSize().w / 2 - kPullPos, m_add.y + m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h / 2 + 2.0f) == MapEvent_screenMoveU))
	{
		m_isPlayerMoveU = true;
	}
	//プレイヤーの下座標が縦移動できる場所についたら移動させる
	if (!m_isPlayerMoveU &&
		(m_map->GetMapEventParam(m_add.x + m_player->GetRect().GetCenter().x - m_player->GetRect().GetSize().w / 2, m_add.y + m_player->GetRect().GetCenter().y - m_player->GetRect().GetSize().h / 2 - 1.0f) == MapEvent_screenMoveD) &&
		(m_map->GetMapEventParam(m_add.x + m_player->GetRect().GetCenter().x + m_player->GetRect().GetSize().w / 2 - kPullPos, m_add.y + m_player->GetRect().GetCenter().y - m_player->GetRect().GetSize().h / 2 - 2.0f) == MapEvent_screenMoveD))
	{
		m_isPlayerMoveD = true;
	}

	if (m_isPlayerMoveU || m_isPlayerMoveD || m_isPlayerMoveW)
	{
		//ショットを削除する
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

	// 左下のチェック、もしブロックの上辺に着いていたら落下を止める
	if (MapHitCheck(m_player->GetRect().GetCenter().x - wsize + m_add.x, m_player->GetRect().GetCenter().y + hsize + m_add.y, Dummy, MoveY) == 3)
	{
		m_fallPlayerSpeed = 0.0f;
	}
	// 右下のチェック、もしブロックの上辺に着いていたら落下を止める
	if (MapHitCheck(m_player->GetRect().GetCenter().x + wsize + m_add.x, m_player->GetRect().GetCenter().y + hsize + m_add.y, Dummy, MoveY) == 3)
	{
		m_fallPlayerSpeed = 0.0f;
	}
	// 左上のチェック、もしブロックの下辺に当たっていたら落下させる
	if (MapHitCheck(m_player->GetRect().GetCenter().x - wsize + m_add.x, m_player->GetRect().GetCenter().y - hsize + m_add.y, Dummy, MoveY) == 4)
	{
		m_fallPlayerSpeed *= -1.0f;
	}
	// 右上のチェック、もしブロックの下辺に当たっていたら落下させる
	if (MapHitCheck(m_player->GetRect().GetCenter().x + wsize + m_add.x, m_player->GetRect().GetCenter().y - hsize + m_add.y, Dummy, MoveY) == 4)
	{
		m_fallPlayerSpeed *= -1.0f;
	}
	
	m_add += {0.0f, MoveY};
	MoveY *= -1.0f;
	m_map->Movement({ 0.0f,MoveY });
	m_correction.y += MoveY;

	// 左下のチェック
	MapHitCheck(m_player->GetRect().GetCenter().x - wsize + m_add.x, m_player->GetRect().GetCenter().y + hsize + m_add.y, MoveX, Dummy);
	// 右下のチェック
	MapHitCheck(m_player->GetRect().GetCenter().x + wsize + m_add.x, m_player->GetRect().GetCenter().y + hsize + m_add.y, MoveX, Dummy);
	// 左上のチェック
	MapHitCheck(m_player->GetRect().GetCenter().x - wsize + m_add.x, m_player->GetRect().GetCenter().y - hsize + m_add.y, MoveX, Dummy);
	// 右上のチェック
	MapHitCheck(m_player->GetRect().GetCenter().x + wsize + m_add.x, m_player->GetRect().GetCenter().y - hsize + m_add.y, MoveX, Dummy);

	m_add += { MoveX, 0.0f };
	MoveX *= -1.0f;
	m_map->Movement({ MoveX,0.0f });
	m_correction.x += MoveX;
	//終了
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
	PlayerCenter();//プレイヤーがセンターに居るかどうか
	m_player->Action(ActionType::grah_idle);
	float PlayerMoveX = 0.0f, PlayerMoveY = 0.0f;//プレイヤーの移動
	m_correction = {0.0f,0.0f};
	//左に移動
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
	//右に移動
	else if (input.IsPressed(InputType::right))
	{
		if (input.IsTriggered(InputType::right))
		{
			m_player->SetLeft(false);
		}
		else
		{
			//プレイヤーが画面中央にいる＆マップの表示できる範囲以内の時は動かせる
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
	//梯子移動
	//上キーで梯子を上がれる　上が梯子または下が梯子
	if (input.IsPressed(InputType::up))
	{
		m_player->SetJump(false);
		m_fallPlayerSpeed = 0.0f;
		PlayerMoveY = 0.0f;
		if ((m_map->GetMapEventParam(posX + m_player->GetRect().GetSize().w / 2 - kPullPos, posY + m_player->GetRect().GetSize().h / 2 + 1.0f) == MapEvent_screenMoveU) ||
			(m_map->GetMapEventParam(posX + m_player->GetRect().GetSize().w / 2 - kPullPos, posY + m_player->GetRect().GetSize().h / 2 + 1.0f) == MapEvent_ladder))
		{
			PlayerMoveY -= kPlayerMoveSpeed;
			//下に何もないとき　は移動しないようにする
			if ((m_map->GetMapEventParam(posX + m_player->GetRect().GetSize().w / 2 - kPullPos, posY + m_player->GetRect().GetSize().h / 2) == MapEvent_no))
			{
				PlayerMoveY += kPlayerMoveSpeed;
			}
		}
	}
	//下キーで梯子を下がれる　下が梯子
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
	//プレイヤージャンプ処理
	else if (m_player->IsJump() == false && input.IsTriggered(InputType::junp))
	{
		m_fallPlayerSpeed = -kJumpAcc;
		m_player->Action(ActionType::grah_jump);
		m_player->SetJump(true);
		Sound::Play(Sound::PlayerJump);
	}

	//プレイヤーがジャンプしていたら
	if (m_player->IsJump())
	{
		// 落下処理
		m_fallPlayerSpeed += 0.4f;
		// 落下速度を移動量に加える
		PlayerMoveY = m_fallPlayerSpeed;
	}

	if (m_isPlayerMoveU || m_isPlayerMoveD || m_isPlayerMoveW)
	{
		m_updateFunc = &GameplayingScene::MoveMapUpdat;
	}

	// 移動量に基づいてキャラクタの座標を移動
	MovePlayer(PlayerMoveX, PlayerMoveY);

	m_map->Update();		//	マップ更新
	m_player->Update();		//	プレイヤー更新
	//エネミー
	float MoveX = m_correction.x, MoveY = m_correction.y;
	MoveEnemy(MoveX, MoveY);
	m_enemyFactory->Update();

	m_shotFactory->Update();//ショット更新

	//ショット
	if (input.IsTriggered(InputType::shot))//shotを押したら弾を作る
	{
		createShot(m_player->GetRect().GetCenter(), true, m_player->IsLeft());
		Sound::Play(Sound::PlayeyShot);
		m_player->Action(ActionType::grah_attack);
	}

	for (int i = 0; i < kShot; i++)
	{
		if (m_shots[i]->IsExist())//存在している弾だけ更新する
		{
			m_shots[i]->Update();
			m_shots[i]->Movement({ kShotSpeed, m_correction.y });
		}
	}
	m_shotFactory->Movement({ kShotSpeed, m_correction.y });

	//自機の弾と、敵機の当たり判定
	for (auto& shot : m_shots)
	{
		if (!shot->IsExist())	continue;
		if (!shot->GetPlayerShot())	continue;//プレイヤーが撃った弾ではなかったら処理しない
		for (auto& enemy : m_enemyFactory->GetEnemies())
		{
			if (!enemy->IsExist())	continue;
			//敵に弾がヒットした
			if (shot->GetRect().IsHit(enemy->GetRect()))
			{
				shot->SetExist(false);
				enemy->Damage(shot->AttackPower());
				Sound::Play(Sound::PlayeyShotHit);
				break;
			}
		}
	}
	//敵の弾と、プレイヤーの当たり判定
	if (m_player->IsCollidable())
	{
		for (auto& shot : m_shotFactory->GetShot())
		{
			if (!shot->IsExist())	continue;
			if (shot->GetPlayerShot())	continue;
			//ショットとプレイヤーの当たり判定
			if (shot->GetRect().IsHit(m_player->GetRect()))
			{
				shot->SetExist(false);
				m_player->Damage(shot->AttackPower());
				Sound::Play(Sound::PlayeyShotHit);
				return;
			}
		}
	}
	//プレイヤーと、敵の当たり判定
	if (m_player->IsCollidable())
	{
		for (auto& enemy : m_enemyFactory->GetEnemies())
		{
			if (!enemy->IsExist())	continue;
			//敵とプレイヤーが当たった
			if (enemy->GetRect().IsHit(m_player->GetRect()))
			{
				m_player->Damage(enemy->TouchAttackPower());
				Sound::Play(Sound::PlayeyShotHit);
				break;
			}
		}
	}

	//ゲームオーバー判定
	{
		//プレイヤーのHPが０になったらゲームオーバーにする
		if (m_hp[Object_Player]->GetHp() <= 0)
		{
			m_updateFunc = &GameplayingScene::FadeOutUpdat;
			m_fadeColor = 0xff0000;
			m_crea = 1;
		}
		//プレイヤーの上座標+10ぐらいが　death判定の部分に触れたら
		if ((m_map->GetMapEventParam(posX - m_player->GetRect().GetSize().w * 0.5f, posY + m_player->GetRect().GetSize().h * 0.5f + 10.0f) == MapEvent_death) &&
			(m_map->GetMapEventParam(posX + m_player->GetRect().GetSize().w * 0.5f, posY + m_player->GetRect().GetSize().h * 0.5f + 10.0f) == MapEvent_death))
		{
			m_player->Action(ActionType::grah_hit);
			m_updateFunc = &GameplayingScene::FadeOutUpdat;
			m_fadeColor = 0xff0000;
			m_crea = 1;
		}
	}
	//ポーズ画面
	if (input.IsTriggered(InputType::pause))
	{
		Sound::Play(Sound::MenuOpen);
		m_manager.PushScene(new PauseScene(m_manager));
		return;
	}
}

void GameplayingScene::MoveMapUpdat(const InputState& input)
{
	//プレイヤーには足して、プレイヤーの画面上の下座標が画面の下座標の位置になるように
	//マップにはプレイヤーが↑になるまで同じ分だけ引く
	//画面下に来たら

	//今いる場所が画面の下になるように移動させる

	m_map->Update();
	
	float moveY = (Game::kMapNumY * Game::ChipSize) / 120.0f;
	float moveX = ((Game::kMapNumX * Game::ChipSize) / 120.0f) * -1.0f;

	//上に移動するとき
	if (m_isPlayerMoveU)
	{
		//プレイヤーの下座標がフィールドの下座標よりも小さいとき
		if (m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h / 2 < Game::kMapScreenBottomY)
		{
			m_map->Movement({ 0.0f,moveY });//mapを移動させる
			MoveEnemy( 0.0f,moveY);//エネミーを移動させる
			MovePlayer(0.0f, moveY - 0.1f);//プレイヤーを移動させる
			m_correction.y += moveY;//どのくらいマップが移動したか
			moveY *= -1.0f;
			m_add.y += moveY;//どのくらいプレイヤーが移動したか
		}
		else
		{
			m_updateFunc = &GameplayingScene::NormalUpdat;
			m_isPlayerMoveU = false;
			return;
		}
	}
	//下に移動するとき
	else if (m_isPlayerMoveD)
	{
		moveY *= -1.0f;
		//プレイヤーの上座標がフィールドの上座標よりも大きいとき
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
	//右に移動
	else if (m_isPlayerMoveW)
	{
		//プレイヤーの左座標がフィールドの左座標よりも大きいとき
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
			//ボス戦に移動する前にエネミーをすべて消す

			m_updateFunc = &GameplayingScene::BossUpdate;
			m_isPlayerMoveW = false;
			return;
		}
	}
}

void GameplayingScene::BossUpdate(const InputState& input)
{
	m_player->Action(ActionType::grah_idle);
	float PlayerMoveX = 0.0f, PlayerMoveY = 0.0f;//プレイヤーの移動

	//左に移動
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
	//右に移動
	else if (input.IsPressed(InputType::right))
	{
		if (input.IsTriggered(InputType::right))
		{
			m_player->SetLeft(false);
		}
		else
		{
			//プレイヤーが画面中央にいる＆マップの表示できる範囲以内の時は動かせる
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
	//プレイヤージャンプ処理
	if (m_player->IsJump() == false && input.IsTriggered(InputType::junp))
	{
		m_fallPlayerSpeed = -kJumpAcc;
		m_player->Action(ActionType::grah_jump);
		m_player->SetJump(true);
		Sound::Play(Sound::PlayerJump);
	}
	//プレイヤーがジャンプしていたら
	if (m_player->IsJump())
	{
		// 落下処理
		m_fallPlayerSpeed += 0.4f;
		float camera = m_map->GetPos().y + Game::kMapChipNumY * Game::ChipSize;
		// 落下速度を移動量に加える
		PlayerMoveY = m_fallPlayerSpeed;
	}
	// 移動量に基づいてキャラクタの座標を移動
	MovePlayer(PlayerMoveX, PlayerMoveY);
	m_player->Update();		//	プレイヤー更新
	m_enemyFactory->Update();//エネミー更新
	m_shotFactory->Update();//ショット更新

	//ショット
	if (input.IsTriggered(InputType::shot))//shotを押したら弾を作る
	{
		createShot(m_player->GetRect().GetCenter(), true, m_player->IsLeft());
		Sound::Play(Sound::PlayeyShot);
		m_player->Action(ActionType::grah_attack);
	}
	for (int i = 0; i < kShot; i++)
	{
		if (m_shots[i]->IsExist())//存在している弾だけ更新する
		{
			m_shots[i]->Update();
			m_shots[i]->Movement({ kShotSpeed, m_correction.y });
		}
	}

	//自機の弾と、敵機の当たり判定
	for (auto& shot : m_shots)
	{
		if (!shot->IsExist())	continue;
		if (!shot->GetPlayerShot())	continue;//プレイヤーが撃った弾ではなかったら処理しない
		for (auto& enemy : m_enemyFactory->GetEnemies())
		{
			if (!enemy->IsExist())	continue;
			//敵に弾がヒットした
			if (shot->GetRect().IsHit(enemy->GetRect()))
			{
				shot->SetExist(false);
				enemy->Damage(shot->AttackPower());
				break;
			}
		}
	}
	//敵の弾と、プレイヤーの当たり判定
	if (m_player->IsCollidable())
	{
		for (auto& shot : m_shotFactory->GetShot())
		{
			if (!shot->IsExist())	continue;
			if (shot->GetPlayerShot())	continue;
			//ショットとプレイヤーの当たり判定
			if (shot->GetRect().IsHit(m_player->GetRect()))
			{
				shot->SetExist(false);
				m_player->Damage(shot->AttackPower());
				return;
			}
		}
	}
	//プレイヤーと、敵の当たり判定
	if (m_player->IsCollidable())
	{
		for (auto& enemy : m_enemyFactory->GetEnemies())
		{
			if (!enemy->IsExist())	continue;
			//敵とプレイヤーが当たった
			if (enemy->GetRect().IsHit(m_player->GetRect()))
			{
				m_player->Damage(enemy->TouchAttackPower());
				break;
			}
		}
	}

	//プレイヤーのHPが０になったらゲームオーバーにする
	if (m_hp[Object_Player]->GetHp() <= 0)
	{
		m_updateFunc = &GameplayingScene::FadeOutUpdat;
		m_fadeColor = 0xff0000;
		m_crea = 1;
		return;
	}
	//エネミーのHPが０になったらゲームクリア
	if (m_hp[Object_EnemyBoss]->GetHp() <= 0)
	{
		m_updateFunc = &GameplayingScene::FadeOutUpdat;
		m_fadeColor = 0xff0000;
		m_crea = 0;
		return;
	}
	//ポーズ画面
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

