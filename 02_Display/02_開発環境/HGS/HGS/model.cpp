//=============================================================================
//
// ���f������ [model.cpp]
// Author :	���V ����
//
//=============================================================================
#include "model.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
//���f���N���X�̃R���X�g���N�^
//=============================================================================
CModel::CModel(int nPriority) :CScene(nPriority)
{
	// �ϐ��̃N���A
	m_model.pBuffMat = NULL;
	m_model.pMesh = NULL;
	m_model.nNumMat = 0;
	m_model.pos = D3DXVECTOR3();
	m_model.size = D3DXVECTOR3();
	m_model.rot = D3DXVECTOR3();
	m_model.nParent = -1;
	memset(&m_model.apTexture, 0, sizeof(m_model.apTexture));
	memset(&m_defMat, 0, sizeof(m_defMat));
}

//=============================================================================
//���f���N���X�̃f�X�g���N�^
//=============================================================================
CModel::~CModel()
{
}

//=============================================================================
//���f���N���X�̃N���G�C�g����
//=============================================================================
CModel * CModel::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	//���f���N���X�̃|�C���^�ϐ�
	CModel *pModel = NULL;

	//�������m��
	pModel = new CModel;

	//���������m�ۂł��Ă�����
	if (pModel != NULL)
	{
		//�����������Ăяo��
		pModel->Init();
		pModel->m_model.pos = pos;
		pModel->m_model.size = size;
	}
	//�������m�ۂɎ��s�����Ƃ�
	else
	{
		return NULL;
	}

	return pModel;
}

//=============================================================================
//���f���N���X�̏���������
//=============================================================================
HRESULT CModel::Init()
{
	//�f�o�C�X���̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�ʒu�̏�����
	m_model.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�����̏�����
	m_model.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�傫���̏�����
	m_model.size = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	return S_OK;
}

//=============================================================================
//���f���N���X�̏I������
//=============================================================================
void CModel::Uninit(void)
{
	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
//���f���N���X�̍X�V����
//=============================================================================
void CModel::Update(void)
{
}

//=============================================================================
//���f���N���X�̕`�揈��
//=============================================================================
void CModel::Draw(void)
{
	//�f�o�C�X���̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxScail;
	D3DMATERIAL9 matDef;	//���݂̃}�e���A���ێ��p
	D3DXMATERIAL*pMat;	//�}�e���A���f�[�^�ւ̃|�C���^

						//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_model.mtxWorld);

	//�T�C�Y�𔽉f
	D3DXMatrixScaling(&mtxScail, m_model.size.x, m_model.size.y, m_model.size.z);
	D3DXMatrixMultiply(&m_model.mtxWorld, &m_model.mtxWorld, &mtxScail);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_model.rot.y, m_model.rot.x, m_model.rot.z);
	D3DXMatrixMultiply(&m_model.mtxWorld, &m_model.mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_model.pos.x, m_model.pos.y, m_model.pos.z);
	D3DXMatrixMultiply(&m_model.mtxWorld, &m_model.mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_model.mtxWorld);

	//���݂̃}�e���A�����擾����
	pDevice->GetMaterial(&matDef);

	if (m_model.pBuffMat != NULL)
	{
		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)m_model.pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)m_model.nNumMat; nCntMat++)
		{
			//�}�e���A���̃A���r�G���g�Ƀf�B�t���[�Y�J���[��ݒ�
			pMat[nCntMat].MatD3D.Ambient = pMat[nCntMat].MatD3D.Diffuse;

			//�}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
			// �e�N�X�`��
			pDevice->SetTexture(0, m_model.apTexture[nCntMat]);
			//���f���p�[�c�̕`��
			m_model.pMesh->DrawSubset(nCntMat);

			pMat[nCntMat] = m_defMat[nCntMat];
		}
	}
	//�ێ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
//���f���N���X�̃��b�V�����̎擾
//=============================================================================
LPD3DXMESH CModel::GetMesh(void) const
{
	return m_model.pMesh;
}

//=============================================================================
//���f���N���X�̈ʒu���̐ݒ�
//=============================================================================
void CModel::SetPos(const D3DXVECTOR3 pos)
{
	m_model.pos = pos;
}

//=============================================================================
//���f���N���X�̈ʒu���̎擾
//=============================================================================
D3DXVECTOR3 CModel::GetPos(void) const
{
	return m_model.pos;
}

//=============================================================================
//���f���N���X�̑傫�����̐ݒ�
//=============================================================================
void CModel::SetSize(const D3DXVECTOR3 size)
{
	m_model.size = size;
}

//=============================================================================
//���f���N���X�̑傫�����̎擾
//=============================================================================
D3DXVECTOR3 CModel::GetSize(void) const
{
	return m_model.size;
}

//=============================================================================
//���f���N���X�̌����̐ݒ�
//=============================================================================
void CModel::SetRot(const D3DXVECTOR3 rot)
{
	m_model.rot = rot;
}

//=============================================================================
//���f���N���X�̌����̎擾
//=============================================================================
D3DXVECTOR3 CModel::GetRot(void) const
{
	return m_model.rot;
}


//=============================================================================
// ���f���̊��蓖��
//=============================================================================
void CModel::BindModel(LPD3DXMESH pMeshModel, LPD3DXBUFFER pBuffMatModel, DWORD nNumMatModel)
{
	m_model.pMesh = pMeshModel;
	m_model.pBuffMat = pBuffMatModel;
	m_model.nNumMat = nNumMatModel;

	for (int nCntMat = 0; nCntMat < (int)m_model.nNumMat; nCntMat++)
	{
		m_defMat[nCntMat] = ((D3DXMATERIAL*)m_model.pBuffMat->GetBufferPointer())[nCntMat];

	}
}

//=============================================================================
// ���f���̃e�N�X�`�����蓖��
//=============================================================================
void CModel::BindTexture(const int nIndex, const LPDIRECT3DTEXTURE9 pTexture)
{
	m_model.apTexture[nIndex] = pTexture;
}

//=============================================================================
// ���[���h�}�g���b�N�X�̐ݒ�
//=============================================================================
void CModel::SetxWorldMatrix(D3DXMATRIX mtxWorld)
{
	m_model.mtxWorld = mtxWorld;
}

//=============================================================================
// ���f�����̎擾
//=============================================================================
CModel::Model * CModel::GetModelData(void)
{
	return &m_model;
}
