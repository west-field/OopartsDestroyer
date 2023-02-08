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
	//constexpr int kFieldSizeX = (Game::kScreenWidth / 3) + (Game::kScreenWidth / 3);//700;
	//constexpr int kFieldSizeY = kFieldSizeX;
	//constexpr int kFieldLeftX = (Game::kScreenWidth-kFieldSizeX)/2;
	//constexpr int kFieldLeftY = (Game::kScreenHeight-kFieldSizeY)/2;
	//constexpr int kField = Game::kScreenHeight - 100;

	constexpr float kPlayerMoveSpeed = 5.0f;//プレイヤーの移動速度
	constexpr float kEnemyMoveSpeed = -4.0f;//エネミーの移動速度
	constexpr float kJumpAcc = 10.0f;//ジャンプ力
	constexpr float kShotSpeed = 8.0f;//ショットスピード

	constexpr float kPullPos = 10.0f;
}

GameplayingScene::GameplayingScene(SceneManager& manager) : Scene(manager), m_updateFunc(&GameplayingScene::FadeInUpdat)
, m_add()
{
	m_test = MakeScreen( Game::kScreenWidth,Game::kScreenHeight, true);

	m_map = std::make_shared<Map>();
	
	m_player = std::make_shared<Player>(Position2{(271),(720/*Game::kMapChipNumY-5*Game::ChipSize*/)});
	m_enemyFactory = std::make_shared<EnemyFactory>(m_player);

	m_map->Movement({0.0f,((Game::kMapChipNumY * Game::ChipSize) - (Game::kNumY * Game::ChipSize)) * -1.0f });
	m_add.y = ((Game::kMapChipNumY * Game::ChipSize) - (Game::kNumY * Game::ChipSize));

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

	DrawBoxAA(m_player->GetRect().GetCenter().x - m_player->GetRect().GetSize().w / 2+ kPullPos, m_player->GetRect().GetCenter().y - m_player->GetRect().GetSize().h / 2 - 1.0f,
		m_player->GetRect().GetCenter().x + m_player->GetRect().GetSize().w / 2- kPullPos, m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h / 2 + 1.0f, 0xffffff, false);
	
	m_enemyFactory->Draw();
	int num = 0;
	for (auto& shot : m_shots)
	{
		if (shot->IsExist())
		{
			shot->Draw();
			num++;
		}
	}

	m_hp[Object_Player]->Draw(true);

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

	/*SetDrawScreen(DX_SCREEN_BACK);
	int shakeX = GetRand(4) - 2;
	int shakeY = GetRand(4) - 2;
	DrawGraph(shakeX, shakeY, m_test, true);*/

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
	DrawBoxAA(fieldCenterLeftUp.x, fieldCenterLeftUp.y, fieldCenterRightBottom.x, fieldCenterRightBottom.y, 0xaaffaa, false);

	DrawFormatString(0, 80, 0xffffff, L"player.x%3f,y.%3f", m_player->GetRect().center.x+m_add.x, m_player->GetRect().center.y+m_add.y);//プレイヤーと足す座標
	DrawFormatString(0, 100, 0xffffff, L"player.x%3f,y.%3f", m_player->GetRect().center.x, m_player->GetRect().center.y);//プレイヤー座標
	DrawFormatString(0, 120, 0xffffff, L"add.x%3f,y.%3f",m_add.x, m_add.y);//画面がどのくらい移動したか

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

int GameplayingScene::MovePlayer(float MoveX, float MoveY)
{
	float Dummy = 0.0f;
	float hsize, wsize;

	// キャラクタの左上、右上、左下、右下部分が当たり判定のある
	// マップに衝突しているか調べ、衝突していたら補正する

	// 半分のサイズを算出
	wsize = m_player->GetRect().GetSize().w * 0.5f;
	hsize = m_player->GetRect().GetSize().h * 0.5f;

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

	m_player->Movement({ 0.0f,MoveY });

	// 後に左右移動成分だけでチェック
	// 左下のチェック
	MapHitCheck(m_player->GetRect().GetCenter().x - wsize + m_add.x, m_player->GetRect().GetCenter().y + hsize + m_add.y, MoveX, Dummy);

	// 右下のチェック
	MapHitCheck(m_player->GetRect().GetCenter().x + wsize + m_add.x, m_player->GetRect().GetCenter().y + hsize + m_add.y, MoveX, Dummy);

	// 左上のチェック
	MapHitCheck(m_player->GetRect().GetCenter().x - wsize + m_add.x, m_player->GetRect().GetCenter().y - hsize + m_add.y, MoveX, Dummy);

	// 右上のチェック
	MapHitCheck(m_player->GetRect().GetCenter().x + wsize + m_add.x, m_player->GetRect().GetCenter().y - hsize + m_add.y, MoveX, Dummy);

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
	return 0;
}

int GameplayingScene::MoveEnemy(float MoveX, float MoveY)
{
	float Dummy = 0.0f;
	float hsize, wsize;

	// キャラクタの左上、右上、左下、右下部分が当たり判定のある
	// マップに衝突しているか調べ、衝突していたら補正する
	for (auto& enemy : m_enemyFactory->GetEnemies())
	{
		if (!enemy->IsExist())	continue;
		// 半分のサイズを算出
		wsize = enemy->GetRect().GetSize().w * 0.5f;
		hsize = enemy->GetRect().GetSize().h * 0.5f;

		// 左下のチェック、もしブロックの上辺に着いていたら落下を止める
		if (MapHitCheck(enemy->GetRect().GetCenter().x - wsize, enemy->GetRect().GetCenter().y + hsize, Dummy, MoveY) == 3)
		{
			m_fallEnemySpeed = 0.0f;
		}
		// 右下のチェック、もしブロックの上辺に着いていたら落下を止める
		if (MapHitCheck(enemy->GetRect().GetCenter().x + wsize, enemy->GetRect().GetCenter().y + hsize, Dummy, MoveY) == 3)
		{
			m_fallEnemySpeed = 0.0f;
		}

		// 左上のチェック、もしブロックの下辺に当たっていたら落下させる
		if (MapHitCheck(enemy->GetRect().GetCenter().x - wsize, enemy->GetRect().GetCenter().y - hsize, Dummy, MoveY) == 4)
		{
			m_fallEnemySpeed *= -1.0f;
		}

		// 右上のチェック、もしブロックの下辺に当たっていたら落下させる
		if (MapHitCheck(enemy->GetRect().GetCenter().x + wsize, enemy->GetRect().GetCenter().y - hsize, Dummy, MoveY) == 4)
		{
			m_fallEnemySpeed *= -1.0f;
		}
		// 補正された上下移動成分を加算して実際に移動。あたってなかったらそのまま計算される
		enemy->Movement({ 0.0f,MoveY });

		if (enemy->IsLeft())
		{
			//MoveX *= -1.0f;
		}
		// 後に左右移動成分だけでチェック
		// 左のチェック
		if (MapHitCheck(enemy->GetRect().GetCenter().x - hsize, enemy->GetRect().GetCenter().y + hsize, MoveX, Dummy) == 2 ||
			MapHitCheck(enemy->GetRect().GetCenter().x - hsize, enemy->GetRect().GetCenter().y - hsize, MoveX, Dummy) == 2)
		{
			enemy->SetExist(false);
		}

		// 右のチェック
		if (MapHitCheck(enemy->GetRect().GetCenter().x + hsize, enemy->GetRect().GetCenter().y + hsize, MoveX, Dummy) == 1 ||
			MapHitCheck(enemy->GetRect().GetCenter().x + hsize, enemy->GetRect().GetCenter().y - hsize, MoveX, Dummy) == 1)
		{
			enemy->SetExist(false);
		}

		// 左右移動成分を加算
		enemy->Movement({ MoveX,0.0f });

		// 接地判定
			// キャラクタの左下と右下の下に地面があるか調べる
		if ((m_map->GetMapChipParam(enemy->GetRect().GetCenter().x - enemy->GetRect().GetSize().w * 0.5f, enemy->GetRect().GetCenter().y + enemy->GetRect().GetSize().h * 0.5f + 1.0f) == no)||
			(m_map->GetMapChipParam(enemy->GetRect().GetCenter().x - enemy->GetRect().GetSize().w * 0.5f, enemy->GetRect().GetCenter().y + enemy->GetRect().GetSize().h * 0.5f + 1.0f) == sky) &&
			(m_map->GetMapChipParam(enemy->GetRect().GetCenter().x + enemy->GetRect().GetSize().w * 0.5f, enemy->GetRect().GetCenter().y + enemy->GetRect().GetSize().h * 0.5f + 1.0f) == no)||
			(m_map->GetMapChipParam(enemy->GetRect().GetCenter().x + enemy->GetRect().GetSize().w * 0.5f, enemy->GetRect().GetCenter().y + enemy->GetRect().GetSize().h * 0.5f + 1.0f) == sky))
		{
			// 足場が無かったらジャンプ中にする
			enemy->SetJump(true);
		}
		else
		{
			// 足場が在ったら接地中にする
			enemy->SetJump(false);
		}
	}

	// 終了
	return 0;
}

int GameplayingScene::MoveShot(int i ,float MoveX, float MoveY)
{
	float Dummy = 0.0f;
	float hsize, wsize;

	// キャラクタの左上、右上、左下、右下部分が当たり判定のある
	// マップに衝突しているか調べ、衝突していたら補正する

	// 半分のサイズを算出
	wsize = m_shots[i]->GetRect().GetSize().w * 0.5f;
	hsize = m_shots[i]->GetRect().GetSize().h * 0.5f;

	if (m_shots[i]->IsLeft())//左に撃った弾かどうか
	{
		MoveX *= -1.0f;
	}

	// 左右移動成分を加算
	m_shots[i]->Movement({ MoveX,0.0f });

	// 後に左右移動成分だけでチェック
	// 左のチェック
	if (m_shots[i]->GetRect().GetCenter().x + wsize < 0)
	{
		m_shots[i]->SetExist(false);
	}
	// 右下
	if (m_shots[i]->GetRect().GetCenter().x - wsize > Game::kScreenWidth)
	{
		m_shots[i]->SetExist(false);
	}

	// 終了
	return 0;
}

int GameplayingScene::MapHitCheck(float X, float Y, float& MoveX, float& MoveY)
{
	float afterX, afterY;
	//移動量を足す
	afterX = X + MoveX;
	afterY = Y + MoveY;
	float blockLeftX = 0.0f, blockTopY = 0.0f, blockRightX = 0.0f, blockBottomY = 0.0f;

	//当たっていたら壁から離す処理を行う、ブロックの左右上下の座標を算出
	blockLeftX = static_cast<float>((int)afterX / Game::ChipSize) * Game::ChipSize;//左　X座標
	blockRightX = static_cast<float>((int)afterX / Game::ChipSize + 1) * Game::ChipSize;//右　X座標

	blockTopY = static_cast<float>((int)afterY / Game::ChipSize) * Game::ChipSize;//上　Y座標
	blockBottomY = static_cast<float>((int)afterY / Game::ChipSize + 1) * Game::ChipSize;//下　Y座標

	//当たり判定のあるブロックに当たっているか
	if (m_map->GetMapChipParam(afterX, afterY ) == terrain ||
		m_map->GetMapChipParam(afterX , afterY) == wall1 ||
		m_map->GetMapChipParam(afterX, afterY) == wall2 ||
		m_map->GetMapChipParam(afterX , afterY) == stone)
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
			MoveX = blockLeftX - X - 1.0f - m_add.x;
			//左辺に当たったと返す
			return 1;
		}
		//ブロックの右に当たっていた場合
		if (MoveX < 0.0f)
		{
			//移動量を補正する
			MoveX = blockRightX - X + 1.0f + m_add.x;
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

int GameplayingScene::MoveMap(float MoveX, float MoveY)
{
	float Dummy = 0.0f;
	float wsize = m_player->GetRect().GetSize().w * 0.5f;
	float hsize = m_player->GetRect().GetSize().h * 0.5f;

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
	m_map->Movement({ 0.0f,MoveY });
	MoveY *= -1.0f;
	m_add += {0.0f, MoveY};

	// 左下のチェック
	MapHitCheck(m_player->GetRect().GetCenter().x - wsize + m_add.x, m_player->GetRect().GetCenter().y + hsize + m_add.y, MoveX, Dummy);
	// 右下のチェック
	MapHitCheck(m_player->GetRect().GetCenter().x + wsize + m_add.x, m_player->GetRect().GetCenter().y + hsize + m_add.y, MoveX, Dummy);
	// 左上のチェック
	MapHitCheck(m_player->GetRect().GetCenter().x - wsize + m_add.x, m_player->GetRect().GetCenter().y - hsize + m_add.y, MoveX, Dummy);
	// 右上のチェック
	MapHitCheck(m_player->GetRect().GetCenter().x + wsize + m_add.x, m_player->GetRect().GetCenter().y - hsize + m_add.y, MoveX, Dummy);

	m_map->Movement({ MoveX,0.0f });
	MoveX *= -1.0f;
	m_add += { MoveX, 0.0f };

	//終了
	return 0;
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

	float PlayerMoveX = 0.0f, PlayerMoveY = 0.0f;
	//左に移動
	if (input.IsPressed(InputType::left))
	{
		if (m_isPlayerCenterLR && m_map->GetPos().x < 0.0f)
		{
			//m_camera.x += kPlayerMoveSpeed;
			MoveMap(kPlayerMoveSpeed, 0.0f);
		}
		else
		{
			PlayerMoveX -= kPlayerMoveSpeed;
			m_player->SetLeft(true);
		}
		m_player->Action(ActionType::grah_walk);
	}
	//右に移動
	else if (input.IsPressed(InputType::right))
	{
		if (m_add.x < 0.0f)
		{
			m_add.x = 0.0f;
			m_map->SetPos({ 0.0f,0.0f });
		}
		float camera = m_map->GetPos().x + Game::kMapChipNumX * Game::ChipSize;
		if (m_isPlayerCenterLR && camera > Game::kScreenWidth)
		{
			//m_camera.x -= kPlayerMoveSpeed;
			MoveMap(-kPlayerMoveSpeed, 0.0f);
		}
		else
		{
			PlayerMoveX += kPlayerMoveSpeed;
			m_player->SetLeft(false);
		}
		m_player->Action(ActionType::grah_walk);
	}
	else
	{
		m_player->Action(ActionType::grah_walk);
	}

	//プレイヤージャンプ処理
	if (m_player->IsJump() == false && input.IsTriggered(InputType::junp))
	{
		m_fallPlayerSpeed = -kJumpAcc;
		m_player->Action(ActionType::grah_jump);
		m_player->SetJump(true);
	}

	float camera = m_map->GetPos().y + Game::kMapChipNumY * Game::ChipSize;
	if (m_isPlayerCenterUD && camera > Game::kScreenHeight)
	{
		MoveMap(0.0f, kJumpAcc);
	}

	//プレイヤーがジャンプしていたら
	if (m_player->IsJump())
	{
		// 落下処理
		m_fallPlayerSpeed += 0.4f;
		float camera = m_map->GetPos().y + Game::kMapChipNumY * Game::ChipSize;
		if (m_isPlayerCenterUD && camera > Game::kScreenHeight)
		{
			MoveMap(0.0f, -0.4f);
		}
		else
		{
			// 落下速度を移動量に加える
			PlayerMoveY = m_fallPlayerSpeed;
		}
	}

	//梯子移動　　プレイヤーの上下 の場所に梯子があるか
	if ((m_map->GetMapChipParam(m_add.x + m_player->GetRect().GetCenter().x- m_player->GetRect().GetSize().w / 2+ kPullPos, m_add.y + m_player->GetRect().GetCenter().y - m_player->GetRect().GetSize().h / 2 - 2.0f) == ladder1)||
		(m_map->GetMapChipParam(m_add.x + m_player->GetRect().GetCenter().x- m_player->GetRect().GetSize().w / 2+ kPullPos, m_add.y + m_player->GetRect().GetCenter().y - m_player->GetRect().GetSize().h / 2 - 2.0f) == ladder2) &&
		(m_map->GetMapChipParam(m_add.x + m_player->GetRect().GetCenter().x+ m_player->GetRect().GetSize().w / 2- kPullPos, m_add.y + m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h / 2 + 2.0f) == ladder1) ||
		(m_map->GetMapChipParam(m_add.x + m_player->GetRect().GetCenter().x+ m_player->GetRect().GetSize().w / 2- kPullPos, m_add.y + m_player->GetRect().GetCenter().y + m_player->GetRect().GetSize().h / 2 + 2.0f) == ladder2))
	{
		DrawString(500, 40, L"梯子", 0xffffff);
		//上キーで梯子を上がれる
		if (input.IsPressed(InputType::up))
		{
			if (m_isPlayerCenterUD && m_map->GetPos().y < 0.0f)
			{
				m_map->Movement({ 0.0f,kPlayerMoveSpeed });
				m_add += {0.0f, -kPlayerMoveSpeed};
			}
			else
			{
				PlayerMoveY -= kPlayerMoveSpeed;
			}
		}
		//下キーで梯子を下がれる
		if (input.IsPressed(InputType::down))
		{
			if (m_isPlayerCenterUD && camera > Game::kScreenHeight)
			{
				m_map->Movement({ 0.0f,-kPlayerMoveSpeed });
				m_add += {0.0f, kPlayerMoveSpeed};
			}
			else
			{
				PlayerMoveY += kPlayerMoveSpeed;
			}
		}
	}
	else if (m_player->IsJump() == false && m_isPlayerCenterUD && camera > Game::kScreenHeight)
	{
		MoveMap( 0.0f,-0.4f );
	}

	// 移動量に基づいてキャラクタの座標を移動
	MovePlayer(PlayerMoveX, PlayerMoveY);

	m_map->Update();
	m_player->Update();
	
	//エネミー
	float MoveX = kEnemyMoveSpeed, MoveY = 0.0f;
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
			MoveShot(i, kShotSpeed, 0.0f);
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

	//プレイヤーのHPが０になったらゲームオーバーにする
	if (m_hp[Object_Player]->GetHp() <= 0)
	{
		for (auto& shot : m_shots)
		{
			shot->SetExist(false);
		}

		m_updateFunc = &GameplayingScene::FadeOutUpdat;
		m_fadeColor = 0xff0000;
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

