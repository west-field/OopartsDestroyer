#pragma once
#include <vector>

//�}�b�v�̍L���~���C���[���Ȃ̂ŁA2�̃x�N�^�[�����q�ɂ��Ă���
using MapData_t = std::vector<std::vector<unsigned char>>;

/// <summary>
/// �X�e�[�W�����Ǘ�����N���X
/// </summary>
class Stage
{
	struct Header
	{
		char id[4];					//�@�t�@�C�����ʎq(FMF_)
		unsigned int size;			//�@�w�b�_���̂������f�[�^�T�C�Y
		unsigned int mapWidth;		//�@�}�b�v�̉���
		unsigned int mapHeight;		//�@�}�b�v�̏c��
		unsigned char chipWidth;	//�@�p�[�c(�Z��1��)�̉���
		unsigned char chipHeight;	//�@�p�[�c(�Z��1��)�̏c��
		unsigned char layerCount;	//�@���C���[��
		unsigned char bitCount;		//�@1�Z��������̃r�b�g��(��8�Ńo�C�g��)

	};
	MapData_t m_mapData;
	int m_mapWidth;
	int m_mapHeight;
public:
	Stage();
	virtual ~Stage();

	/// <summary>
	/// �w�b�_�[��ǂݍ���
	/// </summary>
	/// <param name="filePath">�ǂݍ��ރt�@�C����</param>
	void Load(const wchar_t* filePath);

	/// <summary>
	/// �}�b�v�f�[�^�̎Q�Ƃ�Ԃ�
	/// </summary>
	/// <returns>�}�b�v�f�[�^</returns>
	const MapData_t& GetMapData()const;

	/// <summary>
	/// �}�b�v��ID���擾����֐�
	/// </summary>
	/// <param name="layerId">���C���[</param>
	/// <param name="chipX">�`�b�v�̏ꏊ��</param>
	/// <param name="chipY">�`�b�v�̏ꏊ�c</param>
	/// <returns>�}�b�v��ID</returns>
	const int GetChipId(int layerId,int chipX,int chipY)const;

	/// <summary>
	/// �}�b�v�̃T�C�Y����肷��
	/// </summary>
	/// <param name="width"></param>
	/// <param name="height"></param>
	void GetMapSize(int& width, int& height);
};

