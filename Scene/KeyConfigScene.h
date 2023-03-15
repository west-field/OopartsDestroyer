#pragma once
#include "Secne.h"

class InputState;
class SceneManager;

/// <summary>
/// キーコンフィグシーン
/// </summary>
class KeyConfigScene : public Scene
{
private:
    const InputState& inputState_;
    int currentInputIndex_;//現在選択中のインデックス
    bool isEditing = false;//現在特定のにゅうりょくは編集中です。
public:
    KeyConfigScene(SceneManager& manager, const InputState& input);
    virtual ~KeyConfigScene();
    void Update(const InputState& input);
    void Draw();
};

