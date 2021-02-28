////////////////////////////////////////////////////
//
//    �Q�[���N���X�̏���[game.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////

//=============================
// �C���N���[�h
//=============================
#include "game.h"
#include "bg.h"
#include "number.h"
#include "score.h" 
#include "mouse.h"
#include "camera.h"
#include "light.h"
#include "fade.h"
#include "keyboard.h"
#include "particle.h"
#include "time.h"
#include "player.h"
#include "course.h"
#include "checkpoint.h"
#include "character.h"
#include "cpu_point.h"
#include "cpu.h"
#include "rank.h"
#include "item_point.h"
#include "item.h"
#include "start.h"
#include "joypad.h"

//=============================
// �}�N����`
//=============================
#define RESULT_COUNT 250 // ���U���g�ւ̑J�ڃJ�E���g
#define INIT_POS_X 400   // �L�����N�^�[�z�u����X�ʒu
#define INIT_POS_Z 0     // �L�����N�^�[�z�u����Y�ʒu
#define SPACE_X 280      // �L�����N�^�[�z�uX�̊Ԋu
#define SPACE_Z 140      // �L�����N�^�[�z�uY�̊Ԋu
#define X_RESET_NUM 4    // �L�����N�^�[�z�uX�����Z�b�g����Ԋu

//=============================
// �ÓI�����o�ϐ��錾
//=============================
CCamera *CGame::m_apCamera[MAX_PLAYER_NUM] = {};           // �J�����N���X�|�C���^
CLight  *CGame::m_pLight = NULL;                           // ���C�g�N���X�|�C���^
CCharacter * CGame::m_apCharacter[MAX_CHARACTER_NUM] = {}; // �L�����N�^�[�N���X�|�C���^
int CGame::m_nNumCaracter = 0;                             // ���������L�����N�^�[�̐�
int CGame::m_nNumPlayer = 1;                               // �v���C���[��
CCpuPoint *CGame::m_pCpuPoint = NULL;                      // CPU�p�R�[�X�f�[�^
CCheckPoint *CGame::m_pCheckPoint = NULL;                  // �`�F�b�N�|�C���g
CItemPoint *CGame::m_pItemPoint = NULL;                    // �A�C�e���|�C���g
CCourse    *CGame::m_pCourse = NULL;                       // �R�[�X�|�C���^
CGame::GAME_STATE CGame::m_gameState = CGame::GAME_NORMAL; // �Q�[���̏��

//=============================
// �R���X�g���N�^
//=============================
CGame::CGame()
{
	// �ϐ��̃N���A
	m_nNumCaracter = 0;
	m_nCntResult= 0;
	m_bResult = false;
}

//=============================
// �f�X�g���N�^
//=============================
CGame::~CGame()
{
}

//=============================
// �N���G�C�g
//=============================
CGame * CGame::Create(void)
{
	// �������̊m��
	CGame *pGame = new CGame;
	// ������
	pGame->Init();
	return pGame;
}

//=============================
// ����������
//=============================
HRESULT CGame::Init(void)
{
	// �J�[�\��������
	ShowCursor(FALSE);

	// �|�[�Y�̏�����
	CManager::SetActivePause(false);

	// ���C�g�N���X�̐���
	m_pLight = new CLight;
	// ���C�g�N���X�̏�����
	if (m_pLight != NULL)
	{
		if (FAILED(m_pLight->Init()))
		{
			return -1;
		}
	}
	// ���U���g�J�E���g�̏�����
	m_nCntResult = 0;
	// ���U���g�t���O�̏�����
	m_bResult = false;
	// �Q�[���̏�Ԃ̏�����
	m_gameState = GAME_NORMAL;

	// �w�i�̐���
	CBg::Create();

	// ���ʊǗ��N���X�̐���
	CRank::Create();

	// �R�[�X����
	m_pCourse = CCourse::Create();

	// �`�F�b�N�|�C���g�̐���
	m_pCheckPoint = CCheckPoint::Create();

	// CPU�p�̃R�[�X�f�[�^
	m_pCpuPoint = CCpuPoint::Create();
	
	// �A�C�e�����W
	m_pItemPoint = CItemPoint::Create();


	// �L�����N�^�[�̐����E�z�u
	float fPosX = INIT_POS_X;
	float fPosZ = INIT_POS_Z;
	for (int nCnt = 0; nCnt < MAX_NPC_NUM; nCnt++)
	{
		if (m_nNumCaracter % X_RESET_NUM == 0)
		{
			fPosX = INIT_POS_X;
		}

		// CPU����
		m_apCharacter[m_nNumCaracter] = CCpu::Create(D3DXVECTOR3(fPosX, 0.0f, fPosZ), D3DXVECTOR3(0.0f, D3DXToRadian(180.0f), 0.0f), (CCpu::CPUTYPE)(rand() % CCpu::CPU_TYPE_MAX), rand() % 10 + 1);
		// �L�����N�^�[�����Z
		m_nNumCaracter++;
		fPosX -= SPACE_X;
		fPosZ += SPACE_Z;

	}
	for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
	{
		// �J�����N���X�̐���
		m_apCamera[nCnt] = CCamera::Create(nCnt);
		// �v���C���[����
		m_apCharacter[m_nNumCaracter] = CPlayer::Create(D3DXVECTOR3(fPosX, 0.0f, fPosZ), D3DXVECTOR3(0.0f, D3DXToRadian(180.0f), 0.0f), 0);
		// �L�����N�^�[�����Z
		m_nNumCaracter++;
		fPosX -= SPACE_X;
		fPosZ += SPACE_Z;
	}
	

	// �X�^�[�g�̃J�E���g�_�E���̐���
	CStart::Create();
	return S_OK;
}

//=============================
// �I������
//=============================
void CGame::Uninit(void)
{
	// �J�����N���X�̉������
	for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
	{
		if (m_apCamera[nCnt] != NULL)
		{
			m_apCamera[nCnt]->Uninit();
		}
	}

	// ���C�g
	if (m_pLight != NULL)
	{
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = NULL;
	}

	// �J������
	Release();
}


//=============================
// �X�V����
//=============================
void CGame::Update(void)
{
	// �}�E�X�J�[�\���̈ʒu�Œ�
	CManager::GetMouse()->SetMousePos(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
#ifdef _DEBUG
	// �f�o�b�O�p��ʑJ�ڃR�}���h
	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_F1))
	{
		CManager::GetFade()->SetFade(CManager::MODE_RESULT);
	}

#endif // _DEBUG

	if (m_gameState == GAME_NORMAL)
	{
		// �A�C�e������]������
		CItem::RotationItem();

		// �S�[����Ԃ̃`�F�b�N
		CheckGoal();
	}
	else
	{

		if (CManager::GetKeyboard()->GetKeyTrigger(DIK_RETURN) ||
			CManager::GetMouse()->GetMouseTrigger(0) ||
			CManager::GetJoypad()->GetJoystickTrigger(3, 0) ||
			CManager::GetJoypad()->GetJoystickTrigger(11, 0))
		{
			CManager::GetFade()->SetFade(CManager::MODE_TITLE);
		}
	}

	// �J�����N���X�̍X�V����
	for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
	{
		if (m_apCamera[nCnt] != NULL)
		{
			m_apCamera[nCnt]->Update();
		}
	}
}

//=============================
// �`�揈��
//=============================
void CGame::Draw(void)
{
	// �J�����̃Z�b�g
	for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
	{
		if (m_apCamera[nCnt] != NULL)
		{
			m_apCamera[nCnt]->SetCamera();
		}
	}
}

//=============================
// �v���C���[���S�[�����Ă��邩�`�F�b�N
//=============================
void CGame::CheckGoal(void)
{
	if (!m_bResult)
	{
		for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
		{
			// ��l�ł��S�[�����ĂȂ������珈���I��
			if (!CPlayer::GetPlayer(nCnt)->GetGoalFlag()) return;
		}

		// �����܂ŏ����������炷�ׂẴv���C���[���S�[�����Ă���
		m_bResult = true;
	}
	else
	{
		// �J�E���g��i�߂�
		m_nCntResult++;

		if (m_nCntResult > RESULT_COUNT)
		{// ���̃J�E���g��������
			// ���U���g�Ɉڍs
			m_gameState = GAME_RESULT;
		}
	}
}
