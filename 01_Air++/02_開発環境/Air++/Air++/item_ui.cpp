////////////////////////////////////////////////////
//
//    FeverLogo�̏���[FeverLogo.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////


//=============================
// �C���N���[�h
//=============================
#include "item_ui.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "number.h"
#include "polygon.h"

//**********************************
// �}�N����`
//**********************************
#define TEXTURE_PATH_FRAME "./data/Textures/Item_ui_Frame.png"            // �e�N�X�`���̃p�X
#define FRAME_SIZE   D3DXVECTOR3( 80.0f, 80.0f, 0.0f)               // �A�C�e���t���[��

// �ʒu
#define POS_PLAYER1_1 D3DXVECTOR3( 0                   + 150.0f, 0                 + 140.0f, 0.0f)  // ��ʕ������ĂȂ��Ƃ�
#define POS_PLAYER1_2 D3DXVECTOR3( 0                   + 150.0f, 0                 + 140.0f, 0.0f)  // ��ʂ��ɕ����Ă���Ƃ��̃v���C���[1
#define POS_PLAYER1_4 D3DXVECTOR3( 0                   + 150.0f, 0                 + 140.0f, 0.0f)  // ��ʂ��l�ɕ����Ă���Ƃ��̃v���C���[1
#define POS_PLAYER2_2 D3DXVECTOR3( 0                   + 150.0f, (SCREEN_HEIGHT/2) + 140.0f, 0.0f)  // ��ʂ��ɕ����Ă���Ƃ��̃v���C���[2
#define POS_PLAYER2_4 D3DXVECTOR3((SCREEN_WIDTH/2)     + 150.0f, 0                 + 140.0f, 0.0f)  // ��ʂ��l�ɕ����Ă���Ƃ��̃v���C���[2
#define POS_PLAYER3   D3DXVECTOR3( 0                   + 150.0f, (SCREEN_HEIGHT/2) + 140.0f, 0.0f)  // �v���C���[3
#define POS_PLAYER4   D3DXVECTOR3((SCREEN_WIDTH/2)     + 150.0f, (SCREEN_HEIGHT/2) + 140.0f, 0.0f)  // �v���C���[4

// ���U���g���̃T�C�Y
#define RESULT_NUMBER_SIZE D3DXVECTOR3( 15.0f*2.5f, 30.0f*2.5f, 0.0f)     // �����T�C�Y*�摜�̉𑜓x�ɍ��킹��

//**********************************
// �ÓI�����o�ϐ��錾
//**********************************
LPDIRECT3DTEXTURE9 CItemUi::m_pFrameTex = {};
LPDIRECT3DTEXTURE9 CItemUi::m_apTexture[CItem::ITEM_MAX] = {};
 char *CItemUi:: m_apItemPath[CItem::ITEM_MAX] = 
 {
	 { "./data/Textures/item_ui_speed.png" },
	 { "./data/Textures/item_ui_attack.png" },
	 { "./data/Textures/item_ui_trap.png" },
 };

//=============================
// �R���X�g���N�^
//=============================
CItemUi::CItemUi() :CScene(OBJTYPE_UI)
{
	// �ϐ��̃N���A
	m_nPlayerNum = 0;                         // �v���C���[�ԍ�
	m_pos = VEC3_ZERO;                        // ���S���W
	m_pFramePolygon = NULL;                   // �t���[��
	m_pItemPolygon = NULL;                    // �A�C�e���{��
}

//=============================
// �f�X�g���N�^
//=============================
CItemUi::~CItemUi()
{
}

//=============================
// �N���G�C�g
//=============================
CItemUi * CItemUi::Create(int nPlayerNum)
{
	// �������̊m��
	CItemUi *pItemUi = new CItemUi;
	// �v���C���[�ԍ��̑��
	pItemUi->m_nPlayerNum = nPlayerNum;
	// ������
	pItemUi->Init();
	return pItemUi;
}

//==================================
// �e�N�X�`���̃��[�h
//==================================
HRESULT CItemUi::Load(void)
{

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_PATH_FRAME, &m_pFrameTex);

	for (int nCnt = 0; nCnt < CItem::ITEM_MAX; nCnt++)
	{
		D3DXCreateTextureFromFile(pDevice, m_apItemPath[nCnt], &m_apTexture[nCnt]);
	}
	return S_OK;
}

//==================================
// �e�N�X�`���̃A�����[�h
//==================================
void CItemUi::Unload(void)
{
	// �e�N�X�`���̉��
	if (m_pFrameTex != NULL)
	{
		m_pFrameTex->Release();
		m_pFrameTex = NULL;
	}
}

//=============================
// ����������
//=============================
HRESULT CItemUi::Init(void)
{
	// ���W�̐ݒ�

	// �v���C���[�l���̎擾
	int nNumPlayer = CGame::GetPlayerNum();
	// �v���C���[�l���Ɣԍ��ɂ���ĕ���
	switch (m_nPlayerNum)
	{
	case 0:
		// �v���C���[1

		if (nNumPlayer == 1)
		{// �����Ȃ�
			m_pos = (POS_PLAYER1_1);
			
		}
		else if (nNumPlayer == 2)
		{// �񕪊�
			m_pos = (POS_PLAYER1_2);
			
		}
		else
		{// �l����
			m_pos = (POS_PLAYER1_4);
			
		}
		break;
	case 1:
		// �v���C���[2

		if (nNumPlayer == 2)
		{// �񕪊�
			m_pos = (POS_PLAYER2_2);

		}
		else
		{// �l����
			m_pos = (POS_PLAYER2_4);
		}
		break;
	case 2:
		// �v���C���[3
		m_pos = (POS_PLAYER3);

		break;
	case 3:
		// �v���C���[4
		m_pos = (POS_PLAYER4);

		break;
	default:
		break;
	}

	// �|���S������
	m_pFramePolygon = CPolygon::Create(m_pos, FRAME_SIZE);
	m_pFramePolygon->BindTexture(m_pFrameTex);

	// �|���S������
	m_pItemPolygon = CPolygon::Create(m_pos, FRAME_SIZE);
	m_pItemPolygon->BindTexture(NULL);

	return S_OK;
}

//=============================
// �I������
//=============================
void CItemUi::Uninit(void)
{
	if (m_pFramePolygon != NULL)
	{
		m_pFramePolygon->Uninit();
		delete m_pFramePolygon;
		m_pFramePolygon = NULL;
	}
	
	Release();
}


//=============================
// �X�V����
//=============================
void CItemUi::Update(void)
{
	// �v���C���[���̎擾
	CPlayer*pPlayer = (CPlayer*)GetTop(OBJTYPE_PLAYER);
	while (pPlayer != NULL)
	{
		if (pPlayer->GetPlayerNum() == m_nPlayerNum)
		{
			if (pPlayer->GetItem() != CItem::ITEM_NONE)
			{
				// �e�N�X�`���̊��蓖��
				m_pItemPolygon->BindTexture(m_apTexture[pPlayer->GetItem()]);
			}
			else
			{
				// �e�N�X�`����NULL�ɂ���
				m_pItemPolygon->BindTexture(NULL);
			}
			break;
		}
	}

	if (CGame::GetState() == CGame::GAME_RESULT)
	{// ���U���g������
		Uninit();
	}
	
}


//=============================
// �`�揈��
//=============================
void CItemUi::Draw(void)
{
	if (m_pFramePolygon != NULL)
	{
		// �|���S���`��
		m_pFramePolygon->Draw();
	}

	if (m_pItemPolygon != NULL)
	{
		if (m_pItemPolygon->GetTexture() != NULL)
		{
			m_pItemPolygon->Draw();
		}
	}
	
}