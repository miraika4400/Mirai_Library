//=============================================================================
//
// stan_effect�w�b�_ [stan_effect.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _STAN_EFFECT_H_
#define _STAN_EFFECT_H_

//*****************************
//�C���N���[�h
//*****************************
#include "main.h"
#include "billboard.h"

//*****************************
//�N���X��`
//*****************************

// �S�[��UI�N���X
class CStanEffect : public CBillboard
{
public:
	//============
	// �����o�֐�
	//============
	CStanEffect();
	~CStanEffect();

	static void SetEffect(D3DXVECTOR3 pos); // �N���X����
	static CStanEffect *Create(D3DXVECTOR3 pos); // �N���X����
	static HRESULT Load(void);                      // �e�N�X�`�����[�h
	static void Unload(void);                       // �e�N�X�`���A�����[�h

	HRESULT Init(void); // ������
	void Uninit(void);  // �I��
	void Update(void);  // �X�V
	void Draw(void);    // �`��

private:
	//============
	// �����o�ϐ�
	//============
	static LPDIRECT3DTEXTURE9 m_pTexture; // �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3 m_move;      // �ړ���
	int m_nLife;             // ����
};

#endif