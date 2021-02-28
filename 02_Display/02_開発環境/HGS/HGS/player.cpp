//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "player.h"
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
#define PLAYER_LIFE				(1)			// �v���C���[�̗̑�
#define PLAYER_SPEED			(10.0f)	 	// �v���C���[�̑��x
#define PLAYER_INERTIA			(0.08f)		// �����̑傫��
#define BULLET_INTERVAL			(12)		// �e�̘A�ˊԊu

//=============================================================================
// static������
//=============================================================================
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture = NULL;
bool CPlayer::m_bAlive = true;
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPlayer::CPlayer() :CScene2d(OBJTYPE_PLAYER)
{
	m_nBulletCnt = 0;
	m_nLife = 0;
	m_nStock = 0;
	m_State = PLAYER_STATE_NONE;
	m_StateCount = 0;
	m_Inertia = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bAlive = true;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// �e�N�X�`�����[�h
//=============================================================================
HRESULT CPlayer::Load(void)
{
	// �����_���[�̏����󂯎��
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/Textures/Player_Animatio.png",
		&m_pTexture);		// �e�N�X�`���ւ̃|�C���^

	return S_OK;
}

//=============================================================================
// �C���X�^���X����
//=============================================================================
CPlayer * CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �������m��
	CPlayer *pPlayer = new CPlayer;

	if (pPlayer != NULL)
	{	
		// ����������
		pPlayer->Init();
		pPlayer->SetPos(pos);		// �v���C���[�̈ʒu�ݒ�
		pPlayer->SetSize(size);		// �T�C�Y�̐ݒ�
		pPlayer->BindTexture(m_pTexture);	// �e�N�X�`���ݒ�
		pPlayer->InitAnimation(5, 7, -1);
	}
	return pPlayer;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPlayer::Init(void)
{
	// �l�̑��
	m_nBulletCnt = 0;			// �e�̊Ԋu������
	m_nLife = PLAYER_LIFE;		// �̗�

	// �e�N�X�`���󂯓n��
	BindTexture(m_pTexture);

	// �|���S��������
	CScene2d::Init();

	m_bAlive = true;
	return S_OK;
}

//=============================================================================
// �e�N�X�`���A�����[�h
//=============================================================================
void CPlayer::UnLoad(void)
{
	// �e�N�X�`���̊J��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// �I������
//=============================================================================
void CPlayer::Uninit(void)
{
	// �I������
	CScene2d::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CPlayer::Update(void)
{
	// �X�V����
	CScene2d::Update();

	// �v���C���[�̏��
//	UpdateState();

	// �v���C���[����֐�
	PlayerControl();

	// �e�̔���
	Shot();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CPlayer::Draw(void)
{
	// �`�揈��
	CScene2d::Draw();
}

//=============================================================================
// �v���C���[����֐�
//=============================================================================
void CPlayer::PlayerControl(void)
{
	// ���W�󂯎��
	D3DXVECTOR3 pos = GetPos();

	// �W���C�p�b�h�̎擾
	DIJOYSTATE js = CInputJoypad::GetStick(0);

	if (js.lX != 0.0f || js.lY != 0)
	{
		float fAngle = atan2f((float)js.lX, (float)js.lY);

		// �W���C�p�b�h����
		m_Inertia.x = sinf(fAngle)* PLAYER_SPEED;
		m_Inertia.y = cosf(fAngle)* PLAYER_SPEED;
	}
	else
	{
		m_Inertia = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	m_move += (m_Inertia - m_move)*PLAYER_INERTIA;

	pos += m_move;
	// �L�[�{�[�h�X�V
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	// W�L�[���������Ƃ�
	if (pKeyboard->GetKeyPress(DIK_W))
	{
		pos.y += cosf(D3DX_PI)*PLAYER_SPEED;
	}
	// S�L�[���������Ƃ�
	if (pKeyboard->GetKeyPress(DIK_S))
	{
		pos.y += cosf(0)*PLAYER_SPEED;
	}
	// A�L�[���������Ƃ�
	if (pKeyboard->GetKeyPress(DIK_A))
	{
		pos.x -= sinf(D3DX_PI / 2)*PLAYER_SPEED;
	}
	// D�L�[���������Ƃ�
	if (pKeyboard->GetKeyPress(DIK_D))
	{
		pos.x += sinf(D3DX_PI / 2)*PLAYER_SPEED;
	}


	// ������
	if (pos.x - PLAYER_SIZE_X / 2 < 0.0f)
	{
		pos.x = PLAYER_SIZE_X / 2;
	}
	// �E����
	if (pos.x + PLAYER_SIZE_X / 2 > SCREEN_WIDTH)
	{
		pos.x = SCREEN_WIDTH - PLAYER_SIZE_X / 2;
	}
	// �㔻��
	if (pos.y - PLAYER_SIZE_Y / 2 < 0.0f)
	{
		pos.y = PLAYER_SIZE_Y / 2;
	}
	// ������
	if (pos.y + PLAYER_SIZE_Y / 2 > SCREEN_HEIGHT)
	{
		pos.y = SCREEN_HEIGHT - PLAYER_SIZE_Y / 2;
	}

	// ���W��n��
	SetPos(pos);

}

//=============================================================================
// �e�̔���
//=============================================================================
void CPlayer::Shot(void)
{
	// ���W�󂯎��
	D3DXVECTOR3 pos = GetPos();

	// �W���C�p�b�h�̎擾
	DIJOYSTATE js = CInputJoypad::GetStick(0);

	// �L�[�{�[�h�X�V
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	// �o���b�g�̊Ԋu
	m_nBulletCnt++;

	// �o���b�g�̊Ԋu
	if (m_nBulletCnt >= BULLET_INTERVAL)
	{
		// W�L�[���������Ƃ�
		if (pKeyboard->GetKeyTrigger(DIK_UP) || CManager::GetJoypad()->GetJoystickTrigger(1, 0))
		{
			// �e�̐���
			CBullet::Create(D3DXVECTOR3(pos.x, pos.y, 0.0f),
				D3DXVECTOR3(0.0f, -BULLET_SPEED, 0.0f),
				D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f),
				100);

			m_nBulletCnt = 0;
		}
		// S�L�[���������Ƃ�
		else if (pKeyboard->GetKeyTrigger(DIK_DOWN) || CManager::GetJoypad()->GetJoystickTrigger(2, 0))
		{
			// �e�̐���
			CBullet::Create(D3DXVECTOR3(pos.x, pos.y, 0.0f),
				D3DXVECTOR3(0.0f, BULLET_SPEED, 0.0f),
				D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f),
				100);

			m_nBulletCnt = 0;
		}
		// A�L�[���������Ƃ�
		else if (pKeyboard->GetKeyTrigger(DIK_LEFT) || CManager::GetJoypad()->GetJoystickTrigger(0, 0))
		{
			// �e�̐���
			CBullet::Create(D3DXVECTOR3(pos.x, pos.y, 0.0f),
				D3DXVECTOR3(-BULLET_SPEED, 0.0f, 0.0f),
				D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f),
				100);

			m_nBulletCnt = 0;
		}
		// D�L�[���������Ƃ�
		else if (pKeyboard->GetKeyTrigger(DIK_RIGHT) || CManager::GetJoypad()->GetJoystickTrigger(3, 0))
		{
			// �e�̐���
			CBullet::Create(D3DXVECTOR3(pos.x, pos.y, 0.0f),
				D3DXVECTOR3(BULLET_SPEED, 0.0f, 0.0f),
				D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f),
				100);

			m_nBulletCnt = 0;
		}
		// SE�Đ�
		CManager::GetSound()->Play(CSound::LABEL_SE_SHOT);
	}

}

//=============================================================================
// �_���[�W���󂯂��Ƃ�
//=============================================================================
void CPlayer::HitDamage(int nCount)
{
	m_nLife -= nCount;

	// �̗͂������Ȃ�����
	if (m_nLife <= 0)
	{
		CGrid::BreakAll(GetPos());
		// ������Ԃ�false��
		m_bAlive = false;

		// SE�Đ�
		CManager::GetSound()->Play(CSound::LABEL_SE_GAME_OVER);
		// �I������
		Uninit();
		
		// �t�F�[�h
		//CManager::GetFade()->SetFade(CManager::MODE_RESULT);
		return;
	}
}