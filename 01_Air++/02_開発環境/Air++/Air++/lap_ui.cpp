////////////////////////////////////////////////////
//
//    Start�N���X�̏���[Start.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////

//******************************
// �C���N���[�h
//******************************
#include "lap_ui.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"

//*****************************
// �}�N����`
//*****************************

// ���W
#define POS   D3DXVECTOR3(1100.0f, 140.0f, 0.0f)       // ���W
#define SIZE  D3DXVECTOR3(604.0f/7, 400.0f/7, 0.0f)   // �T�C�Y*�摜�T�C�Y�ɍ��킹��

#define MAX_ANIMATION_X 3      // �A�j���[�V������ ��
#define MAX_ANIMATION_Y 1      // �A�j���[�V������ �c

#define TEXTURE_PATH "./data/Textures/Lap_ui.png"    //�e�N�X�`���̃p�X

//******************************
// �ÓI�����o�ϐ��錾
//******************************
LPDIRECT3DTEXTURE9  CLapUi::m_pTexture = NULL; // �e�N�X�`���|�C���^

//******************************
// �R���X�g���N�^
//******************************
CLapUi::CLapUi() :CScene2d(OBJTYPE_UI)
{
	m_nCntAnim = 0;
	m_nAnimX = 0;
	m_nAnimY = 0;
	m_nPlayerNum = 0;
}

//******************************
// �f�X�g���N�^
//******************************
CLapUi::~CLapUi()
{
}

//******************************
// �N���G�C�g
//******************************
CLapUi * CLapUi::Create(int nPlayerNum)
{
	// �������̊m��
	CLapUi *pStart;
	pStart = new CLapUi;

	// �v���C���[�ԍ��̎擾
	pStart->m_nPlayerNum = nPlayerNum;
	// ������
	pStart->Init();

	return pStart;
}

//******************************
// �e�N�X�`���̃��[�h
//******************************
HRESULT CLapUi::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_PATH, &m_pTexture);

	return S_OK;
}

//******************************
// �e�N�X�`���̃A�����[�h
//******************************
void CLapUi::Unload(void)
{
	// �e�N�X�`���̉������
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}


//******************************
// ����������
//******************************
HRESULT CLapUi::Init(void)
{
	if (FAILED(CScene2d::Init()))
	{
		return E_FAIL;
	}

	SetPos(POS);
	// �T�C�Y�̐ݒ�
	SetSize(SIZE);

	// �e�N�X�`�����蓖��
	BindTexture(m_pTexture);


	// �e�N�X�`��UV���W�̏�����
	m_nAnimX = 0;
	m_nAnimY = 0;

	// UV���W�̐ݒ�
	D3DXVECTOR2 uv[NUM_VERTEX];
	float fu = 1.0f / MAX_ANIMATION_X;
	float fv = 1.0f / MAX_ANIMATION_Y;

	uv[0] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY);
	uv[1] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY);
	uv[2] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY + fv);
	uv[3] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY + fv);

	// UV���W�Z�b�g
	SetTextureUV(uv);

	return S_OK;
}

//******************************
// �I������
//******************************
void CLapUi::Uninit(void)
{

	CScene2d::Uninit();
}

//******************************
// �X�V����
//******************************
void CLapUi::Update(void)
{
	// �e�N�X�`��UV���W�̏�����
	m_nAnimX = CPlayer::GetPlayer(m_nPlayerNum)->GetRankData().nLap;

	if (m_nAnimX > MAX_ANIMATION_X-1)
	{
		m_nAnimX = MAX_ANIMATION_X - 1;
	}
	// UV���W�̐ݒ�
	D3DXVECTOR2 uv[NUM_VERTEX];
	float fu = 1.0f / MAX_ANIMATION_X;
	float fv = 1.0f / MAX_ANIMATION_Y;

	uv[0] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY);
	uv[1] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY);
	uv[2] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY + fv);
	uv[3] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY + fv);

	// UV���W�Z�b�g
	SetTextureUV(uv);

	if (CGame::GetState() == CGame::GAME_RESULT)
	{// ���U���g������
		Uninit();
	}
}

//******************************
// �`�揈��
//******************************
void CLapUi::Draw(void)
{
	CScene2d::Draw();
}
