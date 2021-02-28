//=============================================================================
//
// rank_ui�w�b�_ [rank_ui.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _RANK_UI_H_
#define _RANK_UI_H_

//*****************************
//�C���N���[�h
//*****************************
#include "main.h"
#include "scene2d.h"

//*****************************
//�N���X��`
//*****************************

// �����NUI�N���X
class CRankUi : public CScene2d
{
public:
	//============
	// ��
	//============

	// �g���e�N�X�`���̎�ނ̗�
	typedef enum
	{
		RANK_UI_TEX_NORMAL,
		RANK_UI_TEX_RESULT,
		RANK_UI_TEX_MAX
	}RANK_UI_TEX_TYPE;

	//============
	// �����o�֐�
	//============
	CRankUi();
	~CRankUi();

	static CRankUi *Create(int nPlayerNum); // �N���X����
	static HRESULT Load(void);            // �e�N�X�`�����[�h
	static void Unload(void);             // �e�N�X�`���A�����[�h

	HRESULT Init(void); // ������
	void Uninit(void);  // �I��
	void Update(void);  // �X�V
	void Draw(void);    // �`��

private:
	//============
	// �����o�ϐ�
	//============
	static LPDIRECT3DTEXTURE9 m_apTexture[RANK_UI_TEX_MAX];        // �e�N�X�`���ւ̃|�C���^

	D3DXVECTOR3 m_resultPos;       // ���U���g���̍��W
	D3DXVECTOR3 m_resultSize;      // ���U���g���̃T�C�Y

	int m_nCntAnim;                // �A�j���[�V�����J�E���g
	int m_nAnimX;                  // �A�j���[�V����X��
	int m_nAnimY;                  // �A�j���[�V����Y��
	int m_nPlayerNum;              // �v���C���[�ԍ�
};

#endif