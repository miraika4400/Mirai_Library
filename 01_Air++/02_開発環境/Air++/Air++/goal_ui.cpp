////////////////////////////////////////////////////
//
//    FeverLogo�̏���[FeverLogo.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////


//=============================
// �C���N���[�h
//=============================
#include "goal_ui.h"
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
#define TEXTURE_PATH "./data/Textures/Goal.png"    // �e�N�X�`���̃p�X
#define SIZE D3DXVECTOR3( 960.0f / 2.5f, 276.0f/2.5f, 0.0f)   // �T�C�Y*�摜�̉𑜓x�ɍ��킹��
#define COL D3DXCOLOR(1.0f, 1.0f, 0.6f, 1.0f)                    // �J���[

#define MAX_ANIMATION_X 19                                    // �A�j���[�V������ ��
#define MAX_ANIMATION_Y 1                                     // �A�j���[�V������ �c
#define ANIMATION_COUNT 3                                     // �A�j���[�V�����̑��x*�Ⴂ��������

// �ʒu
#define POS_PLAYER1_1 D3DXVECTOR3( SCREEN_WIDTH/2                   , SCREEN_HEIGHT/2                   , 0.0f)  // ��ʕ������ĂȂ��Ƃ�
#define POS_PLAYER1_2 D3DXVECTOR3( SCREEN_WIDTH/2                   , SCREEN_HEIGHT/4                   , 0.0f)  // ��ʂ��ɕ����Ă���Ƃ��̃v���C���[1
#define POS_PLAYER1_4 D3DXVECTOR3( SCREEN_WIDTH/4                   , SCREEN_HEIGHT/4                   , 0.0f)  // ��ʂ��l�ɕ����Ă���Ƃ��̃v���C���[1
#define POS_PLAYER2_2 D3DXVECTOR3( SCREEN_WIDTH/2                   , SCREEN_HEIGHT/4 + SCREEN_HEIGHT/2 , 0.0f)  // ��ʂ��ɕ����Ă���Ƃ��̃v���C���[2
#define POS_PLAYER2_4 D3DXVECTOR3( SCREEN_WIDTH/4 + SCREEN_WIDTH/2  , SCREEN_HEIGHT/4                   , 0.0f)  // ��ʂ��l�ɕ����Ă���Ƃ��̃v���C���[2
#define POS_PLAYER3   D3DXVECTOR3( SCREEN_WIDTH/4                   , SCREEN_HEIGHT/4 + SCREEN_HEIGHT/2 , 0.0f)  // �v���C���[3
#define POS_PLAYER4   D3DXVECTOR3( SCREEN_WIDTH/4 + SCREEN_WIDTH/2  , SCREEN_HEIGHT/4 + SCREEN_HEIGHT/2 , 0.0f)  // �v���C���[4

//**********************************
// �ÓI�����o�ϐ��錾
//**********************************
LPDIRECT3DTEXTURE9 CGoalUi::m_pTexture = NULL;

//=============================
// �R���X�g���N�^
//=============================
CGoalUi::CGoalUi() :CScene2d(OBJTYPE_UI)
{
	// �ϐ��̃N���A
	m_nPlayerNum = 0; // �v���C���[�ԍ�
	m_nCntAnim = 0;           // �A�j���[�V�����J�E���g
	m_nAnimX = 0;             // �A�j���[�V����X��
	m_nAnimY = 0;             // �A�j���[�V����Y��
}

//=============================
// �f�X�g���N�^
//=============================
CGoalUi::~CGoalUi()
{
}

//=============================
// �N���G�C�g
//=============================
CGoalUi * CGoalUi::Create(int nPlayerNum)
{
	// �������̊m��
	CGoalUi *pRank = new CGoalUi;
	// �v���C���[�ԍ��̑��
	pRank->m_nPlayerNum = nPlayerNum;
	// ������
	pRank->Init();
	return pRank;
}

//==================================
// �e�N�X�`���̃��[�h
//==================================
HRESULT CGoalUi::Load(void)
{

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_PATH, &m_pTexture);

	return S_OK;
}

//==================================
// �e�N�X�`���̃A�����[�h
//==================================
void CGoalUi::Unload(void)
{
	// �e�N�X�`���̉��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================
// ����������
//=============================
HRESULT CGoalUi::Init(void)
{
	CScene2d::Init();
	// �e�N�X�`���̐ݒ�
	BindTexture(m_pTexture);

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
		}
		else if (nNumPlayer == 2)
		{// �񕪊�
			SetPos(POS_PLAYER1_2);
		}
		else
		{// �l����
			SetPos(POS_PLAYER1_4);
		}
		break;
	case 1:
		// �v���C���[2

		if (nNumPlayer == 2)
		{// �񕪊�
			SetPos(POS_PLAYER2_2);
		}
		else
		{// �l����
			SetPos(POS_PLAYER2_4);
		}
		break;
	case 2:
		// �v���C���[3
		SetPos(POS_PLAYER3);
		break;
	case 3:
		// �v���C���[4
		SetPos(POS_PLAYER4);
		break;
	default:
		break;
	}

	if (nNumPlayer <= 2)
	{
		// �T�C�Y�̐ݒ�
		SetSize(SIZE);
	}
	else
	{
		// �T�C�Y�̐ݒ�
		SetSize(SIZE / 2);
	}

	// �e�N�X�`���A�j���[�V�����̏�����
	m_nAnimX = 0;     // �A�j���[�V����X��
	m_nAnimY = 0;     // �A�j���[�V����Y��
	m_nCntAnim = 0;   // �A�j���[�V�����J�E���g

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

	// �F�̐ݒ�
	SetColor(COL);

	return S_OK;
}

//=============================
// �I������
//=============================
void CGoalUi::Uninit(void)
{
	CScene2d::Uninit();
}


//=============================
// �X�V����
//=============================
void CGoalUi::Update(void)
{
	// �e�N�X�`���A�j���[�V�����̏���
	if (m_nAnimX < MAX_ANIMATION_X-1)
	{
		// �J�E���g��i�߂�
		m_nCntAnim++;
		if (m_nCntAnim % ANIMATION_COUNT == 0)
		{// ���̃J�E���g��X���̃A�j���[�V������i�߂�
			m_nAnimX++;
		}

		// UV���W�̍X�V
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


	if (CGame::GetState() == CGame::GAME_RESULT)
	{// ���U���g��Ԃ̎�
	 // UI������
		Uninit();
	}
}


//=============================
// �`�揈��
//=============================
void CGoalUi::Draw(void)
{
	CScene2d::Draw();
}