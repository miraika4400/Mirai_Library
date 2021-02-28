////////////////////////////////////////////////////
//
//    FeverLogo�̏���[FeverLogo.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////


//=============================
// �C���N���[�h
//=============================
#include "rank_ui.h"
#include "manager.h"
#include "renderer.h"
#include "polygon.h"
#include "keyboard.h"
#include "mouse.h"
#include "joypad.h"
#include "fade.h"
#include "sound.h"
#include "game.h"
#include "player.h"
#include "checkpoint.h"
#include "collision.h"

//**********************************
// �}�N����`
//**********************************
#define TEXTURE_PATH "./data/Textures/rank.png"    // �e�N�X�`���̃p�X
#define TEXTURE_PATH_RESULT "./data/Textures/rank_result.png"    // �e�N�X�`���̃p�X
#define MAX_ANIMATION_X 10                                    // �A�j���[�V������ ��
#define MAX_ANIMATION_Y 1                                     // �A�j���[�V������ �c
#define SIZE D3DXVECTOR3( 70.0f, 70.0f, 0.0f)   // �T�C�Y*�摜�̉𑜓x�ɍ��킹��

// �ʒu
#define POS_PLAYER1_1 D3DXVECTOR3( SCREEN_WIDTH    - 90 ,  SCREEN_HEIGHT    - 100 , 0.0f)  // ��ʕ������ĂȂ��Ƃ�
#define POS_PLAYER1_2 D3DXVECTOR3( SCREEN_WIDTH    - 90 , (SCREEN_HEIGHT/2) - 100 , 0.0f)  // ��ʂ��ɕ����Ă���Ƃ��̃v���C���[1
#define POS_PLAYER1_4 D3DXVECTOR3((SCREEN_WIDTH/2) - 90 , (SCREEN_HEIGHT/2) - 80 , 0.0f)  // ��ʂ��l�ɕ����Ă���Ƃ��̃v���C���[1
#define POS_PLAYER2_2 D3DXVECTOR3( SCREEN_WIDTH    - 90 ,  SCREEN_HEIGHT    - 100 , 0.0f)  // ��ʂ��ɕ����Ă���Ƃ��̃v���C���[2
#define POS_PLAYER2_4 D3DXVECTOR3( SCREEN_WIDTH    - 90 , (SCREEN_HEIGHT/2) - 100 , 0.0f)  // ��ʂ��l�ɕ����Ă���Ƃ��̃v���C���[2
#define POS_PLAYER3   D3DXVECTOR3((SCREEN_WIDTH/2) - 90 , (SCREEN_HEIGHT)   - 80 , 0.0f)  // �v���C���[3
#define POS_PLAYER4   D3DXVECTOR3( SCREEN_WIDTH    - 90 , (SCREEN_HEIGHT)   - 80 , 0.0f)  // �v���C���[4

// ���U���g���̃T�C�Y
#define RESULT_SIZE D3DXVECTOR3(200.0f, 200.0f, 0.0f)   // �摜�̉𑜓x�ɍ��킹��
// ���U���g���̈ʒu
#define RESULT_POS_PLAYER1_1 D3DXVECTOR3( SCREEN_WIDTH/2                   , SCREEN_HEIGHT/2                   - 60.0f, 0.0f)  // ��ʕ������ĂȂ��Ƃ�
#define RESULT_POS_PLAYER1_2 D3DXVECTOR3( SCREEN_WIDTH/2                   , SCREEN_HEIGHT/4                   - 60.0f, 0.0f)  // ��ʂ��ɕ����Ă���Ƃ��̃v���C���[1
#define RESULT_POS_PLAYER1_4 D3DXVECTOR3( SCREEN_WIDTH/4                   , SCREEN_HEIGHT/4                   - 60.0f, 0.0f)  // ��ʂ��l�ɕ����Ă���Ƃ��̃v���C���[1
#define RESULT_POS_PLAYER2_2 D3DXVECTOR3( SCREEN_WIDTH/2                   , SCREEN_HEIGHT/4 + SCREEN_HEIGHT/2 - 60.0f, 0.0f)  // ��ʂ��ɕ����Ă���Ƃ��̃v���C���[2
#define RESULT_POS_PLAYER2_4 D3DXVECTOR3( SCREEN_WIDTH/4 + SCREEN_WIDTH/2  , SCREEN_HEIGHT/4                   - 60.0f, 0.0f)  // ��ʂ��l�ɕ����Ă���Ƃ��̃v���C���[2
#define RESULT_POS_PLAYER3   D3DXVECTOR3( SCREEN_WIDTH/4                   , SCREEN_HEIGHT/4 + SCREEN_HEIGHT/2 - 60.0f, 0.0f)  // �v���C���[3
#define RESULT_POS_PLAYER4   D3DXVECTOR3( SCREEN_WIDTH/4 + SCREEN_WIDTH/2  , SCREEN_HEIGHT/4 + SCREEN_HEIGHT/2 - 60.0f, 0.0f)  // �v���C���[4

//**********************************
// �ÓI�����o�ϐ��錾
//**********************************
LPDIRECT3DTEXTURE9 CRankUi::m_apTexture[RANK_UI_TEX_MAX] = {};

//=============================
// �R���X�g���N�^
//=============================
CRankUi::CRankUi() :CScene2d(OBJTYPE_UI)
{
	// �ϐ��̃N���A
	m_nCntAnim = 0;           // �A�j���[�V�����J�E���g
	m_nAnimX = 0;             // �A�j���[�V����X��
	m_nAnimY = 0;             // �A�j���[�V����Y��
	m_nPlayerNum = 0;         // �v���C���[�ԍ�
	m_resultPos = VEC3_ZERO;  // ���U���g���̃|�X
	m_resultSize = VEC3_ZERO; // ���U���g���̃T�C�Y
}

//=============================
// �f�X�g���N�^
//=============================
CRankUi::~CRankUi()
{
}

//=============================
// �N���G�C�g
//=============================
CRankUi * CRankUi::Create(int nPlayerNum)
{
	// �������̊m��
	CRankUi *pRank = new CRankUi;
	// �v���C���[�ԍ��̑��
	pRank->m_nPlayerNum = nPlayerNum;
	// ������
	pRank->Init();
	return pRank;
}

//==================================
// �e�N�X�`���̃��[�h
//==================================
HRESULT CRankUi::Load(void)
{

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_PATH, &m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_PATH_RESULT, &m_apTexture[1]);
	return S_OK;
}

//==================================
// �e�N�X�`���̃A�����[�h
//==================================
void CRankUi::Unload(void)
{
	for (int nCnt = 0; nCnt < RANK_UI_TEX_MAX; nCnt++)
	{
		// �e�N�X�`���̉��
		if (m_apTexture[nCnt] != NULL)
		{
			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = NULL;
		}
	}
	
}

//=============================
// ����������
//=============================
HRESULT CRankUi::Init(void)
{
	CScene2d::Init();

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	m_nAnimX = 0;     // �A�j���[�V����X��
	m_nAnimY = 0;     // �A�j���[�V����Y��

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

	// �T�C�Y�̐ݒ�
	SetSize(SIZE);

	m_resultSize = RESULT_SIZE;      // ���U���g���̃T�C�Y
									 // �e�N�X�`���̐ݒ�
	BindTexture(m_apTexture[RANK_UI_TEX_NORMAL]);

	// ���W�̐ݒ�

	// �v���C���[�l���̎擾
	int nNumPlayer = CGame::GetPlayerNum();
	// �v���C���[�ԍ��ɂ���ĕ���
	switch (m_nPlayerNum)
	{
	case 0:
		// �v���C���[1

		if (nNumPlayer == 1)
		{// �����Ȃ�
			SetPos(POS_PLAYER1_1);
			// ���U���g���̍��W
			m_resultPos = RESULT_POS_PLAYER1_1;
		}
		else if (nNumPlayer == 2)
		{// �񕪊�
			SetPos(POS_PLAYER1_2);
			// ���U���g���̍��W
			m_resultPos = RESULT_POS_PLAYER1_2;
		}
		else
		{// �l����
			SetPos(POS_PLAYER1_4);
			// ���U���g���̍��W
			m_resultPos = RESULT_POS_PLAYER1_4;
		}
		break;
	case 1:
		// �v���C���[2

		if (nNumPlayer == 2)
		{// �񕪊�
			SetPos(POS_PLAYER2_2);
			// ���U���g���̍��W
			m_resultPos = RESULT_POS_PLAYER2_2;
		}
		else
		{// �l����
			SetPos(POS_PLAYER2_4);
			// ���U���g���̍��W
			m_resultPos = RESULT_POS_PLAYER2_4;
		}
		break;
	case 2:
		// �v���C���[3
		SetPos(POS_PLAYER3);
		// ���U���g���̍��W
		m_resultPos = RESULT_POS_PLAYER3;
		break;
	case 3:
		// �v���C���[4
		SetPos(POS_PLAYER4);
		// ���U���g���̍��W
		m_resultPos = RESULT_POS_PLAYER4;
		break;
	default:
		break;
	}


	return S_OK;
}

//=============================
// �I������
//=============================
void CRankUi::Uninit(void)
{
	CScene2d::Uninit();
}


//=============================
// �X�V����
//=============================
void CRankUi::Update(void)
{
	if (CGame::GetState() == CGame::GAME_NORMAL)
	{
		CPlayer*pPlayer = (CPlayer*)GetTop(OBJTYPE_PLAYER);

		while (pPlayer != NULL)
		{
			if (pPlayer != NULL &&pPlayer->GetPlayerNum() == m_nPlayerNum)
			{
				m_nAnimX = pPlayer->GetRankData().nRank - 1;     // �A�j���[�V����X��

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
			}
			pPlayer = (CPlayer*)pPlayer->GetNext();
		}
	}
	else if(CGame::GetState() == CGame::GAME_RESULT)
	{// ���U���g��Ԃ̎�
		
		if (GetTexture() != m_apTexture[RANK_UI_TEX_RESULT])
		{// �e�N�X�`���̐؂�ւ�
			BindTexture(m_apTexture[RANK_UI_TEX_RESULT]);
		}

		SetPos(m_resultPos);
		SetSize(m_resultSize);
	}
}


//=============================
// �`�揈��
//=============================
void CRankUi::Draw(void)
{
	CScene2d::Draw();
}