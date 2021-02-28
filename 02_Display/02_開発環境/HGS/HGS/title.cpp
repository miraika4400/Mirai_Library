////////////////////////////////////////////////////
//
//    �^�C�g���N���X�̏���[title.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////


//=============================
// �C���N���[�h
//=============================
#include "title.h"
#include "manager.h"
#include "renderer.h"
#include "polygon.h"
#include "keyboard.h"
#include "mouse.h"
#include "joypad.h"
#include "fade.h"
#include "sound.h"
#include "start_logo.h"

//**********************************
// �ÓI�����o�ϐ��錾
//**********************************
LPDIRECT3DTEXTURE9 CTitle::m_pTexture = NULL;

//**********************************
// �}�N����`
//**********************************
#define  TITLE_TEXTURE_PATH "data/Textures/title_screen.png" // �e�N�X�`��

//=============================
// �R���X�g���N�^
//=============================
CTitle::CTitle()
{
	m_pPolygon = NULL;
}

//=============================
// �f�X�g���N�^
//=============================
CTitle::~CTitle()
{
}

//=============================
// �N���G�C�g
//=============================
CTitle * CTitle::Create(void)
{
	// �������̊m��
	CTitle *pTitle = new CTitle;
	// ������
	pTitle->Init();

	return pTitle;
}

//=============================
// ����������
//=============================
HRESULT CTitle::Init(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TITLE_TEXTURE_PATH, &m_pTexture);

	m_pPolygon = CPolygon::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
		D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	m_pPolygon->BindTexture(m_pTexture);

	// �^�C�g�����S
	CStartLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, START_LOGO_POS_Y, 0.0f),
		D3DXVECTOR3(START_LOGO_SIZE_X, START_LOGO_SIZE_Y, 0.0f));



	
	return S_OK;
}

//=============================
// �I������
//=============================
void CTitle::Uninit(void)
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


	// �J������
	Release();
}


//=============================
// �X�V����
//=============================
void CTitle::Update(void)
{
	// �|���S���̍X�V����
	m_pPolygon->Update();

	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_RETURN) || 
		CManager::GetMouse()->GetMouseTrigger(0) || 
		CManager::GetJoypad()->GetJoystickTrigger(3, 0)|| CManager::GetJoypad()->GetJoystickTrigger(11, 0))
	{
		CManager::GetSound()->Play(CSound::LABEL_SE_DECISION);
		CManager::GetFade()->SetFade(CManager::MODE_TUTORIAL);
	}
}


//=============================
// �`�揈��
//=============================
void CTitle::Draw(void)
{
	// �|���S���̕`�揈��
	m_pPolygon->Draw();
}
