////////////////////////////////////////////////////
//
//    ���U���g�N���X�̏���[result.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////


//=============================
// �C���N���[�h
//=============================
#include "result.h"
#include "manager.h"
#include "renderer.h"
#include "polygon.h"
#include "keyboard.h"
#include "mouse.h"
#include "joypad.h"
#include "fade.h"
#include "score.h"
#include "display_on.h"
#include "ranking.h"
#include "sound.h"

//**********************************
// �ÓI�����o�ϐ��錾
//**********************************
LPDIRECT3DTEXTURE9 CResult::m_pTexture = NULL;
CRanking *CResult::m_pRanking = NULL;

//**********************************
// �}�N����`
//**********************************
#define  TITLE_TEXTURE_PATH "data/Textures/Result.png" // �e�N�X�`��

//=============================
// �R���X�g���N�^
//=============================
CResult::CResult()
{
	m_pPolygon = NULL;
}

//=============================
// �f�X�g���N�^
//=============================
CResult::~CResult()
{
}

//=============================
// �N���G�C�g
//=============================
CResult * CResult::Create(void)
{
	// �������̊m��
	CResult *pResult = new CResult;
	// ������
	pResult->Init();
	return pResult;
}

//=============================
// ����������
//=============================
HRESULT CResult::Init(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TITLE_TEXTURE_PATH, &m_pTexture);


	m_pPolygon = CPolygon::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
		D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	m_pPolygon->BindTexture(m_pTexture);

	if (m_pRanking == NULL)
	{
		m_pRanking = CRanking::Create();
	}

	
	return S_OK;
}

//=============================
// �I������
//=============================
void CResult::Uninit(void)
{
	// �e�N�X�`���̉��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	if (m_pPolygon != NULL)
	{
		// �|���S���̏I������
		m_pPolygon->Uninit();

		// �������̉��
		delete m_pPolygon;
		m_pPolygon = NULL;
	}

	if (m_pRanking != NULL)
	{
		m_pRanking = NULL;
	}


	// �J������
	Release();
}


//=============================
// �X�V����
//=============================
void CResult::Update(void)
{
	// �|���S���̍X�V����
	m_pPolygon->Update();

	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_RETURN) || 
		CManager::GetMouse()->GetMouseTrigger(0) || 
		CManager::GetJoypad()->GetJoystickTrigger(3, 0) || CManager::GetJoypad()->GetJoystickTrigger(11, 0))
	{
		CManager::GetSound()->Play(CSound::LABEL_SE_DECISION);
		CDisplayON::Create();
	}
}


//=============================
// �`�揈��
//=============================
void CResult::Draw(void)
{
	// �|���S���̕`�揈��
	m_pPolygon->Draw();
}
