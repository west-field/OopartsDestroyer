#pragma once

#include <map>
#include <vector>
#include <string>

enum class InputType
{
	next,//次へ行くボタン
	prev,//前に戻るボタン
	pause,//ポーズボタン
	keyconf,//キーコンフィグ
	up,//上
	down,//下
	left,//左
	right,//右

	junp,//ジャンプ
	shot,//ショット
	rapidFire,//連射

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

class KeyConfigScene;

/// <summary>
/// 入力状態を管理する
/// </summary>
class InputState
{
	friend KeyConfigScene;//KeyConfigSceneにだけ、すべてを見せる。
private:
	std::map<InputType,std::vector<InputInfo>> m_inputMapTable;

	//入力
	std::map<InputType, std::wstring> m_inputNameTable;

	std::vector<bool> m_currentInput;
	std::vector<bool> m_lastInput;

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
	void RewriteInputInfo(InputType type,InputCategory cat,int id);
};

