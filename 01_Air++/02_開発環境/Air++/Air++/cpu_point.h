//=============================================================================
//
// cpupoint�w�b�_ [cpupoint.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _CPUPOINT_H_
#define _CPUPOINT_H_

//*****************************
// �C���N���[�h
//*****************************
#include "main.h"
#include "scene.h"
#include "course.h"
//*****************************
// �}�N����`
//*****************************
#define MAX_CPUPOINT_NUM 256 // CPU�|�C���g�ő吔
#define ROOT_NUM 2           // ���[�g����

//*****************************
// �O���錾
//*****************************


//*****************************
// �N���X��`
//*****************************

// CPU�p�R�[�X�f�[�^�N���X
class CCpuPoint : public CScene
{
public:
	//============
	// ��ԗ�
	//============
	// �|�C���g�̃^�C�v*�C���R�[�X������p
	typedef enum
	{
		POINT_TYPE_NORMAL = 0, // �m�[�}�� 
		POINT_TYPE_IN_LEFT,    // �����C���R�[�X��
		POINT_TYPE_IN_RIGHT,   // �E���C���R�[�X��
		POINT_TYPE_MAX,        // �^�C�v��
	}POINTTYPE;

	//============
	// �����o�֐�
	//============
	// CPU�p�R�[�X�f�[�^�\����
	typedef struct
	{
		D3DXVECTOR3 pos;        // ���W
		float fAngle;           // �p�x
		POINTTYPE inType;       // �C���R�[�X������p
	}CpuPointData;

	//============
	// �����o�֐�
	//============
	CCpuPoint();
	~CCpuPoint();
	static CCpuPoint *Create(void); // �N���X����

	HRESULT Init(void); // ������
	void Uninit(void);  // �I��
	void Update(void);  // �X�V
	void Draw(void);    // �`��

	D3DXVECTOR3 GetNearPos(int nRoot, D3DXVECTOR3 pos); // �������̍��W����ł��߂����W�̎擾
	int GetNearPosIndex(int nRoot, D3DXVECTOR3 pos); // �������̍��W����ł��߂����W�̃C���f�b�N�X�̎擾
	// �|�C���g���̎擾
	int GetPointNum(int nRoot) { return m_anNumPoint[nRoot]; }

	// �|�C���g���̎擾
	CpuPointData GetPointData(int nRoot,int nIndex) { return m_aCpuPoint[nRoot][nIndex]; }
private:
	void SavePoint(void); // �e�L�X�g�t�@�C���Ƀ|�C���g����������
	void LoadPoint(void); // �e�L�X�g�t�@�C���̃|�C���g��ǂݍ���

	//============
	// �����o�ϐ�
	//============
	static char *m_apPath[ROOT_NUM];           // �e�L�X�g�t�@�C���̃p�X
	int m_anNumPoint[ROOT_NUM];                           // �|�C���g�̐�
	int m_nCntSet;                             // �ݒu���̃J�E���g
	CpuPointData m_aCpuPoint[ROOT_NUM][MAX_CPUPOINT_NUM]; // CPU�p�R�[�X�f�[�^�\����
	int m_nCpuPointNum;                        // �R�[�X�f�[�^�z��Ǘ��p
};

#endif