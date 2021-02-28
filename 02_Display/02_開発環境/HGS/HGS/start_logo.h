#ifndef _START_LOGO_H_
#define _START_LOGO_H_

//=============================================================================
//
// �X�^�[�g���S�w�b�_�[	[start_logo.h]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "scene2d.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define START_LOGO_POS_Y			(600.0f)		// ���W
#define START_LOGO_SIZE_X			(400.0f)		// �T�C�Y
#define START_LOGO_SIZE_Y			(30.0f)			// �T�C�Y
#define MAX_START_LOGO_TEXTURE		(1)				// �e�N�X�`���̐�

//=============================================================================
// �O���錾
//=============================================================================
class CScene2d;

//=============================================================================
// �^�C�g��Bg�N���X
//=============================================================================
class CStartLogo : public CScene2d
{
public:
	CStartLogo();			// �R���X�g���N�^
	~CStartLogo();			// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);					// ����������
	void Uninit(void);													// �I������
	void Update(void);													// �X�V����
	void Draw(void);													// �`�揈��

	static CStartLogo * Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// �|���S������
	static HRESULT Load(void);													// �e�N�X�`�����[�h
	static void UnLoad(void);													// �e�N�X�`���A�����[�h
	void FlashPolygon(void);
private:
	D3DXVECTOR3 m_Pos;												// ���W
	D3DXVECTOR3 m_move;												// �ړ���
	D3DXVECTOR3 m_size;												// �T�C�Y
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_START_LOGO_TEXTURE];	// �e�N�X�`�����̃|�C���^
	int m_nFlashFlame;												// �_�ł̃t���[��
	int m_nSubNumber;												// �J���[���Z
	int m_nSubNum;													// �J���[�̌��Z��
	bool m_bDisappearFlag;											// �������Ƃ��̃t���O
};

#endif