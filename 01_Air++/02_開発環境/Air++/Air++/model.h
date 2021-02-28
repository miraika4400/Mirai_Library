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
#define MAX_MATERIAL 64 // �e�N�X�`���ő吔

//=============================================================================
//���f���N���X
//=============================================================================
class CModel : public CScene
{
public:
	//============
	//��
	//============
	// �ėp���f���^�C�v
	typedef enum
	{
		GENERAL_MODEL_SPHERE = 0, // ��
		GENERAL_MODEL_BOX,        // �{�b�N�X
		GENERAL_MODEL_CONE,       // �~��
		GENERAL_MODEL_MAX         // �^�C�v��
	}GENERAL_MODEL_TYPE;

	//============
	//�\����
	//============
	// ���f���f�[�^
	typedef struct
	{
		LPDIRECT3DTEXTURE9 apTexture[MAX_MATERIAL]; // �e�N�X�`��
		LPD3DXMESH pMesh;                           // ���b�V�����ւ̃|�C���^
		LPD3DXBUFFER pBuffMat;                      // �}�e���A�����ւ̃|�C���^
		DWORD nNumMat;                              // �}�e���A�����̐�
		D3DXVECTOR3 pos;                            // �ʒu
		D3DXVECTOR3 size;                           // �傫��
		D3DXVECTOR3 rot;                            // ����
		D3DXMATRIX mtxWorld;                        // ���[���h�}�g���b�N�X
		D3DXMATERIAL defMat[MAX_MATERIAL];          // �}�e���A���ۊǗp
		D3DXCOLOR diffuseCol[MAX_MATERIAL];         // �J���[*�g�U
		int nParent;                                // �e�̃C���f�b�N�X
	}Model;

	//============
	// �����o�֐�
	//============
	CModel(int nPliority = OBJTYPE_NONE);
	~CModel();

	static CModel *Create(D3DXVECTOR3 pos , GENERAL_MODEL_TYPE modelType  ,D3DXVECTOR3 size = D3DXVECTOR3(1.0f, 1.0f, 1.0f)); // �N���X����*���W,�ėp���f���^�C�v,�T�C�Y
	static HRESULT Load(void); // ���f���ǂݍ���
	static void Unload(void);  // ���f���A�����[�h

	HRESULT Init(void); // ������
	void Uninit(void);  // �I��
	void Update(void);  // �X�V
	void Draw(void);    // �`��
	LPD3DXMESH GetMesh(void)const; // ���b�V���̎擾����

	void BindModel(LPD3DXMESH pMeshModel, LPD3DXBUFFER pBuffMatModel, DWORD nNumMatModel); // ���f���̊��蓖��
	void BindTexture(const int nIndex, const LPDIRECT3DTEXTURE9 pTexture);                 // �e�N�X�`���̊��蓖��

	// �Q�b�^�[�E�Z�b�^�[
	// ���W
	void SetPos(const D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPos(void)const;
	// �T�C�Y
	void SetSize(const D3DXVECTOR3 size);
	D3DXVECTOR3 GetSize(void)const;
	// ��]
	void SetRot(const D3DXVECTOR3 rot);
	D3DXVECTOR3 GetRot(void)const;
	// ���[���h�}�g���b�N�X
	void SetxWorldMatrix(D3DXMATRIX mtxWorld);
	Model *GetModelData(void);

private:
	//============
	// �����o�ϐ�
	//============
	static Model m_aGeneralModel[GENERAL_MODEL_MAX]; // �ėp���f��
	Model m_model;                                   // ���f���f�[�^
};
#endif 