//=============================================================================
//
// item_point�w�b�_ [item_point.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _ITEM_POINT_H_
#define _ITEM_POINT_H_

//*****************************
// �C���N���[�h
//*****************************
#include "main.h"
#include "scene.h"

//*****************************
// �}�N����`
//*****************************
#define MAX_ITEMPOINT_NUM 256 // ITEM�|�C���g�ő吔

//*****************************
// �O���錾
//*****************************


//*****************************
// �N���X��`
//*****************************

// CPU�p�R�[�X�f�[�^�N���X
class CItemPoint : public CScene
{
public:
	//============
	// ��ԗ�
	//============

	//============
	// �����o�֐�
	//============
	// CPU�p�R�[�X�f�[�^�\����

	//============
	// �����o�֐�
	//============
	CItemPoint();
	~CItemPoint();
	static CItemPoint *Create(void); // �N���X����

	HRESULT Init(void); // ������
	void Uninit(void);  // �I��
	void Update(void);  // �X�V
	void Draw(void);    // �`��

						// �A�C�e�����̎擾
	int GetItemNum(void) { return m_nItemNum; }

	// �A�C�e�����W�̎擾 *�����F�z��ԍ�
	D3DXVECTOR3 GetItemPos(int nIndex) { return m_aItemPos[nIndex]; }

private:
	void SaveText(void); // �e�L�X�g�t�@�C���Ƀ|�C���g����������
	void LoadText(void); // �e�L�X�g�t�@�C���̃|�C���g��ǂݍ���

						 //============
						 // �����o�ϐ�
						 //============
	D3DXVECTOR3 m_aItemPos[MAX_ITEMPOINT_NUM]; // �A�C�e�����W
	int m_nItemNum;                            // �A�C�e����
};

#endif