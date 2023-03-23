#pragma once
#include "game.h"
#include "Util/Geometry.h"
#include <memory>
#include <vector>

class EnemyFactory;

enum MapLayer
{
	MapLayer_bg,	//�w�i
	MapLayer_map,	//�}�b�v
	MapLayer_enemy,	//�G�l�~�[
	MapLayer_event	//�����蔻��
};

//�}�b�v�̍L���~���C���[���Ȃ̂ŁA2�̃x�N�^�[�����q�ɂ��Ă���
using MapData_t = std::vector<std::vector<unsigned char>>;

class Map
{
public:
	Map(std::shared_ptr<EnemyFactory> enemyFactory, int stage);
	virtual ~Map();
	void Update();
	void Draw();

	/// <summary>
	/// ��ʈړ�
	/// </summary>
	/// <param name="vec">��ʈړ�</param>
	void Movement(Vector2 vec);
	const Position2 GetPos()const { return m_camera; }
	void SetPos(Position2 pos);

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
	const int GetChipId(int layerId, int chipX, int chipY)const;

	/// <summary>
	/// �}�b�v�̃T�C�Y����肷��
	/// </summary>
	/// <param name="width"></param>
	/// <param name="height"></param>
	void GetMapSize(int& width, int& height);

	/// <summary>
	/// ��ʃX�N���[���̌������x�G��\��������
	/// </summary>
	void EnemyPos();

	//�}�b�v�`�b�v�̒l���擾����֐�
	int GetMapChipParam(float X, float Y);
	//�C�x���g�`�b�v�̒l���擾����֐�
	int GetMapEventParam(float X, float Y);
	//�}�b�v�`�b�v�̒l������ʒu��Ԃ�
	Vector2 GetMapChipPos(float X, float Y);
private:
	MapData_t m_mapData;//�}�b�v�f�[�^
	int m_mapWidth;//�}�b�v�̉���
	int m_mapHeight;//�}�b�v�̏c��

	Position2 m_camera;//�ړ�

	int m_stage;//

	int m_handle;//�O���t�B�b�N

	int m_trapH = -1;
	int m_trapIdx = 0;

	std::vector<int> m_enemyPos;
	//std::vector<bool> m_enemyPos;
	std::shared_ptr<EnemyFactory> m_enemies;
	int m_frame = 0;
	int num = 0;
};


