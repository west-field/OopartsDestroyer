#pragma once
#include <array>
#include "Geometry.h"

enum class BgGraph
{
	BgGraph_1,
	BgGraph_2,
	BgGraph_3,
	BgGraph_4,
	BgGraph_5,
	BgGraph_Max
};

class Background
{
private:
	~Background();
	//�R�s�[��������֎~����
	Background(const Background&) = delete;
	void operator= (const Background&) = delete;

	/// <summary>
	/// �w�i�̃��[�h
	/// </summary>
	/// <param name="id">�摜��</param>
	/// <param name="fileName">�t�@�C����</param>
	/// <returns>�w�i�n���h��</returns>
	int Load(BgGraph id,const wchar_t* fileName);

	struct BackImg
	{
		int handle = -1;//�摜�̃n���h��
		float scrollSpeed;//�X�N���[���X�s�[�h
		Size imgSize;
	};
	std::array<BackImg, static_cast<int>(BgGraph::BgGraph_Max)> bgImgs;//�w�i

	int scrollx = 0;//���ɃX�N���[��
public:
	Background();
	/// <summary>
	/// BackImg�g�p�҂�GetInstance()��ʂ����Q�Ƃ��炵�����p�ł��Ȃ�
	/// </summary>
	/// <returns>���̂̎Q�Ƃ�Ԃ�</returns>
	static Background& GetInstance()
	{
		static Background instance;
		return instance;
	}
	/// <summary>
	/// ������
	/// </summary>
	void Init();
	/// �X�V
	void Update();
	/// �\��
	void Draw();
	///�}�b�v�̈ꕔ�����\������
	void Bg();
};

