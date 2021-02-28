//=============================================================================
//
// item�w�b�_ [item.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _ITEM_H_
#define _ITEM_H_

//*****************************
// �C���N���[�h
//*****************************
#include "main.h"
#include "model.h"

//*****************************
// �}�N����`
//*****************************

//*****************************
// �O���錾
//*****************************
class CCollision;
class CBillboard;

//*****************************
// �N���X��`
//*****************************

// �p�[�e�B�N���N���X
class CItem : public CModel
{
public:
	//==========================
	// ��
	//==========================
	// �A�C�e���^�C�v
	typedef enum
	{

		ITEM_NONE = -1,    // ��
		ITEM_ACCELERATION, // ����
		ITEM_ATTACK,       // �U��
		ITEM_TRAP,         // �g���b�v
		ITEM_MAX           // �A�C�e���^�C�v�ő吔
	}ITEM_TYPE;

	//==========================
	//�����o�֐�
	//==========================
	CItem();
	~CItem();
	static CItem *Create(const D3DXVECTOR3 pos);
	static HRESULT Load(void);
	static void Unload(void);
	static void RotationItem(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	void CollisionCharacter(void); // �v���C���[�Ƃ̓����蔻��

	//==========================
	// �����o�ϐ�
	//==========================
	static Model m_model;                 // ���f���\����
	static LPDIRECT3DTEXTURE9 m_pTexture; // �e�N�X�`��
	static D3DXVECTOR3 m_rot;             // �S�A�C�e���̌����𓝈ꂷ��

	CCollision*m_pCollision;   // �����蔻��
	CBillboard* m_pBillboard;  // �{�b�N�X�̒���?�}�[�N
	bool m_bActive;            // �A�C�e�����L����
	int  m_nCntRePop;          // �A�C�e�����|�b�v�J�E���g
	bool m_bExpansion;         // �T�C�Y�g��t���O
};
#endif