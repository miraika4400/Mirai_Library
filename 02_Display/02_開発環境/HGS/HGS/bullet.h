#ifndef _BULLET_H_
#define _BULLET_H_
//=============================================================================
//
// �o���b�g�N���X�w�b�_�[ [bullet.h]
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
#define MAX_BULLET				(512)		// �e�̍ő吔
#define BULLET_SIZE_X			(20)		// �e�̉���
#define BULLET_SIZE_Y			(20)		// �e�̏c��
#define MAX_BULLET_TEXTURE		(1)			// �e�N�X�`���̍ő吔
#define BULLET_SPEED			(15.0f)		// �X�s�[�h

//=============================================================================
// �O���錾
//=============================================================================
class CScene2d;

//=============================================================================
// �o���b�g�N���X
//=============================================================================
class CBullet : public CScene2d
{
public:
	CBullet();			// �R���X�g���N�^
	~CBullet();			// �f�X�g���N�^

	HRESULT Init(void);						// ����������
	void Uninit(void);																// �I������
	void Update(void);																// �X�V����
	void Draw(void);																// �`�揈��
	static HRESULT Load(void);																// �e�N�X�`�����[�h
	static void UnLoad(void);																// �e�N�X�`���A�����[�h
	bool Collision(void);										// �����蔻��

	static CBullet * Create(																// �|���S������
		D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size,
		int nLife);
private:
	D3DXVECTOR3 m_move;												// �ړ���
	int m_nLife;													// ��������
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_BULLET_TEXTURE];		// �e�N�X�`�����̃|�C���^
};

#endif