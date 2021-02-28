////////////////////////////////////////////////////
//
//    Start�N���X�̏���[Start.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////

//******************************
// �C���N���[�h
//******************************
#include "Start.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "sound.h"

#include <time.h>

//*****************************
// �}�N����`
//*****************************
//#define POS   D3DXVECTOR3(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,0.0f) // ���W
#define POS   D3DXVECTOR3(0.0f,200.0f,-500.0f) // ���W
#define SIZE  D3DXVECTOR3(100.0f,100.0f,0.0f) 

#define MAX_ANIMATION_X 4      // �A�j���[�V������ ��
#define MAX_ANIMATION_Y 1      // �A�j���[�V������ �c

#define TEXTURE_PATH "./data/Textures/start.png"    //�e�N�X�`���̃p�X

//******************************
// �ÓI�����o�ϐ��錾
//******************************
LPDIRECT3DTEXTURE9  CStart::m_pTexture = NULL; // �e�N�X�`���|�C���^

//******************************
// �R���X�g���N�^
//******************************
CStart::CStart() :CScene3d(OBJTYPE_UI)
{
	m_nCntAnim = 0;
	m_nAnimX = 0;
	m_nAnimY = 0;
	m_currentTime = 0;
	m_nTime = 2000;
}

//******************************
// �f�X�g���N�^
//******************************
CStart::~CStart()
{
}

//******************************
// �N���G�C�g
//******************************
CStart * CStart::Create(void)
{
	// �������̊m��
	CStart *pStart;
	pStart = new CStart;

	// ������
	pStart->Init();

	return pStart;
}

//******************************
// �e�N�X�`���̃��[�h
//******************************
HRESULT CStart::Load(void)
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
void CStart::Unload(void)
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
HRESULT CStart::Init(void)
{
	if (FAILED(CScene3d::Init()))
	{
		return E_FAIL;
	}

	// �R�[�X�ɂ�����Ĉʒu�̕���
	SetPos(POS);

	// �T�C�Y�̐ݒ�
	SetSize(SIZE);

	// �e�N�X�`�����蓖��
	BindTexture(m_pTexture);

	// �J�n���Ԃ�����
	m_currentTime = timeGetTime();

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

	m_nTime = 2000;
	return S_OK;
}

//******************************
// �I������
//******************************
void CStart::Uninit(void)
{
	CScene3d::Uninit();
}

//******************************
// �X�V����
//******************************
void CStart::Update(void)
{
	// �A�j���[�V�������ő喢���̎�
	if (m_nAnimX + 1 < MAX_ANIMATION_X)
	{
		// �����Ԃ̎擾
		DWORD nowTime = timeGetTime();

		// �O��L���������ԂƂ̍����Z�o
		int nTime = nowTime - m_currentTime;

		// ����1�b�imsec�j�𒴂��Ă���
		if (nTime >= m_nTime)
		{
			// �O��L�������^�C���̍X�V
			m_currentTime = nowTime;

			// �A�j���[�V������i�߂�
			m_nAnimX++;

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

			if (m_nAnimX + 1 != MAX_ANIMATION_X)
			{
				CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_COUNT1);
			}
		}

		if (m_nTime == 2000 && nTime >= 1000)
		{// �ŏ��̃J�E���g����2�{�̃^�C��

		 // �O��L�������^�C���̍X�V
			m_currentTime = nowTime;
			// �؂�ւ��^�C��
			m_nTime = 1000;
			if (m_nAnimX + 1 != MAX_ANIMATION_X)
			{
				CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_COUNT1);
			}
		}

		if (m_nAnimX + 1 == MAX_ANIMATION_X)
		{
			// SE�Đ�
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_COUNT2);
			// �Q�[��BGM�Đ�
			CManager::GetSound()->Play(CSound::LABEL_BGM_GAME);

			// �S���ړ��\��Ԃɂ���
			for (int nCntChara = 0; nCntChara < CGame::GetCharacterNum(); nCntChara++)
			{
				CGame::GetCharacter(nCntChara)->SetMoveFlag(true);
			}
		}
	}

	if (CGame::GetState() == CGame::GAME_RESULT)
	{// ���U���g��Ԃ̎�
	 // UI������
		Uninit();
	}

}

//******************************
// �`�揈��
//******************************
void CStart::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// ���C�g������
	pDevice->SetRenderState(D3DRS_LIGHTING, false);

	CScene3d::Draw();

	// ���C�g������
	pDevice->SetRenderState(D3DRS_LIGHTING, true);
}
