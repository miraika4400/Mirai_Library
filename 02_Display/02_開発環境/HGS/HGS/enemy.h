#ifndef _ENEMY_H_
#define _ENEMY_H_
//=============================================================================
//
// �G�l�~�[�N���X�w�b�_�[ [enemy.h]
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
#define MAX_ENEMY				(256)			// �G�̍ő吔
#define ENEMY_SIZE_X			(25)			// �G�l�~�[�̃T�C�Y
#define ENEMY_SIZE_Y			(25)			// �G�l�~�[�̃T�C�Y
#define MAX_ENEMY_TEXTURE		(1)				// �G�l�~�[�̃e�N�X�`��
#define ENEMY_SCORE_NORMAL		(1)				// �ʏ�X�R�A

//=============================================================================
// �O���錾
//=============================================================================
class CScene2d;
class CLight;

//=============================================================================
// �G�l�~�[�N���X
//=============================================================================
class CEnemy : public CScene2d
{
public:
	CEnemy();			// �R���X�g���N�^
	~CEnemy();			// �f�X�g���N�^

	HRESULT Init(void);		// ����������
	void Uninit(void);												// �I������
	void Update(void);												// �X�V����
	void Draw(void);												// �`�揈��
	static HRESULT Load(void);										// �e�N�X�`�����[�h
	static void UnLoad(void);										// �e�N�X�`���A�����[�h

	void HitDamage(int nCount);										// �G�l�~�[�̃q�b�g��
	static CEnemy * Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);		// �G�l�~�[����
	bool Collision(void);											// �����蔻��
	void Rotation(void);											// �|���S���̉�]
private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_ENEMY_TEXTURE];		// �e�N�X�`�����̃|�C���^
	int m_nLife;							// �̗�
	int m_nSpeed;							// ����
	float m_fRasian;
	float m_fDiagonal;
	float m_fAngle;
};

#endif