//=============================================================================
//
// attack�w�b�_ [attack.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _ATTACK_H_
#define _ATTACK_H_

//*****************************
// �C���N���[�h
//*****************************
#include "main.h"
#include "character.h"

//*****************************
// �}�N����`
//*****************************

//*****************************
// �O���錾
//*****************************

//*****************************
// �N���X��`
//*****************************

// �p�[�e�B�N���N���X
class CAttack : public CCharacter
{
public:
	//==========================
	// ��
	//==========================


	//==========================
	// �����o�֐�
	//==========================
	CAttack();
	~CAttack();

	static CAttack *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, int nRank, int nUseID); // �N���X����*����:���W,��������]�p�x,����
	static HRESULT Load(void); // ���f���ǂݍ���
	static void Unload(void);  // ���f���A�����[�h

	HRESULT Init(void); // ������
	void Uninit(void);  // �I��
	void Update(void);  // �X�V
	void Draw(void);    // �`��

private:
	void Move(void);        // �ړ�����
	void DistManager(void); // �ړ��ڕW�̊Ǘ�
	void CollisionCharacter(void);// �v���C���[�ENPC�Ƃ̓����蔻��*�L�����N�^�[�N���X�̍X�V�ŌĂ΂�Ă�
	void FallAction(void); // �������̃A�N�V����

	//==========================
	// �����o�ϐ�
	//==========================
	static Model m_model;            // ���f���\����
	static Shader m_shader;          // �V�F�[�_�[�\����
	CCollision * m_pDIstCollision;   // �ړ��ڕW�l�����蔻��
	CCollision * m_pTargetCollision; // �U���ڕW�̓����蔻��
	int m_nPointNum;                 // �R�[�X�|�C���g�z��Ǘ��p
	int m_nRank;                     // �������l�̏���
	int m_nLife;                     // ����
	int m_nUseID;                    // �g�����l�̃I�u�W�F�N�g�ԍ�
	int m_nRoot;                     // ���[�g
};
#endif