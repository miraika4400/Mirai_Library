//===================================================
//
//    �w�i�N���X�̏���[bg.cpp]
//    Author:���V ����
//
//====================================================

//**********************************
//�C���N���[�h
//**********************************
#include "bg.h"
#include "manager.h"
#include "renderer.h"
#include "scene3d.h"
#include "camera.h"
#include "game.h"

//*****************************
// �}�N����`
//*****************************
#define MODEL_PATH "./data/Models/bg.x" // ���f���̃p�X
#define SIZE       D3DXVECTOR3(110.0f,110.0f ,110.0f) // �T�C�Y
#define ADD_ROT D3DXVECTOR3(0.0f,0.0005f,0.0f)         // ��������
//*****************************
// �ÓI�����o�ϐ��錾
//*****************************
CModel::Model CBg::m_model = {};  //���b�V�����ւ̃|�C���^


//==================================
// �R���X�g���N�^
//==================================
CBg::CBg() : CModel(OBJTYPE_BG)
{
	
}

//==================================
// �f�X�g���N�^
//==================================
CBg::~CBg()
{
}

//==================================
// �N���G�C�g
//==================================
CBg * CBg::Create(void)
{
	// �������̊m��
	CBg *pBg = new CBg;
	// ����������
	pBg->Init();

	return pBg;
}

//==================================
// ���[�h
//==================================
HRESULT CBg::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(MODEL_PATH, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_model.pBuffMat, NULL, &m_model.nNumMat, &m_model.pMesh);

	// �e�N�X�`���ǂݍ���
	if (m_model.nNumMat != 0)
	{
		D3DXMATERIAL*pMat = (D3DXMATERIAL*)m_model.pBuffMat->GetBufferPointer();
		for (int nCnt = 0; nCnt < (int)m_model.nNumMat; nCnt++)
		{
			if (pMat[nCnt].pTextureFilename != NULL)
			{
				char cPath[64] = {};
				sprintf(cPath, "./data/Textures/%s", pMat[nCnt].pTextureFilename);
				// �e�N�X�`���̐���
				D3DXCreateTextureFromFile(pDevice, cPath, &m_model.apTexture[nCnt]);
			}
		}
	}

	return S_OK;
}

//==================================
// �A�����[�h
//==================================
void CBg::Unload(void)
{
	//���b�V���̔j��
	if (m_model.pMesh != NULL)
	{
		m_model.pMesh->Release();
		m_model.pMesh = NULL;
	}

	//�}�e���A���̔j��
	if (m_model.pBuffMat != NULL)
	{
		m_model.pBuffMat->Release();
		m_model.pBuffMat = NULL;
	}
}

//==================================
// ����������
//==================================
HRESULT CBg::Init(void)
{
	if (FAILED(CModel::Init()))
	{
		return E_FAIL;
	}

	// ���f�����蓖��
	BindModel(m_model.pMesh, m_model.pBuffMat, m_model.nNumMat);

	//�e�N�X�`���̊��蓖��
	for (int nCnt = 0; nCnt < (int)m_model.nNumMat; nCnt++)
	{
		BindTexture(nCnt, m_model.apTexture[nCnt]);
	}

	// �T�C�Y�̐ݒ�
	SetSize(SIZE);
	
	return S_OK;
}

//==================================
// �I������
//==================================
void CBg::Uninit(void)
{
	CModel::Uninit();
}

//==================================
// �X�V����
//==================================
void CBg::Update(void)
{
	// ��������
	SetRot(GetRot() + ADD_ROT);
}

//==================================
// �`�揈��
//==================================
void CBg::Draw(void)
{
	//�f�o�C�X���̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	
	// ���C�g������
	pDevice->SetRenderState(D3DRS_LIGHTING, false);
	
	CModel::Draw();

	// ���C�g������
	pDevice->SetRenderState(D3DRS_LIGHTING, true);
}
