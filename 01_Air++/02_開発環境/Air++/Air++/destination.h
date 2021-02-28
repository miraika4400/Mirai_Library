//=============================================================================
//
// destination�w�b�_ [destination.h]
// Author : ���V ����
//
//=============================================================================

//��d�C���N���[�h�h�~
#ifndef _DESTNATION_H_
#define _DESTNATION_H_

//*****************************
// �C���N���[�h
//*****************************
#include "main.h"
#include "scene.h"
//*****************************
// �}�N����`
//*****************************
#define DISTANCE_PLAYER_DDRIFT  -300                   // �v���C���[�Ƃ̋����̃h���t�g��
#define DISTANCE_PLAYER_DEFAULT -200                   // �v���C���[�Ƃ̋����̊�{�l
#define DISTANCE_PLAYER_INIT    -10                    // �v���C���[�Ƃ̋����̏���

//*****************************
// �N���X��`
//*****************************
class CModel;

//*****************************
// �N���X��`
//*****************************

// �����蔻��N���X
class CDestination
{
public:
	//�����o�֐�
	CDestination();
	~CDestination();
	static CDestination *Create(const int nPlayerNum, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot = VEC3_ZERO);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// ���W�̎擾�E�Z�b�g
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	// ���b�g�̎擾�E�Z�b�g
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; m_rotDest = rot; }
	// ���b�g�̖ڕW�l�̃Z�b�g
	void SetRotDest(D3DXVECTOR3 rot) { m_rotDest = rot; }

	// �ړ��ʂ̎擾�E�Z�b�g
	D3DXVECTOR3  GetMove(void) { return m_move; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	// �d�̓t���O�̎擾�E�Z�b�g
	bool GetActiveGravity(void) { return m_bGravity; }
	void SetActiveGravity(bool bBool) { m_bGravity = bBool; }
	// �v���C���[�ԍ��̎擾
	int GetPlayerNum(void) { return m_nPlayerNum; }

	// �v���C���[�Ƃ̋����̎擾�E�Z�b�g
	float GetDistancePlayer(void) { return m_fDistancePlayer; }
	void SetDistancePlayer(float fDistance);
	// �v���C���[�Ƃ̋����̖ڕW�l�̎擾�E�Z�b�g
	float GetDistanceDest(void) { return m_fDistanceDest; }
	void SetDistanceDest(float fDistance) { m_fDistanceDest = fDistance; }

private:
	void MoveControll(void);
	void Direction(void);

	// �����o�ϐ�
	D3DXVECTOR3 m_pos;         // ���W
	D3DXVECTOR3 m_rot;         // ���b�g
	D3DXVECTOR3 m_rotDest;     // ���b�g�ڕW�l
	D3DXVECTOR3 m_move;        // �ړ���

	int m_nPlayerNum;          // �ǂ̃v���C���[�̖ڕW��
	D3DXVECTOR3 m_gravityVec;  // �d��
	bool m_bGravity;           // �d�̓t���O
	float m_fDistancePlayer;   // �v���C���[�Ƃ̋���
	float m_fDistanceDest;     // �v���C���[�Ƃ̖ڕW�l
	bool m_bGoal;              // �S�[���t���O
#ifdef _DEBUG
							   // �f�o�b�O�p

	void CreateMesh(void);

	LPD3DXMESH m_pMeshModel;	//���b�V�����ւ̃|�C���^
	LPD3DXBUFFER m_pBuffMatModel;	//�}�e���A�����ւ̃|�C���^
	DWORD m_nNumMatModel;	//�}�e���A�����̐�
	CModel*pModel;
#endif
};

#endif