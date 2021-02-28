#ifndef _RANKING_H_
#define _RANKING_H_
//=============================================================================
//
// �����L���O�N���X�w�b�_�[ [ranking.h]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "scene.h"
#include "number.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_NUMBER		(2)				// �i���o�[�̌���
#define MAX_RANKING		(5)				// �����L���O�̐�
#define RANKING_POS_X	(820.0f)		// �����L���O�̉��̍��W
#define RANKING_POS_Y	(210.0f)		// �����L���O�̏c�̍��W
#define RANKING_SIZE_X	(15.0f)			// �����L���O�̉��̃T�C�Y
#define RANKING_SIZE_Y	(20.0f)			// �����L���O�̏c�̃T�C�Y

#define MY_RANKING_SIZE_X	(55.0f)		// �����L���O�̉��̃T�C�Y
#define MY_RANKING_SIZE_Y	(60.0f)		// �����L���O�̏c�̃T�C�Y

//=============================================================================
// �O�u�錾
//=============================================================================
class CNumber;

//=============================================================================
// �����_���[�N���X
//=============================================================================
class CRanking : public CScene
{
public:
	CRanking();				// �R���X�g���N�^
	~CRanking();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);	// ����������
	void Uninit(void);											// �I������
	void Update(void);											// �X�V����
	void Draw(void);											// �`�揈��
	static CRanking *Create(void);								// ����
	void Save(void);											// �t�@�C���o��
	static void Load(void);										// �t�@�C������
	static void SetRanking(int nNumber);						// �����L���O�ɃX�R�A��^����
	void FlashPolygon(void);									// �|���S���̓_��

private:
	CNumber *m_apRanking[MAX_RANKING][MAX_NUMBER];		// ������
	CNumber *m_apMyRanking[MAX_NUMBER];					// �����̃X�R�A�\��
	static int m_nRanking[MAX_RANKING];					// �X�R�A
	int m_nNumRanking;									// �����L���O�̐�
	static int m_nCurrentNum;							// ���݃v���C�����X�R�A
	int m_nFlashFlame = 0;								// �����L���O�̓_�Ńt���[��
	int m_nSubNumber;									// �_�ŗp�ϐ�
	int m_nSubNum;										// �_�ŗp�ϐ�
	static int m_nMyRank;								// �������g�̃X�R�A
};

#endif