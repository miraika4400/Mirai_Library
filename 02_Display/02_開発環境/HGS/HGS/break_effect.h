//=============================================================================
//
// break_effect�w�b�_ [break_effect.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _BREAK_EFFECT_H_
#define _BREAK_EFFECT_H_

//*****************************
// �C���N���[�h
//*****************************
#include "main.h"
#include "scene.h"

//*****************************
// �N���X��`
//*****************************

// �j�󉉏o�N���X
class CBreakEffect : public CScene
{
public:

	//============
	// �����o�֐�
	//============
	CBreakEffect();
	~CBreakEffect();

	// staic
	static CBreakEffect *Create(const D3DXVECTOR3 pos); // �N���X����

	HRESULT Init(void); // ������
	void Uninit(void);  // �I��
	void Update(void);  // �X�V
	void Draw(void);    // �`��

private:
	//============
	// �����o�ϐ�
	//============
	static LPDIRECT3DTEXTURE9 m_pTexture; // �e�N�X�`��
	D3DXVECTOR3 m_size;                   // �T�C�Y
	D3DXVECTOR3 m_pos;                    // ���W
	float m_fMaxsize;                     // �ő�T�C�Y
	float m_fAlfa;                        // �A���t�@�l

};

#endif