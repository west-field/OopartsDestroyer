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
	constexpr float kJumpAcc = 6.0f;//ジャンプ力
	constexpr float kMoveSpeed = 2.0f;
}

CutManStage::CutManStage(SceneManager& manager):Scene(manager),m_updateFunc(&CutManStage::FadeInUpdat)
{
	// プレイヤーの座標を初期化
	//player.x = 320.0f;
	//player.y = 240.0f;
	//// プレイヤーの落下速度を初期化
	//player.fallSpeed = 0.0f;
	//// ジャンプ中フラグを倒す
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
	// プレイヤーの移動処理
	float MoveX, MoveY;

	// 移動量の初期化
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
	//地に足が着いている,足元に梯子がある場合は上キーで梯子を上れる
	if (m_player->IsJump() == false &&
		m_mapData->GetMapChipParam(m_player->GetPos().x, m_player->GetPos().y ) == ladder1 && 
		input.IsPressed(InputType::up))
	{
		m_fallPlayerSpeed = -kJumpAcc;
	}
	// 地に足が着いている場合のみジャンプボタン(ボタン１ or Ｚキー)を見る
	if (m_player->IsJump() == false && input.IsPressed(InputType::junp))
	{
		m_fallPlayerSpeed = -kJumpAcc;
		m_player->SetJump(true);
		m_mapData->ScrollJump();
	}

	// 落下処理
	m_fallPlayerSpeed += 0.3f;

	// 落下速度を移動量に加える
	MoveY = m_fallPlayerSpeed;

	// 移動量に基づいてキャラクタの座標を移動
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

	// キャラクタの左上、右上、左下、右下部分が当たり判定のある
	// マップに衝突しているか調べ、衝突していたら補正する

	// 半分のサイズを算出
	wsize = m_player->GetColSize().w * 0.5f;
	hsize = m_player->GetColSize().h * 0.5f;

	// 左下のチェック、もしブロックの上辺に着いていたら落下を止める
	if (MapHitCheck(m_player->GetPos().x - wsize, m_player->GetPos().y + hsize, Dummy, MoveY) == 3)
	{
		m_fallPlayerSpeed = 0.0f;
	}

	// 右下のチェック、もしブロックの上辺に着いていたら落下を止める
	if (MapHitCheck(m_player->GetPos().x + wsize, m_player->GetPos().y + hsize, Dummy, MoveY) == 3)
	{
		m_fallPlayerSpeed = 0.0f;
	}

	// 左上のチェック、もしブロックの下辺に当たっていたら落下させる
	if (MapHitCheck(m_player->GetPos().x - wsize, m_player->GetPos().y - hsize, Dummy, MoveY) == 4)
	{
		m_fallPlayerSpeed *= -1.0f;
	}

	// 右上のチェック、もしブロックの下辺に当たっていたら落下させる
	if (MapHitCheck(m_player->GetPos().x + wsize, m_player->GetPos().y - hsize, Dummy, MoveY) == 4)
	{
		m_fallPlayerSpeed *= -1.0f;
	}

	// 補正された上下移動成分を加算して実際に移動。あたってなかったらそのまま計算される
	m_player->AddPos({0.0f,MoveY});

	// 後に左右移動成分だけでチェック
	// 左下のチェック
	MapHitCheck(m_player->GetPos().x - hsize, m_player->GetPos().y + hsize, MoveX, Dummy);

	// 右下のチェック
	MapHitCheck(m_player->GetPos().x + hsize, m_player->GetPos().y + hsize, MoveX, Dummy);

	// 左上のチェック
	MapHitCheck(m_player->GetPos().x - hsize, m_player->GetPos().y - hsize, MoveX, Dummy);

	// 右上のチェック
	MapHitCheck(m_player->GetPos().x + hsize, m_player->GetPos().y - hsize, MoveX, Dummy);

	// 左右移動成分を加算
	m_player->AddPos({ MoveX,0.0f });

	// 接地判定
		// キャラクタの左下と右下の下に地面があるか調べる
	if ((m_mapData->GetMapChipParam(m_player->GetPos().x - m_player->GetColSize().w * 0.5f, m_player->GetPos().y + m_player->GetColSize().h * 0.5f + 1.0f) == sky ||
		m_mapData->GetMapChipParam(m_player->GetPos().x - m_player->GetColSize().w * 0.5f, m_player->GetPos().y + m_player->GetColSize().h * 0.5f + 1.0f) == no) &&

		(m_mapData->GetMapChipParam(m_player->GetPos().x + m_player->GetColSize().w * 0.5f, m_player->GetPos().y + m_player->GetColSize().h * 0.5f + 1.0f) == sky ||
		m_mapData->GetMapChipParam(m_player->GetPos().x + m_player->GetColSize().w * 0.5f, m_player->GetPos().y + m_player->GetColSize().h * 0.5f + 1.0f) == no))
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

int CutManStage::MapHitCheck(float X, float Y, float& MoveX, float& MoveY)
{
	float afterX, afterY;
	//移動量を足す
	afterX = X + MoveX;
	afterY = Y + MoveY;
	float blockLeftX = 0.0f, blockTopY = 0.0f, blockRightX = 0.0f, blockBottomY = 0.0f;

	//当たっていたら壁から離す処理を行う、ブロックの左右上下の座標を算出
	blockLeftX = static_cast<float>((int)afterX / Game::ChipSize) * Game::ChipSize;//左　X座標
	blockRightX = static_cast<float>((int)afterX / Game::ChipSize + 1) * Game::ChipSize;//右　X座標

	blockTopY = static_cast<float>((int)afterY / Game::ChipSize) * Game::ChipSize;//上　X座標
	blockBottomY = static_cast<float>((int)afterY / Game::ChipSize + 1) * Game::ChipSize;//下　X座標

	//当たり判定のあるブロックに当たっているか
	if (m_mapData->GetMapChipParam(afterX, afterY) == terrain ||
		m_mapData->GetMapChipParam(afterX, afterY) == wall1 ||
		m_mapData->GetMapChipParam(afterX, afterY) == wall2 ||
		//m_mapData->GetMapChipParam(afterX, afterY) == ladder2 ||
		m_mapData->GetMapChipParam(afterX, afterY) == stone )
	{
		//上に当たっていた場合
		if (MoveY > 0.0f)
		{
			//移動量を補正する
			MoveY = blockTopY - Y - 1.0f;
			//上辺に当たったと返す
			return 3;
		}
		//下に当たっていた場合
		if (MoveY < 0.0f)
		{
			//移動量を補正する
			MoveY = blockBottomY - Y + 1.0f;
			//下辺に当たったと返す
			return 4;
		}
		//左に当たっていた場合
		if (MoveX > 0.0f)
		{
			//移動量を補正する
			MoveX = blockLeftX - X - 1.0f;
			//左辺に当たったと返す
			return 1;
		}
		//右に当たっていた場合
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
	else if (m_mapData->GetMapChipParam(afterX, afterY) == ladder2)
	{
		//上に当たっていた場合
		if (MoveY > 0.0f)
		{
			//移動量を補正する
			MoveY = blockTopY - Y - 1.0f;
			//上辺に当たったと返す
			return 3;
		}
	}
	//どこにも当たらなかったと返す
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
	//右に移動
	else if (input.IsPressed(InputType::right))
	{
		
		{
			PlayerMoveX += kMoveSpeed;
		}
		m_player->SetLeft(false);
	}
	//プレイヤージャンプ処理
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
