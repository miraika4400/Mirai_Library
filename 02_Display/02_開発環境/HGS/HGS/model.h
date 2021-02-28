//=============================================================================
//
// ���f������ [model.h]
// Author : ���V ����
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

//=============================================================================
//�C���N���[�h�t�@�C��
//=============================================================================
#include "scene.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define MAX_TEXTURE 64

//=============================================================================
//���f���N���X
//=============================================================================
class CModel : public CScene
{
public:
	//============
	//�\����
	//============
	// ���f���f�[�^
	typedef struct
	{
		LPDIRECT3DTEXTURE9 apTexture[MAX_TEXTURE]; // �e�N�X�`��
		LPD3DXMESH pMesh;                          //���b�V�����ւ̃|�C���^
		LPD3DXBUFFER pBuffMat;                     //�}�e���A�����ւ̃|�C���^
		DWORD nNumMat;                             //�}�e���A�����̐�
		D3DXVECTOR3 pos;                           //�ʒu
		D3DXVECTOR3 size;                          //�傫��
		D3DXVECTOR3 rot;                           //����
		D3DXMATRIX mtxWorld;                       //���[���h�}�g���b�N�X
		int nParent;                               // �e�̃C���f�b�N�X
	}Model;

	//============
	// �����o�֐�
	//============
	CModel(int nPliority = OBJTYPE_NONE);
	~CModel();

	static CModel *Create(D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3 size = D3DXVECTOR3(1.0f, 1.0f, 1.0f));

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	LPD3DXMESH GetMesh(void)const;

	void SetPos(const D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPos(void)const;
	void SetSize(const D3DXVECTOR3 size);
	D3DXVECTOR3 GetSize(void)const;
	void SetRot(const D3DXVECTOR3 rot);
	D3DXVECTOR3 GetRot(void)const;
	void BindModel(LPD3DXMESH pMeshModel, LPD3DXBUFFER pBuffMatModel, DWORD nNumMatModel);
	void BindTexture(const int nIndex, const LPDIRECT3DTEXTURE9 pTexture);
	void SetxWorldMatrix(D3DXMATRIX mtxWorld);
	Model *GetModelData(void);

private:
	//============
	// �����o�ϐ�
	//============
	Model m_model;              // ���f���f�[�^
	D3DXMATERIAL m_defMat[128]; // �}�e���A���ۊǗp
};
#endif 