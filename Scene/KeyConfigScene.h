#pragma once
#include "Secne.h"
#include <string>

class InputState;
class SceneManager;

/// <summary>
/// キーコンフィグシーン
/// </summary>
class KeyConfigScene : public Scene
{
public:
    KeyConfigScene(SceneManager& manager, const InputState& input);
    virtual ~KeyConfigScene();
    void Update(const InputState& input);
    void Draw();
private:
    std::wstring PadName(int id);
    std::wstring KeyName(int id);
    std::wstring MouseName(int id);

    const InputState& inputState_;
    int currentInputIndex_;//現在選択中のインデックス
    bool isEditing = false;//現在特定のにゅうりょくは編集中です。
};

