////////////////////////////////////////////////////
//
//    LOCUS�N���X�̏���[LOCUS.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////

//******************************
// �C���N���[�h
//******************************
#include "locus.h"
#include "renderer.h"
#include "scene.h"
#include "manager.h"
#include "keyboard.h"
#include "joypad.h"
#include "model.h"

//******************************
// �}�N����`
//******************************
#define TEX_PATH "./data/Textures/locus.png"
#define LIFE_MAX 60 // ����
#define MAINUS_ALPHA 0.05f // �A���t�@�l�����炷

//******************************
// �O���錾
//******************************
LPDIRECT3DTEXTURE9 CLocus::m_pTexture = NULL;

//===================================
// �R���X�g���N�^
//===================================
CLocus::CLocus(int nPriority) :CScene(OBJTYPE_SYSTEM)
{
	m_pVtxBuff = NULL;
	m_col = { 1.0f,1.0f,1.0f,1.0f };
	m_bAddMode = false;
	m_nLife = 0;
	D3DXMatrixIdentity(&m_mtxWorld);
}

//===================================
// �f�X�g���N�^
//===================================
CLocus::~CLocus()
{
}

//===================================
// �N���G�C�g�֐�
//===================================
CLocus * CLocus::Create(D3DXVECTOR3 aPos[NUM_VERTEX /2], D3DXVECTOR3 aPosOld[NUM_VERTEX/2] , D3DXCOLOR col)
{
	// �������̊m��
	CLocus *pLocus;
	pLocus = new CLocus;
	// ������
	pLocus->Init();
	pLocus->SetPos(aPos, aPosOld);
	pLocus->SetColor(col);

	return pLocus;
}

//==================================
// �e�N�X�`���̃��[�h
//==================================
HRESULT CLocus::Load(void)
{

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEX_PATH, &m_pTexture);

	return S_OK;
}

//==================================
// �e�N�X�`���̃A�����[�h
//==================================
void CLocus::Unload(void)
{
	// �e�N�X�`���̉��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//===================================
// ����������
//===================================
HRESULT CLocus::Init(void)
{
	VERTEX_3D *pVtx;// ���_���|�C���^

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D)*NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	// �����o�ϐ��̏�����
	m_col = { 1.0f,1.0f,1.0f,1.0f };

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = VEC3_ZERO;
	pVtx[1].pos = VEC3_ZERO;
	pVtx[2].pos = VEC3_ZERO;
	pVtx[3].pos = VEC3_ZERO;

	// �e�N�X�`��UV���W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	for (int nCnt = 0; nCnt < NUM_VERTEX; nCnt++)
	{
		// �F�̐ݒ�
		pVtx[nCnt].col = m_col;
		// �@��
		pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	}

	// �A�����b�N
	m_pVtxBuff->Unlock();

	// ���Z�������[�h�ɂ���
	m_bAddMode = true;

	return S_OK;
}

//===================================
// �I������
//===================================
void CLocus::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	Release();
}

//===================================
// �X�V����
//===================================
void CLocus::Update(void)
{
	// ���C�t���J�E���g
	m_nLife++;

	m_col.a -= MAINUS_ALPHA;
	SetColor(m_col);
	if (m_nLife > LIFE_MAX)
	{// �����𒴂�����
		// ����
		Uninit();
	}
}

//===================================
// �`�揈��
//===================================
void CLocus::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �J�����O
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); 
	// ���C�g������
	pDevice->SetRenderState(D3DRS_LIGHTING, false);

	// ���Z���[�h�̎�
	if (m_bAddMode)
	{
		// ���Z����
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	// �}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));
	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	pDevice->SetTexture(0, m_pTexture);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	// �e�N�X�`��������
	pDevice->SetTexture(0, NULL);

	// ���Z���[�h�̎�
	if (m_bAddMode)
	{
		// �ʏ퍇���ɖ߂�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}

	// �J�����O
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	// ���C�g������
	pDevice->SetRenderState(D3DRS_LIGHTING, true);
}

//===================================
// ���W�̃Z�b�g
//===================================
void CLocus::SetPos(const D3DXVECTOR3 aPos[NUM_VERTEX / 2], const D3DXVECTOR3 aPosOld[NUM_VERTEX / 2])
{
	VERTEX_3D *pVtx;// ���_���|�C���^

	// ���b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = aPos[0];
	pVtx[1].pos = aPos[1];
	pVtx[2].pos = aPosOld[0];
	pVtx[3].pos = aPosOld[1];

	// �A�����b�N
	m_pVtxBuff->Unlock();
}

//===================================
// �F�̃Z�b�g
//===================================
void CLocus::SetColor(const D3DXCOLOR col)
{
	VERTEX_3D *pVtx;// ���_���|�C���^

	m_col = col;

	// ���b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// �A�����b�N
	m_pVtxBuff->Unlock();
}