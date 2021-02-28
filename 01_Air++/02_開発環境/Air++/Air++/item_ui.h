#pragma once
//=============================================================================
//
// item_ui�w�b�_ [item_ui.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _ITEM_UI_H_
#define _ITEM_UI_H_

//*****************************
// �C���N���[�h
//*****************************
#include "main.h"
#include "scene.h"
#include "item.h"

//*****************************
// �O���錾
//*****************************
class CPolygon;

//*****************************
// �}�N����`
//*****************************

//*****************************
// �N���X��`
//*****************************

// �A�C�e��UI���S�N���X
class CItemUi : public CScene
{
public:
	//============
	// �����o�֐�
	//============
	CItemUi();
	~CItemUi();

	static CItemUi *Create(int nPlayerNum); // �N���X����
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
	static LPDIRECT3DTEXTURE9 m_pFrameTex;                  // �e�N�X�`�� �A�C�e��UI�t���[��
	static LPDIRECT3DTEXTURE9 m_apTexture[CItem::ITEM_MAX]; // �e�N�X�`�� �A�C�e���{��
	static char *m_apItemPath[CItem::ITEM_MAX];             // �A�C�e���{�̃e�N�X�`���̃p�X
	int m_nPlayerNum;                                       // �v���C���[�ԍ�
	D3DXVECTOR3 m_pos;                                      // ���S���W
	CPolygon*m_pFramePolygon;                               // �t���[��
	CPolygon*m_pItemPolygon;                                // �A�C�e��
};

#endif