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

    const InputState& m_inputState;
    int m_currentInputIndex;//現在選択中のインデックス
    bool m_isEditing = false;//現在特定のにゅうりょくは編集中です。

#ifdef _DEBUG
    int m_numSize = 0;
#endif
};

