//=============================================================================
//
// player�w�b�_ [player.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _PLAYER_H_
#define _PLAYER_H_

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
class CDestination;

//*****************************
// �N���X��`
//*****************************

// �p�[�e�B�N���N���X
class CPlayer : public CCharacter
{
public:
	//==========================
	// ��
	//==========================

	//==========================
	//�����o�֐�
	//==========================
	CPlayer();
	~CPlayer();

	static CPlayer *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, int nPlayerNum); // �N���X����
	static HRESULT Load(void); // ���f���ǂݍ���
	static void Unload(void);  // ���f���A�����[�h
	static CPlayer* GetPlayer(int nPlayerNum); // �v���C���[�擾����*�����F�擾�������v���C���[�ԍ�

	HRESULT Init(void); // ������
	void Uninit(void);  // �I��
	void Update(void);  // �X�V
	void Draw(void);    // �`��

	// �h���t�g�t���O�̎擾�E�Z�b�g
	bool GetDriftLeft(void) { return m_bDriftLeft; }
	void SetDriftLeft(bool bDrift) { m_bDriftLeft = bDrift; }
	bool GetDriftRight(void) { return m_bDriftRight; }
	void SetDriftRight(bool bDrift) { m_bDriftRight = bDrift; }

	// �v���C���[�ԍ��̎擾����
	int GetPlayerNum(void) { return m_nPlayerNum; }
	// �v���C���[�̌����擾
	float GetDir(void) { return m_fDir; }
	// �ړ��ڕW�̎擾����
	CDestination * GetDest(void) { return m_pDest; }

	// �S�[�����̏���
	void GoalAction(void);
private:
	void Move(void);      // �ړ�����
	void UseItem(void);   // �A�C�e���g�p�̏���
	void Direction(void); // �����̊Ǘ�����
	void Drift(void);     // �h���t�g����
	void FallAction(void); // �������̃A�N�V����

	//==========================
	//�����o�ϐ�
	//==========================
	static Model m_model;   // ���f���\����
	static Shader m_shader; // �V�F�[�_�[�\����
	CDestination * m_pDest; // �ړ��ڕW�N���X
	int m_nPlayerNum;       // �v���C���[ID
	float m_fDir;           // �v���C���[�̐���
	bool m_bDriftLeft;      // �h���t�g��
	bool m_bDriftRight;     // �h���t�g�E
};
#endif