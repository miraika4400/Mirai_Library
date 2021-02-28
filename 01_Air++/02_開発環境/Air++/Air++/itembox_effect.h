//=============================================================================
//
// itembox_effect�w�b�_ [itembox_effect.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _ITEMBOX_EFFECT_H_
#define _ITEMBOX_EFFECT_H_

//*****************************
//�C���N���[�h
//*****************************
#include "main.h"
#include "scene3d.h"

//*****************************
//�N���X��`
//*****************************

// �S�[��UI�N���X
class CItemBoxEffect : public CScene3d
{
public:
	//============
	// �����o�֐�
	//============
	CItemBoxEffect();
	~CItemBoxEffect();

	static void SetEffect(D3DXVECTOR3 pos, float fCharaAngle); // �N���X����
	static CItemBoxEffect *Create(D3DXVECTOR3 pos, float fCharaAngle); // �N���X����
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
	float m_fCharaAngle; // ���������L�����N�^�[�̌���
	D3DXVECTOR3 m_move;      // �ړ���
	int m_nLife;             // ����
};

#endif