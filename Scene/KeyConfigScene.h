#pragma once
#include "Secne.h"
#include <string>

class InputState;
class SceneManager;

/// <summary>
/// �L�[�R���t�B�O�V�[��
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
    int m_currentInputIndex;//���ݑI�𒆂̃C���f�b�N�X
    bool m_isEditing = false;//���ݓ���̂ɂイ��傭�͕ҏW���ł��B

#ifdef _DEBUG
    int m_numSize = 0;
#endif
};

