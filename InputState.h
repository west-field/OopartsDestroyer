#pragma once

#include <map>
#include <vector>
#include <string>

enum class InputType
{
	next,//次へ行くボタン
	pause,//ポーズボタン
	prev,//戻る
	up,//上
	down,//下
	left,//左
	right,//右

	junp,//ジャンプ
	shot,//ショット

	max
};

/// <summary>
/// 入力装置カテゴリ
/// </summary>
enum class InputCategory
{
	keybd,
	pad,
	mouse
};

/// <summary>
/// 入力情報
/// </summary>
struct InputInfo
{
	InputCategory cat;//
	int id;
};

//フレンド用にプロトタイプ宣言
class KeyConfigScene;

/// <summary>
/// 入力状態を管理する
/// </summary>
class InputState
{
	friend KeyConfigScene;//KeyConfigSceneにだけ、すべてを見せる。
public:
	InputState();

	/// <summary>
	/// 入力情報を更新する
	/// （注意）毎フレームUpdateを呼ばないと入力状況は更新されない
	/// </summary>
	void Update();

	/// <summary>
	/// 押されてたらtrueになる
	/// </summary>
	/// <param name="type">InputType</param>
	/// <returns>押されていたらtrue,押されてないならfalse</returns>
	bool IsPressed(InputType type) const;

	/// <summary>
	/// 押した瞬間にtrueになる
	/// </summary>
	/// <param name="type">InputType</param>
	/// <returns>押した瞬間ならtrue、それ以外ならfalse</returns>
	bool IsTriggered(InputType type) const;

	/// <summary>
	/// 入力情報を更新する
	/// </summary>
	/// <param name="type">ゲームにおける入力種別</param>
	/// <param name="cat">入力カテゴリ</param>
	/// <param name="id">実際の入力</param>
	void RewriteInputInfo(InputType type, InputCategory cat, int id);

	/// <summary>
	/// 現在編集中のキーコンフィグを確定する
	/// </summary>
	void CommitChangedInputInfo();

	/// <summary>
	/// 現在編集中のキーコンフィグ設定をなかったことにする
	/// </summary>
	void RollbackChangedInputInfo();

	/// <summary>
	/// キーマップをデフォルトにリセットする
	/// </summary>
	void ResetInputInfo();

	/// <summary>
	/// キーマップをファイルに保存する
	/// </summary>
	void SaveKeyInfo()const;

	/// <summary>
	/// キーマップをロードする
	/// </summary>
	void LoadKeyInfo();
private:
	//仮想入力情報と、実際の入力テーブルを作る
	//キー(first)=InputType
	//値(second)=std::vector<InputInfo>
	using InputMap_t = std::map<InputType, std::vector<InputInfo>>;
	InputMap_t inputMapTable_;//実際の入力とゲームボタンの対応テーブル

	InputMap_t tempMapTable_;//書き換え用の一時的なinputMapTable_のコピー いきなりかえてしまわないように
	InputMap_t defaultMapTable_;//リセット用キーマップテーブル

	//入力タイプとその名前の対応テーブル
	std::map<InputType, std::wstring> inputNameTable_;

	std::vector<bool> currentInput_;//現在の入力情報(押しているか押していないか)
	std::vector<bool> lastInput_;//直前の入力情報(直前押しているか押していないか)
};
