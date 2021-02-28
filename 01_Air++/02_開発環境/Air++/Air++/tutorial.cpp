////////////////////////////////////////////////////
//
//    tutorial�̏���[tutorial.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////


//=============================
// �C���N���[�h
//=============================
#include "tutorial.h"
#include "manager.h"
#include "renderer.h"
#include "polygon.h"
#include "keyboard.h"
#include "mouse.h"
#include "joypad.h"
#include "fade.h"

//**********************************
// �ÓI�����o�ϐ��錾
//**********************************
LPDIRECT3DTEXTURE9 CTutorial::m_pTexture[TUTORIAL_NUM] = {};

//**********************************
// �}�N����`
//**********************************
#define  TITLE_TEXTURE_1_PATH "./data/Textures/tutorial_1.png" // �e�N�X�`��
#define  TITLE_TEXTURE_2_PATH "./data/Textures/tutorial_2.png" // �e�N�X�`��
#define  TITLE_TEXTURE_3_PATH "./data/Textures/tutorial_3.png" // �e�N�X�`��

//=============================
// �R���X�g���N�^
//=============================
CTutorial::CTutorial()
{
	m_pPolygon = NULL;
	m_nNumTutorial = 0;
}

//=============================
// �f�X�g���N�^
//=============================
CTutorial::~CTutorial()
{
}

//=============================
// �N���G�C�g
//=============================
CTutorial * CTutorial::Create(void)
{
	// �������̊m��
	CTutorial *pTitle = new CTutorial;
	// ������
	pTitle->Init();
	return pTitle;
}

//=============================
// ����������
//=============================
HRESULT CTutorial::Init(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TITLE_TEXTURE_1_PATH, &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, TITLE_TEXTURE_2_PATH, &m_pTexture[1]);
	D3DXCreateTextureFromFile(pDevice, TITLE_TEXTURE_3_PATH, &m_pTexture[2]);

	m_pPolygon = CPolygon::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
		D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	m_pPolygon->BindTexture(m_pTexture[0]);
	return S_OK;
}

//=============================
// �I������
//=============================
void CTutorial::Uninit(void)
{
	for (int nCntTex = 0; nCntTex < TUTORIAL_NUM; nCntTex++)
	{
		// �e�N�X�`���̉��
		if (m_pTexture[nCntTex] != NULL)
		{
			m_pTexture[nCntTex]->Release();
			m_pTexture[nCntTex] = NULL;
		}
	}
	if (m_pPolygon != NULL)
	{
		// �|���S���̏I������
		m_pPolygon->Uninit();

		// �������̉��
		delete m_pPolygon;
		m_pPolygon = NULL;
	}

	// �J������
	Release();
}


//=============================
// �X�V����
//=============================
void CTutorial::Update(void)
{
	// �|���S���̍X�V����
	m_pPolygon->Update();

	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_RETURN) ||
		CManager::GetMouse()->GetMouseTrigger(0) ||
		CManager::GetJoypad()->GetJoystickTrigger(3, 0))
	{
		m_nNumTutorial++;
		if (m_nNumTutorial >= TUTORIAL_NUM)
		{
			CManager::GetFade()->SetFade(CManager::MODE_GAME);
		}
		else
		{
			m_pPolygon->BindTexture(m_pTexture[m_nNumTutorial]);
		}
	}
	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_BACKSPACE) ||
		CManager::GetJoypad()->GetJoystickTrigger(2, 0))
	{
		m_nNumTutorial--;
		if (m_nNumTutorial < 0)
		{
			m_nNumTutorial = 0;
		}

		m_pPolygon->BindTexture(m_pTexture[m_nNumTutorial]);
	}
}


//=============================
// �`�揈��
//=============================
void CTutorial::Draw(void)
{
	// �|���S���̕`�揈��
	m_pPolygon->Draw();
}
