#pragma once
#include "Secne.h"

class InputState;
class SceneManager;

class KeyConfigScene : public Scene
{
private:
    const InputState& m_inputState;
    int m_currentInputIndex;//現在選択中のインデックス
    bool m_isEditing = false;//現在特定のにゅうりょくは編集中です。
public:
    KeyConfigScene(SceneManager& manager,const InputState& input);
    virtual ~KeyConfigScene();
    void Update(const InputState& input);
    void Draw();
};

