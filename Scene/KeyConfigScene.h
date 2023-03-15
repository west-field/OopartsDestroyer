#pragma once
#include "Secne.h"

class InputState;
class SceneManager;

/// <summary>
/// �L�[�R���t�B�O�V�[��
/// </summary>
class KeyConfigScene : public Scene
{
private:
    const InputState& inputState_;
    int currentInputIndex_;//���ݑI�𒆂̃C���f�b�N�X
    bool isEditing = false;//���ݓ���̂ɂイ��傭�͕ҏW���ł��B
public:
    KeyConfigScene(SceneManager& manager, const InputState& input);
    virtual ~KeyConfigScene();
    void Update(const InputState& input);
    void Draw();
};

