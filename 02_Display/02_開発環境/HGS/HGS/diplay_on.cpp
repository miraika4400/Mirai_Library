//=============================================================================
//
// displayon [displayon.cpp]
// Author : ���V ����
//
//=============================================================================

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "display_on.h"
#include "keyboard.h"
#include "manager.h"
#include "renderer.h"
#include "fade.h"
#include "game.h"
#include "joypad.h"
#include "sound.h"
#include "bullet.h"
#include "grid.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define ANIM_SPEED 1              // �A�j���[�V�������x
#define MAX_ANIMATION_X 18        // �A�j���[�V������ ��
#define MAX_ANIMATION_Y 1         // �A�j���[�V������ �c

//=============================================================================
// static������
//=============================================================================
LPDIRECT3DTEXTURE9 CDisplayON::m_pTexture = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CDisplayON::CDisplayON() :CScene2d(OBJTYPE_UI)
{
	m_nCntAnim = 0; // �A�j���[�V�����J�E���g
	m_nAnimX = 0;   // �A�j���[�V����X��
	m_nAnimY = 0;   // �A�j���[�V����Y��
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CDisplayON::~CDisplayON()
{

}

//=============================================================================
// �e�N�X�`�����[�h
//=============================================================================
HRESULT CDisplayON::Load(void)
{
	// �����_���[�̏����󂯎��
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "./data/Textures/Black Out.png",
		&m_pTexture);		// �e�N�X�`���ւ̃|�C���^

	return S_OK;
}

//=============================================================================
// �e�N�X�`���A�����[�h
//=============================================================================
void CDisplayON::Unload(void)
{
	// �e�N�X�`���̊J��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// �C���X�^���X����
//=============================================================================
CDisplayON * CDisplayON::Create(void)
{
	// �������m��
	CDisplayON *pEnd = new CDisplayON;

	if (pEnd != NULL)
	{
		// ����������
		pEnd->Init();
	}
	return pEnd;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CDisplayON::Init(void)
{
	// �l�̑��
	BindTexture(m_pTexture);

	// �|���S��������
	CScene2d::Init();
	// �e�N�X�`��UV���W�̏�����
	m_nAnimX = MAX_ANIMATION_X-1;
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

	SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
	SetSize(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
	return S_OK;
}


//=============================================================================
// �I������
//=============================================================================
void CDisplayON::Uninit(void)
{
	// �I������
	CScene2d::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CDisplayON::Update(void)
{
	// �A�j���[�V�����J�E���g��i�߂�
	m_nCntAnim++;

	if (m_nCntAnim % ANIM_SPEED == 0)
	{
		// �A�j���[�V����X���̉��Z
		m_nAnimX--;

		if (m_nAnimX <= 0)
		{
			// �A�j���[�V����X���̏�����
			m_nAnimX = 0;
			CManager::GetFade()->SetFade(CManager::MODE_TITLE);
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
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CDisplayON::Draw(void)
{
	// �`�揈��
	CScene2d::Draw();
}