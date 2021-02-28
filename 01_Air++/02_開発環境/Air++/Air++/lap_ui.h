//=============================================================================
//
// lap_ui�w�b�_ [lap_ui.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _LAP_UI_H_
#define _LAP_UI_H_

//*****************************
// �C���N���[�h
//*****************************
#include "main.h"
#include "scene2d.h"

//*****************************
// �O���錾
//*****************************

//*****************************
// �}�N����`
//*****************************

//*****************************
// �N���X��`
//*****************************

// ���b�v��UI���S�N���X
class CLapUi : public CScene2d
{
public:
	//============
	// �����o�֐�
	//============
	CLapUi();
	~CLapUi();

	static CLapUi *Create(int nPlayerNum); // �N���X����
	static HRESULT Load(void);               // �e�N�X�`�����[�h
	static void Unload(void);                // �e�N�X�`���A�����[�h

	HRESULT Init(void); // ������
	void Uninit(void);  // �I��
	void Update(void);  // �X�V
	void Draw(void);    // �`��

private:
	//============
	// �����o�ϐ�
	//============
	static LPDIRECT3DTEXTURE9 m_pTexture;	 // �e�N�X�`���ւ̃|�C���^
	int m_nCntAnim;       // �A�j���[�V�����J�E���g
	int m_nAnimX;         // �A�j���[�V����X��
	int m_nAnimY;         // �A�j���[�V����Y��
	int m_nPlayerNum;
};

#endif