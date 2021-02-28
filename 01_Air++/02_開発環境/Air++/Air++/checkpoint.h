//=============================================================================
//
// checkpoint�w�b�_ [checkpoint.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _CHECKPOINT_H_
#define _CHECKPOINT_H_

//*****************************
// �C���N���[�h
//*****************************
#include "main.h"
#include "scene.h"
#include "course.h"
//*****************************
// �}�N����`
//*****************************
#define MAX_CHECKPOINT_NUM 256

//*****************************
// �O���錾
//*****************************
class CCollision;

//*****************************
// �N���X��`
//*****************************

// �p�[�e�B�N���N���X
class CCheckPoint : public CScene
{
public:
	//============
	// ��ԗ�
	//============

	//============
	// �����o�֐�
	//============
	CCheckPoint();

	~CCheckPoint();
	static CCheckPoint *Create(void); // �N���X����
	
	HRESULT Init(void); // ������
	void Uninit(void);  // �I��
	void Update(void);  // �X�V
	void Draw(void);    // �`��

	// �R���W�����̎擾
	CCollision*GetCollision(int nInex) { return m_pCollision[nInex]; }
	// �`�F�b�N�|�C���g���̎擾
	int GetCheckPointNum(void) { return m_nNumPoint; }

private:
	void SaveCheckpoint(void); // �e�L�X�g�t�@�C���Ƀ`�F�b�N�|�C���g����������
	void LoadCheckpoint(void); // �e�L�X�g�t�@�C���Ƀ`�F�b�N�|�C���g����������

	// �����o�ϐ�
	int m_nNumPoint;                             // �`�F�b�N�|�C���g�̐�
	CCollision*m_pCollision[MAX_CHECKPOINT_NUM]; // �R���W�����N���X�̃|�C���^
	D3DXVECTOR3 m_pointPos[MAX_CHECKPOINT_NUM];  // �`�F�b�N�|�C���g�̍��W
};

#endif