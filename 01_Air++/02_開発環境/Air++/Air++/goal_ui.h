//=============================================================================
//
// goal_ui�w�b�_ [goal_ui.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _GOAL_UI_H_
#define _GOAL_UI_H_

//*****************************
//�C���N���[�h
//*****************************
#include "main.h"
#include "scene2d.h"

//*****************************
//�N���X��`
//*****************************

// �S�[��UI�N���X
class CGoalUi : public CScene2d
{
public:
	//============
	// �����o�֐�
	//============
	CGoalUi();
	~CGoalUi();

	static CGoalUi *Create(int nPlayerNum); // �N���X����
	static HRESULT Load(void);              // �e�N�X�`�����[�h
	static void Unload(void);               // �e�N�X�`���A�����[�h
	
	HRESULT Init(void); // ������
	void Uninit(void);  // �I��
	void Update(void);  // �X�V
	void Draw(void);    // �`��

private:
	//============
	// �����o�ϐ�
	//============
	static LPDIRECT3DTEXTURE9 m_pTexture;        // �e�N�X�`���ւ̃|�C���^
	int m_nPlayerNum;              // �v���C���[�ԍ�
	int m_nCntAnim;                // �A�j���[�V�����J�E���g
	int m_nAnimX;                  // �A�j���[�V����X��
	int m_nAnimY;                  // �A�j���[�V����Y��
};

#endif