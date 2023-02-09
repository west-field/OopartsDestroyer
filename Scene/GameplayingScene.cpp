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
	constexpr float kPlayerMoveSpeed = 5.0f;//プレイヤーの移動速度
	constexpr float kEnemyMoveSpeed = -4.0f;//エネミーの移動速度
	constexpr float kJumpAcc = 10.0f;//ジャンプ力
	constexpr float kShotSpeed = 8.0f;//ショットスピード

	constexpr float kPullPos = 10.0f;
}

GameplayingScene::GameplayingScene(SceneManager& manager) : Scene(manager), m_updateFunc(&GameplayingScene::FadeInUpdat)
, m_add(), m_correction()
{
	m_test = MakeScreen( Game::kScreenWidth,Game::kScreenHeight, true);

	m_map = std::make_shared<Map>();
	
	m_player = std::make_shared<Player>(Position2{(Game::kMapScreenLeftX + Game::ChipSize*8),(Game::kMapScreenBottomY - 6*Game::ChipSize)});//プレイヤーの初期位置
	m_enemyFactory = std::make_shared<EnemyFactory>(m_player);//プレイヤーを渡す

	m_map->Movement({ Game::kMapScreenLeftX,((Game::kMapChipNumY * Game::ChipSize) - Game::kMapScreenBottomY) * -1.0f });//表示位置を指定
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

	m_hp[Object_Player]->Draw(true);//HPバーを表示

	bool ismieru = false;
	//見えない場所を作る
	DrawBox(0, 0, Game::kMapScreenLeftX, Game::kScreenHeight, 0x000000, ismieru);//左側
	DrawBox(Game::kMapScreenRightX, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, ismieru);//右側
	DrawBox(Game::kMapScreenLeftX, 0, Game::kMapScreenRightX, Game::kMapScreenTopY, 0x000000, ismieru);//上側
	DrawBox(Game::kMapScreenLeftX, Game::kMapScreenBottomY, Game::kMapScreenRightX, Game::kScreenHeight, 0x000000, ismieru);//下側

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
	DrawBoxAA(fieldCenterLeftUp.x, fieldCenterLeftUp.y, fieldCenterRightBottom.x, fieldCenterRightBottom.y, 0xaaffaa, false);//画面の中心位置

	DrawFormatString(0, 80, 0xffffff, L"player.x%3f,y.%3f", m_player->GetRect().center.x+m_add.x, m_player->GetRect().center.y+m_add.y);//プレイヤーと足す座標
	DrawFormatString(0, 100, 0xffffff, L"player.x%3f,y.%3f", m_player->GetRect().center.x, m_player->GetRect().center.y);//プレイヤー座標
	DrawFormatString(0, 120, 0xffffff, L"add.x%3f,y.%3f",m_add.x, m_add.y);//画面がどのくらい移動したか

	//表示したいマップ画面
	DrawBox(Game::kMapScreenLeftX, Game::kMapScreenTopY, Game::kMapScreenRightX, Game::kMapScreenBottomY, 0xffffff, false);
	//判定する範囲
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
	if ((m_map->GetMapChipParam(m_add.x + m_player->GetRect().GetCenter().x - m_player->GetRect().GetSize().w * 0.5f, m_add.y + m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h * 0.5f + 1.0f) == no)||
		(m_map->GetMapChipParam(m_add.x + m_player->GetRect().GetCenter().x - m_player->GetRect().GetSize().w * 0.5f, m_add.y + m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h * 0.5f + 1.0f) == sky) &&
		(m_map->GetMapChipParam(m_add.x + m_player->GetRect().GetCenter().x + m_player->GetRect().GetSize().w * 0.5f, m_add.y + m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h * 0.5f + 1.0f) == no)||
		(m_map->GetMapChipParam(m_add.x + m_player->GetRect().GetCenter().x + m_player->GetRect().GetSize().w * 0.5f, m_add.y + m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h * 0.5f + 1.0f) == sky))
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

void GameplayingScene::MoveShot(int i ,float MoveX, float MoveY)
{
	float Dummy = 0.0f;
	float hsize, wsize;

	// 半分のサイズを算出
	wsize = m_shots[i]->GetRect().GetSize().w * 0.5f;
	hsize = m_shots[i]->GetRect().GetSize().h * 0.5f;

	if (m_shots[i]->IsLeft())//左に撃った弾かどうか
	{
		//左に打った場合は逆方向に移動できるように
		MoveX *= -1.0f;
	}

	// 左右移動成分を加算
	m_shots[i]->Movement({ MoveX,MoveY });

	//画面の外に出たら消える
	//左端
	if (m_shots[i]->GetRect().GetCenter().x + wsize < Game::kMapScreenLeftX - wsize*2)
	{
		m_shots[i]->SetExist(false);
	}
	//右端
	if (m_shots[i]->GetRect().GetCenter().x - wsize > Game::kMapScreenRightX + wsize*2)
	{
		m_shots[i]->SetExist(false);
	}

	// 終了
	return ;
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

	int mapchip = m_map->GetMapChipParam(afterX, afterY);
	//当たり判定のあるブロックに当たっているか
	if (mapchip  == terrain ||
		mapchip == wall1 ||
		mapchip == wall2 ||
		mapchip == stone)
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
			//マップを表示する画面の左上、左下に何もない空間が来たときはマップを移動できない
			//マップを表示する画面の左上、左下に何か表示できるものがあるときは移動できる
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
		if (m_isPlayerCenterUD && camera > Game::kMapScreenBottomY)//下端よりも大きい場合
		{
			MoveMap(0.0f, -m_fallPlayerSpeed);
		}
		else
		{
			// 落下速度を移動量に加える
			PlayerMoveY = m_fallPlayerSpeed;
		}
	}

	//梯子移動
	//プレイヤーの上下 の場所に梯子があるか　プレイヤーの上に梯子１が時、プレイヤーの上下に梯子２があるとき
	if ((m_map->GetMapChipParam(m_add.x + m_player->GetRect().GetCenter().x- m_player->GetRect().GetSize().w / 2+ kPullPos, m_add.y + m_player->GetRect().GetCenter().y - m_player->GetRect().GetSize().h / 2 - 2.0f) == ladder1)||
		(m_map->GetMapChipParam(m_add.x + m_player->GetRect().GetCenter().x- m_player->GetRect().GetSize().w / 2+ kPullPos, m_add.y + m_player->GetRect().GetCenter().y - m_player->GetRect().GetSize().h / 2 - 2.0f) == ladder2) &&
		(m_map->GetMapChipParam(m_add.x + m_player->GetRect().GetCenter().x+ m_player->GetRect().GetSize().w / 2- kPullPos, m_add.y + m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h / 2 + 2.0f) == ladder1) ||
		(m_map->GetMapChipParam(m_add.x + m_player->GetRect().GetCenter().x+ m_player->GetRect().GetSize().w / 2- kPullPos, m_add.y + m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h / 2 + 2.0f) == ladder2))
	{
		DrawString(400, 40, L"梯子", 0xffffff);
		//上キーで梯子を上がれる
		if (input.IsPressed(InputType::up))
		{
			if (m_isPlayerCenterUD && m_map->GetPos().y < Game::kMapScreenTopY)//上端よりも小さい場合
			{
				MoveMap(0.0f, kPlayerMoveSpeed);
			}
			else
			{
				PlayerMoveY -= kPlayerMoveSpeed;
			}
		}
		//下キーで梯子を下がれる
		if (input.IsPressed(InputType::down))
		{
			float camera = m_map->GetPos().y + Game::kMapChipNumY * Game::ChipSize;
			if (m_isPlayerCenterUD && camera > Game::kMapScreenBottomY)//下端よりも大きい場合
			{
				MoveMap(0.0f, -kPlayerMoveSpeed);
			}
			else
			{
				PlayerMoveY += kPlayerMoveSpeed;
			}
		}
	}

	// 移動量に基づいてキャラクタの座標を移動
	MovePlayer(PlayerMoveX, PlayerMoveY);

	m_map->Update();		//	マップ更新
	m_player->Update();		//	プレイヤー更新
	
	//エネミー
	float MoveX = kEnemyMoveSpeed + m_correction.x, MoveY = m_correction.y;
	MoveEnemy(MoveX, MoveY);
	m_enemyFactory->Update();

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
			MoveShot(i, kShotSpeed, m_correction.y);
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
				enemy->Damage(1);
				break;
			}
		}
	}

	//敵の弾と、プレイヤーの当たり判定
	for (auto& shot : m_shots)
	{
		if (!shot->IsExist())	continue;
		if (shot->GetPlayerShot())	continue;//プレイヤーが撃った弾だったら処理しない
		
		if (shot->GetRect().IsHit(m_player->GetRect()))
		{
			shot->SetExist(false);
			m_hp[Object_Player]->Damage(1);
			m_player->Action(ActionType::grah_hit);
			break;
		}
	}

	//プレイヤーと、敵の当たり判定
	for (auto& enemy : m_enemyFactory->GetEnemies())
	{
		if (!enemy->IsExist())	continue;
		//敵とプレイヤーが当たった
		if (enemy->GetRect().IsHit(m_player->GetRect()))
		{
			enemy->Damage(1);
			m_hp[Object_Player]->Damage(1);
			m_player->Action(ActionType::grah_hit);
			break;
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
		//針のある部分に乗っていたら
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

