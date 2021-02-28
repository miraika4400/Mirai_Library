//=============================================================================
//
// trap�w�b�_ [trap.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _TRAP_H_
#define _TRAP_H_

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

//*****************************
// �N���X��`
//*****************************

// �p�[�e�B�N���N���X
class CTrap : public CModel
{
public:
	//==========================
	// ��
	//==========================

	//==========================
	//�����o�֐�
	//==========================
	CTrap();
	~CTrap();
	static CTrap *Create(const D3DXVECTOR3 pos);
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	bool CollisionCharacter(void); // �v���C���[�Ƃ̓����蔻��
	bool CollisionAttack(void);      // �U���I�u�W�F�N�g�Ƃ̓����蔻��

	//==========================
	// �����o�ϐ�
	//==========================
	static Model m_model;      // ���f���\����
	CCollision*m_pCollision;   // �����蔻��
};
#endif