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
#include "player.h"
#include "grid.h"
#include "enemy.h"

//**********************************
// �ÓI�����o�ϐ��錾
//**********************************
LPDIRECT3DTEXTURE9 CTutorial::m_pTexture[TUTORIAL_NUM] = {};
CPlayer *CTutorial::m_pPlayer = NULL;
CEnemy *CTutorial::m_pEnemy = NULL;

//**********************************
// �}�N����`
//**********************************
#define  TITLE_TEXTURE_1_PATH "./data/Textures/Tutorial.png" // �e�N�X�`��
#define  TITLE_TEXTURE_2_PATH "./data/Textures/tutorial001.png" // �e�N�X�`��
#define  TITLE_TEXTURE_3_PATH "./data/Textures/tutorial002.png" // �e�N�X�`��

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
	CTutorial *pTutorial = new CTutorial;
	// ������
	pTutorial->Init();
	return pTutorial;
}

//=============================
// ����������
//=============================
HRESULT CTutorial::Init(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//// �v���C���[���g���Ă��Ȃ��Ƃ�
	//if (m_pPlayer == NULL)
	//{
	//	// �C���X�^���X����
	//	m_pPlayer = CPlayer::Create(D3DXVECTOR3(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 0.0f),
	//		D3DXVECTOR3(PLAYER_SIZE_X, PLAYER_SIZE_Y, 0.0f));
	//}

	//// �O���b�h����
	//CGrid::CreateAll();

	//if (m_pEnemy == NULL)
	//{
	//	// �G�l�~�[����
	//	m_pEnemy = CEnemy::Create(D3DXVECTOR3(940.0f, 180.0f, 0.0f), D3DXVECTOR3(ENEMY_SIZE_X, ENEMY_SIZE_Y, 0.0f));
	//}

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TITLE_TEXTURE_1_PATH, &m_pTexture[0]);
	//D3DXCreateTextureFromFile(pDevice, TITLE_TEXTURE_2_PATH, &m_pTexture[1]);
	//D3DXCreateTextureFromFile(pDevice, TITLE_TEXTURE_3_PATH, &m_pTexture[2]);

	if (m_pPolygon == NULL)
	{
		m_pPolygon = CPolygon::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
			D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		m_pPolygon->BindTexture(m_pTexture[0]);
	}
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

	//// �v���C���[���g���Ă�����
	//if (m_pPlayer != NULL)
	//{
	//	m_pPlayer = NULL;
	//}

	//if (m_pEnemy != NULL)
	//{
	//	m_pEnemy = NULL;
	//}

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
	if (m_pPolygon != NULL)
	{

		// �|���S���̍X�V����
		m_pPolygon->Update();
	}
	//if (CManager::GetKeyboard()->GetKeyTrigger(DIK_RETURN) ||
	//	CManager::GetMouse()->GetMouseTrigger(0) ||
	//	CManager::GetJoypad()->GetJoystickTrigger(3, 0))
	//{
	//	m_nNumTutorial++;
	//	if (m_nNumTutorial >= TUTORIAL_NUM)
	//	{
	//		CManager::GetFade()->SetFade(CManager::MODE_GAME);
	//	}
	//	else
	//	{
	//		m_pPolygon->BindTexture(m_pTexture[m_nNumTutorial]);
	//	}
	//}
	//if (CManager::GetKeyboard()->GetKeyTrigger(DIK_BACKSPACE) ||
	//	CManager::GetJoypad()->GetJoystickTrigger(2, 0))
	//{
	//	m_nNumTutorial--;
	//	if (m_nNumTutorial < 0)
	//	{
	//		m_nNumTutorial = 0;
	//	}

	//	m_pPolygon->BindTexture(m_pTexture[m_nNumTutorial]);
	//}
	if (CManager::GetJoypad()->GetJoystickTrigger(11, 0)|| CManager::GetKeyboard()->GetKeyTrigger(DIK_RETURN))
	{
		CManager::GetFade()->SetFade(CManager::MODE_GAME);
	}
}

//=============================
// �`�揈��
//=============================
void CTutorial::Draw(void)
{
	if (m_pPolygon != NULL)
	{
		// �|���S���̕`�揈��
		m_pPolygon->Draw();
	}
}

//=============================
// �`���[�g���A���ݒ�
//=============================
void CTutorial::SetGame(void)
{
}

//=============================
// �v���C���[���
//=============================
CPlayer * CTutorial::GetPlayer(void)
{
	return m_pPlayer;
}
