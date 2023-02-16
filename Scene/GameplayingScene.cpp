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
#include "../Game/ShotFactory.h"
#include "../Game/EnemyBase.h"
#include "../Game/HpBar.h"
#include "../Map.h"
/*

スーパーカッター　四角い機械の穴から大量に出てくるはさみ。HP5、攻撃力4
マンブー　　　　丸い形をしていて空中をまっすぐ移動しつつ顔を出した瞬間に8方向に弾を発射する敵 HP1、攻撃力（弾）2、（接触）1
スクリュードライバー　自機が近付くと、５方向同時に弾を二回発射する

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
	m_test = MakeScreen( Game::kScreenWidth,Game::kScreenHeight, true);

	for (auto& hp : m_hp)
	{
		hp = std::make_shared<HpBar>();
		hp->Init(my::MyLoadGraph(L"Data/hp.bmp"));
	}

	for (auto& shot : m_shots)
	{
		shot = std::make_shared<RockBuster>(my::MyLoadGraph(L"Data/rockBuster.png"));
	}

	m_player = std::make_shared<Player>(Position2{(Game::kMapScreenLeftX + Game::ChipSize*8),(Game::kMapScreenBottomY - 6*Game::ChipSize)},m_hp[Object_Player]);//プレイヤーの初期位置
	
	m_shotFactory = std::make_shared<ShotFactory>();
	
	m_enemyFactory = std::make_shared<EnemyFactory>(m_player, m_shotFactory);//プレイヤーとショットを渡す

	m_map = std::make_shared<Map>(m_enemyFactory,0);

	m_map->Movement({ Game::kMapScreenLeftX,((Game::kMapChipNumY * Game::ChipSize) - Game::kMapScreenBottomY) * -1.0f });//表示位置を指定
	m_add = { -Game::kMapScreenLeftX ,(Game::kMapChipNumY * Game::ChipSize) - Game::kMapScreenBottomY};

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
			shot->Start(pos,isLeft);
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

	// キャラクタの左上、右上、左下、右下部分が当たり判定のある
	// マップに衝突しているか調べ、衝突していたら補正する

	// 半分のサイズを算出
	wsize = m_player->GetRect().GetSize().w * 0.5f;
	hsize = m_player->GetRect().GetSize().h * 0.5f;

	// 左下のチェック、もしブロックの上辺に着いていたら落下を止める
	//左位置、下位置
	if (MapHitCheck(m_player->GetRect().GetCenter().x - wsize + m_add.x, m_player->GetRect().GetCenter().y + hsize + m_add.y, Dummy, MoveY) == 3)
	{

		m_fallPlayerSpeed = 0.0f;
	}

	// 右下のチェック、もしブロックの上辺に着いていたら落下を止める
	//右位置、下位置
	if (MapHitCheck(m_player->GetRect().GetCenter().x + wsize + m_add.x, m_player->GetRect().GetCenter().y + hsize + m_add.y, Dummy, MoveY) == 3)
	{
		m_fallPlayerSpeed = 0.0f;
	}

	// 左上のチェック、もしブロックの下辺に当たっていたら落下させる
	//左位置、上位置
	if (MapHitCheck(m_player->GetRect().GetCenter().x - wsize + m_add.x, m_player->GetRect().GetCenter().y - hsize + m_add.y, Dummy, MoveY) == 4)
	{
		m_fallPlayerSpeed *= -1.0f;
	}

	// 右上のチェック、もしブロックの下辺に当たっていたら落下させる
	//右位置、上位置
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

	// 接地判定
	// キャラクタの左下と右下の下に地面があるか調べる
	if ((m_map->GetMapEventParam(m_add.x + m_player->GetRect().GetCenter().x - m_player->GetRect().GetSize().w * 0.5f, m_add.y + m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h * 0.5f + 1.0f) == MapEvent_no) &&
		(m_map->GetMapEventParam(m_add.x + m_player->GetRect().GetCenter().x + m_player->GetRect().GetSize().w * 0.5f, m_add.y + m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h * 0.5f + 1.0f) == MapEvent_no))
	{
		// 足場が無かったらジャンプ中にする
		m_player->SetJump(true);
	}
	else
	{
		// 足場が在ったら接地中にする
		m_player->SetJump(false);
	}

	// 終了
	return;
}

void GameplayingScene::MoveEnemy(float MoveX, float MoveY)
{
	float Dummy = 0.0f;
	float hsize, wsize;

	for (auto& enemy : m_enemyFactory->GetEnemies())
	{
		if (!enemy->IsExist())	continue;
		// 半分のサイズを算出
		wsize = enemy->GetRect().GetSize().w * 0.5f;
		hsize = enemy->GetRect().GetSize().h * 0.5f;
		
		//移動させる
		enemy->Movement({ MoveX,MoveY });

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
			//移動量を補正する
			MoveY = blockBottomY - Y + 1.0f;
			//下辺に当たったと返す
			return 4;
		}
		//ブロックの左に当たっていた場合
		if (MoveX > 0.0f)
		{
			//移動量を補正する
			MoveX = blockLeftX - X - 1.0f;
			//左辺に当たったと返す
			return 1;
		}
		//ブロックの右に当たっていた場合
		if (MoveX < 0.0f)
		{
			//移動量を補正する
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
	//画面の大きさの半分
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

	m_isPlayerCenterLR = false;

	if (m_player->GetRect().GetCenter().x > fieldCenterLeftUp.x && m_player->GetRect().GetCenter().x < fieldCenterRightBottom.x)
	{
		m_isPlayerCenterLR = true;
	}/*
	if (m_player->GetRect().GetCenter().y > fieldCenterLeftUp.y && m_player->GetRect().GetCenter().y < fieldCenterRightBottom.y)
	{
		m_isPlayerCenterUD = true;
	}*/

	//上下移動させる
	//プレイヤーの上座標がフィールドの上に当たっていたら移動できる
	//縦移動できる場所についたら移動させる
	if ((m_map->GetMapEventParam(m_add.x + m_player->GetRect().GetCenter().x - m_player->GetRect().GetSize().w * 0.5f, m_add.y + m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h * 0.5f + 1.0f) == MapEvent_screenMoveU)&&
		(m_map->GetMapEventParam(m_add.x + m_player->GetRect().GetCenter().x + m_player->GetRect().GetSize().w / 2 - kPullPos, m_add.y + m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h / 2 + 2.0f) == MapEvent_screenMoveU))
	{
		m_isPlayerMoveU = true;
	}
	//プレイヤーの下座標が縦移動できる場所についたら移動させる
	if (!m_isPlayerMoveU &&
		(m_map->GetMapEventParam(m_add.x + m_player->GetRect().GetCenter().x - m_player->GetRect().GetSize().w * 0.5f, m_add.y + m_player->GetRect().GetCenter().y - m_player->GetRect().GetSize().h * 0.5f + 1.0f) == MapEvent_screenMoveD) &&
		(m_map->GetMapEventParam(m_add.x + m_player->GetRect().GetCenter().x + m_player->GetRect().GetSize().w / 2 - kPullPos, m_add.y + m_player->GetRect().GetCenter().y - m_player->GetRect().GetSize().h / 2 + 2.0f) == MapEvent_screenMoveD))
	{
		m_isPlayerMoveD = true;
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
		//m_manager.PushScene(new GameStartCountScene(m_manager));
		return;
	}
}

void GameplayingScene::NormalUpdat(const InputState& input)
{
	PlayerCenter();//プレイヤーがセンターに居るかどうか

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
		m_player->Action(ActionType::grah_idle);
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
			//マップを表示する画面の右上、右下に何もない空間が来たときはマップを移動できない
			//マップを表示する画面の右上、左下に何か表示できるものがあるときは移動できる
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
		m_player->Action(ActionType::grah_idle);
	}

	//プレイヤージャンプ処理
	if (m_player->IsJump() == false && input.IsTriggered(InputType::junp))
	{
		m_fallPlayerSpeed = -kJumpAcc;
		m_player->Action(ActionType::grah_jump);
		m_player->SetJump(true);
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

	//梯子移動
	// 梯子判定の場所にいるとき //プレイヤーの上下 の場所に梯子があるか　プレイヤーの上に梯子１が時、プレイヤーの上下に梯子２があるとき
	if ((m_map->GetMapEventParam(m_add.x + m_player->GetRect().GetCenter().x - m_player->GetRect().GetSize().w / 2 + kPullPos, m_add.y + m_player->GetRect().GetCenter().y - m_player->GetRect().GetSize().h / 2 - 2.0f) == MapEvent_ladder) ||
		(m_map->GetMapEventParam(m_add.x + m_player->GetRect().GetCenter().x - m_player->GetRect().GetSize().w / 2 + kPullPos, m_add.y + m_player->GetRect().GetCenter().y - m_player->GetRect().GetSize().h / 2 - 2.0f) ==  MapEvent_screenMoveD)|| 
		(m_map->GetMapEventParam(m_add.x + m_player->GetRect().GetCenter().x + m_player->GetRect().GetSize().w / 2 - kPullPos, m_add.y + m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h / 2 + 2.0f) == MapEvent_ladder)||
		(m_map->GetMapEventParam(m_add.x + m_player->GetRect().GetCenter().x + m_player->GetRect().GetSize().w / 2 - kPullPos, m_add.y + m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h / 2 + 2.0f) == MapEvent_screenMoveD))
	{
#ifdef _DEBUG
		DrawString(400, 40, L"梯子", 0xffffff);
#endif
		//上キーで梯子を上がれる
		if (input.IsPressed(InputType::up))
		{
			PlayerMoveY -= kPlayerMoveSpeed;
		}
		//下キーで梯子を下がれる
		if (input.IsPressed(InputType::down))
		{
			PlayerMoveY += kPlayerMoveSpeed;
		}
	}

	if (m_isPlayerMoveU || m_isPlayerMoveD)
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
				m_player->Damage(enemy->TouchAttackPower());//m_hp[Object_Player]->Damage(enemy->TouchAttackPower());
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
		}
		//death判定の部分に触れたら
		if ((m_map->GetMapEventParam(m_add.x + m_player->GetRect().GetCenter().x - m_player->GetRect().GetSize().w * 0.5f, m_add.y + m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h * 0.5f + 1.0f) == MapEvent_death) &&
			(m_map->GetMapEventParam(m_add.x + m_player->GetRect().GetCenter().x + m_player->GetRect().GetSize().w * 0.5f, m_add.y + m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h * 0.5f + 1.0f) == MapEvent_death))
		{
			m_player->Action(ActionType::grah_hit);
			m_updateFunc = &GameplayingScene::FadeOutUpdat;
			m_fadeColor = 0xff0000;
		}
	}
	
	/*if (input.IsTriggered(InputType::next))
	{
		for (auto& shot : m_shots)
		{
			shot->SetExist(false);
		}

		m_updateFunc = &GameplayingScene::FadeOutUpdat;
		m_fadeColor = 0xff0000;
	}*/
	if (input.IsTriggered(InputType::pause))
	{
		m_manager.PushScene(new PauseScene(m_manager));
		return;
	}
}

void GameplayingScene::MoveMapUpdat(const InputState& input)
{
	////プレイヤーの画面上の上座標がマップを表示する画面の上座標と同じ、小さいとき 画面の大きさだけマップとプレイヤーを移動させる


	//プレイヤーには足して、プレイヤーの画面上の下座標が画面の下座標の位置になるように
	//マップにはプレイヤーが↑になるまで同じ分だけ引く
	//画面下に来たら

	//今いる場所が画面の下になるように移動させる

	m_map->Update();
	//float moveY = kPlayerMoveSpeed - 1.0f;
	
	float moveY = (Game::kMapNumY * Game::ChipSize) / 120.0f;
	
	if (m_isPlayerMoveU)
	{
		if (m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h / 2 < Game::kMapScreenBottomY)
		{
			m_map->Movement({ 0.0f,moveY });
			MoveEnemy(0.0f, moveY);
			m_player->Movement({ 0.0f,moveY});
			m_correction.y += moveY;
			moveY *= -1.0f;
			m_add.y += moveY;
		}
		else
		{
			m_updateFunc = &GameplayingScene::NormalUpdat;
			m_isPlayerMoveU = false;
			return;
		}
	}
	else if (m_isPlayerMoveD)
	{
		moveY *= -1.0f;
		if (m_player->GetRect().GetCenter().y - m_player->GetRect().GetSize().h / 2 > Game::kMapScreenTopY)
		{
			m_map->Movement({ 0.0f,moveY });
			MoveEnemy(0.0f, moveY);
			m_player->Movement({ 0.0f,moveY });
			m_correction.y += moveY;
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

