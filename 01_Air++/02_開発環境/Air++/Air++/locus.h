//=============================================================================
//
// LOCUS�w�b�_ [LOCUS.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _LOCUS_H_
#define _LOCUS_H_

//*****************************
//�C���N���[�h
//*****************************
#include "main.h"
#include "scene.h"

//*****************************
//�N���X��`
//*****************************

//2d�|���S���N���X
class CLocus : public CScene
{
public:
	// �����o�֐�
	CLocus(int nPliority = OBJTYPE_NONE);
	~CLocus();
	static CLocus *Create(D3DXVECTOR3 aPos[NUM_VERTEX /2], D3DXVECTOR3 aPosOld[NUM_VERTEX / 2], D3DXCOLOR col);
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	LPDIRECT3DTEXTURE9 GetTexture(void) { return m_pTexture; }
	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }

	// ���S���W�̃Z�b�g�A�擾
	void SetPos(const D3DXVECTOR3 aPos[NUM_VERTEX / 2], const D3DXVECTOR3 aPosOld[NUM_VERTEX / 2]);

	// �J���[�̃Z�b�g�A�擾
	void SetColor(const D3DXCOLOR col);
	D3DXCOLOR GetColor(void) { return m_col; }

	void SetAddMode(bool bBool) { m_bAddMode = bBool; }
private:
	// �����o�ϐ�
	static LPDIRECT3DTEXTURE9		m_pTexture;	       // �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	       // ���_�o�b�t�@�ւ̃|�C���^
	D3DXCOLOR m_col;                           // �F
	D3DXMATRIX  m_mtxWorld;                     // ���[���h�}�g���b�N�X
	bool m_bAddMode;                     // ���Z������
	int m_nLife;                         // ����
};

#endif