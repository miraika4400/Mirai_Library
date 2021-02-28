#ifndef _PLAYER_H_
#define _PLAYER_H_
//=============================================================================
//
// �v���C���[�N���X�w�b�_�[ [player.h]
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
#define DECISION_PLAYER_SIZE_X	(30.0f)		// �����蔻��
#define DECISION_PLAYER_SIZE_Y	(30.0f)		// �����蔻��
#define PLAYER_SIZE_X			(40.0f)		// �v���C���[�̃T�C�Y
#define PLAYER_SIZE_Y			(40.0f)		// �v���C���[�̃T�C�Y

//=============================================================================
// �O���錾
//=============================================================================
class CScene2d;

//=============================================================================
// �v���C���[�N���X
//=============================================================================
class CPlayer : public CScene2d
{
public:
	typedef enum
	{
		// �v���C���[�̏��
		PLAYER_STATE_NONE = 0,		// �����l
		PLAYER_STATE_NORMAL,		// �ʏ���
		PLAYER_STATE_REVIVE,		// ������
		PLAYER_STATE_LASER,			// ���[�U�[���ˎ�
		PLAYER_STATE_MAX
	}PLAYER_STATE;

	CPlayer();			// �R���X�g���N�^
	~CPlayer();			// �f�X�g���N�^

	HRESULT Init(void);				// ����������
	void Uninit(void);														// �I������
	void Update(void);														// �X�V����
	void Draw(void);														// �`�揈��

	static HRESULT Load(void);												// �e�N�X�`���̃��[�h
	static void UnLoad(void);												// �e�N�X�`���̃A�����[�h
	static CPlayer * Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// �|���S������

	void HitDamage(int nCount);					// �_���[�W����
	void PlayerControl(void);					// �v���C���[����
	void UpdateState(void);						// �v���C���[��ԍX�V
	void Shot(void);							// �e�̔��ˊ֐�

	static bool GetAlive(void) { return m_bAlive; };                          // ��������
private:
	PLAYER_STATE m_State;					// �v���C���[�̏��
	int m_StateCount;						// �v���C���[�̏�ԃJ�E���^�[
	int m_nBulletCnt;						// �o���b�g�Ԋu�̃J�E���^�[
	static LPDIRECT3DTEXTURE9 m_pTexture;	// �e�N�X�`���̃|�C���^
	int m_nLife;							// ���@�̗̑�
	int m_nStock;							// �X�g�b�N
	D3DXVECTOR3 m_Inertia;					// ����
	D3DXVECTOR3 m_move;						// �ړ�
	static bool m_bAlive;                          // ��������

};

#endif