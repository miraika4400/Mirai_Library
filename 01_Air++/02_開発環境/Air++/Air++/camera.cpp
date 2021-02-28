////////////////////////////////////////////////////
//
//    �J�����N���X�̏���[camera.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////

//******************************
// �C���N���[�h
//******************************
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "keyboard.h"
#include "game.h"
#include "mouse.h"
#include "player.h"
#include "cpu.h"
#include "destination.h"
#include "joypad.h"

//******************************
// �}�N����`
//******************************
#define CAMERA_DISTANCE 850.0f                               // �J�����ƑΏۂ̋���
#define CAMERA_VIEW_MAX 100000.0f                            // �J�����̕`�拗��(��)
#define CAMERA_VIEW_MIN 10.0f                                // �J�����̕`�拗��(��)
#define CAMERA_FOV_BASE D3DXToRadian(45.0f)                  // ����p
#define CAMERA_FOV_ACCELERATION D3DXToRadian(60.0f)          // ����p*������
#define CAMERA_FOV_STAN D3DXToRadian(35.0f)                  // ����p*�X�^����
#define CAMERA_FOV_RATE 0.05f                                // ����p�ω����̌W��
#define CAMERA_BACK_BUFFE D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f) // �J�����̔w�i�F
#define CAMERA_MOVE_SPEED_MAX 0.04f                          // �J�������슴�x�ő�l
#define DAMERA_THETA_RATE 0.05f                              // �J�����̃V�[�^�������̌W��
#define CAMERA_THETA_BASE D3DXToRadian(75.0f)                // �J�����̃t�@�C(����)�̊�{�p�x
#define DAMERA_PHI_RATE 0.01f                                // �J�����̃t�@�C(����)�������̌W��
#define THETA_DIFFERENCE D3DXToRadian(60)                    // �V�[�^�ƃV�[�^�̖ڕW�l�̍��̍ő�
#define SHAKE_VEC D3DXVECTOR3(60.0f,70.0f,30.0f)             // �u���̕���
#define SHAKE_COUNT 6                                        // �u���̕����]�����C���^�[�o��
#define SHAKE_RATE 0.05f                                     // �u���̌W��

// CPU�ɒǏ]�ւ���ɐ؂�ւ���}�N��
#define CPU_CAMERA
#undef  CPU_CAMERA

//******************************
// �ÓI�����o�ϐ��錾
//******************************
CCamera*CCamera::m_pCamera = NULL;

//******************************
// �R���X�g���N�^
//******************************
CCamera::CCamera()
{
	// �ϐ��̃N���A
	m_posV = { 0.0f,0.0f,0.0f };
	m_posR = { 0.0f,0.0f,0.0f };
	m_vecU = { 0.0f,0.0f,0.0f };
	D3DXMatrixIdentity(&m_mtxProjection);
	D3DXMatrixIdentity(&m_mtxView);
	m_nCntShake = 0;
	m_shake = VEC3_ZERO;
	m_shakeDist = VEC3_ZERO;
	m_nPlayerNum = 0;
	m_fRad   = 0.0f;              // �ڕW����̋���
	m_fTheta = 0.0f;              // �p�x�V�[�^
	m_fPhi   = 0.0f;              // �p�x�t�@�C
	m_fFov = 0.0f;                // ����p
}

//******************************
// �f�X�g���N�^
//******************************
CCamera::~CCamera()
{
}

//******************************
// �N���G�C�g
//******************************
CCamera * CCamera::Create(int nPlayerNum)
{

	m_pCamera = new CCamera;
	m_pCamera->Init();

	// �v���C���[�ԍ��̃Z�b�g
	m_pCamera->m_nPlayerNum = nPlayerNum;

	return m_pCamera;
}


//******************************
// ����������
//******************************
HRESULT CCamera::Init(void)
{
	// �ϐ��̏�����

	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // �����_
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);      // ������x�N�g��

	m_fRad = CAMERA_DISTANCE;     // ����
	m_fTheta = D3DXToRadian(90);  // �p�x�V�[�^
	m_fPhi = CAMERA_THETA_BASE;   // �p�x�t�@�C

	// ���ʍ��W�̐ݒ�
	m_posV.x = m_posR.x + (m_fRad)* sinf(m_fPhi)*cosf(m_fTheta);
	m_posV.y = m_posR.y + (m_fRad)* cosf(m_fPhi);
	m_posV.z = m_posR.z + (m_fRad)* sinf(m_fPhi)*sinf(m_fTheta);

	m_fFov = CAMERA_FOV_BASE; //����p

	return S_OK;
}

//******************************
// �I������
//******************************
void CCamera::Uninit(void)
{
	delete this;
}

//******************************
// �X�V����
//******************************
void CCamera::Update(void)
{
#ifdef CPU_CAMERA
	// CPU�ɒǏ]����J�����i�f�o�b�O�p�j*������ƎG

	static CCpu* pCpu = (CCpu*)CScene::GetTop(CScene::OBJTYPE_CPU);

	// ���Ă���CPU�̐؂�ւ�
	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_RIGHT))
	{
		if (pCpu->GetNext() != NULL)
		{
			pCpu = (CCpu*)pCpu->GetNext();
		}
	}
	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_LEFT))
	{
		if (pCpu->GetPrev() != NULL)
		{
			pCpu = (CCpu*)pCpu->GetPrev();
		}
	}

	// �����_��CPU�ɂ���
	if (pCpu != NULL)
	{
		m_posR = pCpu->GetPos();
	}

	// �p�x�V�[�^�̖ڕW�l
	float fThetaDist = (-pCpu->GetRot().y - D3DXToRadian(90));

	// �O����Ɖ�]���Ȃ��悤����
	while (m_fTheta - fThetaDist > D3DXToRadian(180))
	{
		fThetaDist += D3DXToRadian(360);
	}
	while (m_fTheta - fThetaDist < D3DXToRadian(-180))
	{
		fThetaDist -= D3DXToRadian(360);
	}

	// �V�[�^��ڕW�l�ɋ߂Â���
	m_fTheta += (fThetaDist - m_fTheta)*0.05f;
	
	//������CPU�̃��b�gX�ɑΉ�������
	m_fPhi += ((-pCpu->GetRot().x) + CAMERA_THETA_BASE - m_fPhi)*DAMERA_PHI_RATE;

	// ���ʍ��W�̐ݒ�
	m_posV.x = m_posR.x + (m_fRad)* sinf(m_fPhi)*cosf(m_fTheta);
	m_posV.y = m_posR.y + (m_fRad)* cosf(m_fPhi);
	m_posV.z = m_posR.z + (m_fRad)* sinf(m_fPhi)*sinf(m_fTheta);

#else
	// �v���C���[�ɒǏ]����J����
	CPlayer* pPlayer = (CPlayer*)CScene::GetTop(CScene::OBJTYPE_PLAYER);

	while (pPlayer != NULL)
	{
		if (pPlayer != NULL &&pPlayer->GetPlayerNum() == m_nPlayerNum)
		{
			// �����_���v���C���[�ɂ���
			if (pPlayer != NULL)
			{
				// �u���̒l�̏���
				Shake();

				m_posR = pPlayer->GetDest() ->GetPos() + m_shake;

				float fThetaDist = -pPlayer->GetDest()->GetRot().y+D3DXToRadian(90);

				// �O����Ɖ�]���Ȃ��悤�ɒ���
				while (m_fTheta - fThetaDist > D3DXToRadian(180))  fThetaDist += D3DXToRadian(360);
				while (m_fTheta - fThetaDist < D3DXToRadian(-180)) fThetaDist -= D3DXToRadian(360);

				// �V�[�^�̖ڕW�l�̍��̏���A����
				if (fThetaDist - m_fTheta >= THETA_DIFFERENCE)
				{
					m_fTheta = fThetaDist + -THETA_DIFFERENCE;
				}
				if (fThetaDist - m_fTheta <= -THETA_DIFFERENCE)
				{
					m_fTheta = fThetaDist + THETA_DIFFERENCE;
				}
				m_fTheta += (fThetaDist - m_fTheta)*DAMERA_THETA_RATE;
			}

			//// �}�E�X�Ŏ��_����
			//m_fTheta -= min(max(CManager::GetMouse()->GetMouseMove().x / 100, -CAMERA_MOVE_SPEED_MAX), CAMERA_MOVE_SPEED_MAX);

			
			//�������v���C���[�̃��b�gX�ɑΉ�������
			m_fPhi += ((-pPlayer->GetRot().x) + CAMERA_THETA_BASE - m_fPhi)*DAMERA_PHI_RATE;

			if (pPlayer->GetActiveAcceleration())
			{// ������
				m_fFov += (CAMERA_FOV_ACCELERATION - m_fFov)*CAMERA_FOV_RATE;
			}
			else if (pPlayer->GetStan())
			{// �X�^����
				m_fFov += (CAMERA_FOV_STAN - m_fFov)*CAMERA_FOV_RATE;
			}
			else
			{// �ʏ�
				m_fFov += (CAMERA_FOV_BASE - m_fFov)*CAMERA_FOV_RATE;
			}

		
				
			if (CManager::GetMouse()->GetMousePress(1)|| CManager::GetJoypad()->GetJoystickPress(1, 0))
			{// �o�b�N�~���[

				// ���ʍ��W�̐ݒ�
				m_posV.x = pPlayer->GetPos().x + (m_fRad)* sinf((pPlayer->GetRot().x) + CAMERA_THETA_BASE)*cosf(m_fTheta + D3DXToRadian(180));
				m_posV.y = pPlayer->GetPos().y + (m_fRad)* cosf((pPlayer->GetRot().x) + CAMERA_THETA_BASE);
				m_posV.z = pPlayer->GetPos().z + (m_fRad)* sinf((pPlayer->GetRot().x) + CAMERA_THETA_BASE)*sinf(m_fTheta + D3DXToRadian(180));
			}
			else
			{
				// ���ʍ��W�̐ݒ�
				m_posV.x = pPlayer->GetPos().x + (m_fRad)* sinf(m_fPhi)*cosf(m_fTheta);
				m_posV.y = pPlayer->GetPos().y + (m_fRad)* cosf(m_fPhi);
				m_posV.z = pPlayer->GetPos().z + (m_fRad)* sinf(m_fPhi)*sinf(m_fTheta);
			}

			break;
		}
		pPlayer = (CPlayer*)pPlayer->GetNext();
	}

#endif // CPU_CAMERA
}

//******************************
// �J�����̃Z�b�g
//******************************
void CCamera::SetCamera(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIXA16 matWorld;

	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_pCamera->m_mtxView);
	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&m_pCamera->m_mtxView, &m_pCamera->m_posV, &m_pCamera->m_posR, &m_pCamera->m_vecU);
	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_pCamera->m_mtxView);

	//�v���W�F�N�g�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_pCamera->m_mtxProjection);

	D3DXMatrixPerspectiveFovLH(&m_pCamera->m_mtxProjection,
		m_fFov, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, CAMERA_VIEW_MIN, CAMERA_VIEW_MAX);
	
	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_pCamera->m_mtxProjection);

	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, CAMERA_BACK_BUFFE, 1.0f, 0);
}

//******************************
// �u���̏���
//******************************
void CCamera::Shake(bool bRand)
{
	if (bRand)
	{// ������true�������Ƃ�

		m_shakeDist = SHAKE_VEC;
	}
	else
	{
		CPlayer* pPlayer = (CPlayer*)CScene::GetTop(CScene::OBJTYPE_PLAYER);

		while (pPlayer != NULL)
		{
			if (pPlayer != NULL &&pPlayer->GetPlayerNum() == m_nPlayerNum)
			{
				// �v���C���[����Q���ɂԂ������Ƃ��ɃJ�������Ԃ炷
				if (pPlayer->GetStan())
				{
					// �J�E���g��i�߂�
					m_nCntShake++;
					// ���̃J�E���g��
					if (m_nCntShake % SHAKE_COUNT == 0)
					{// ���Ε����ɂԂ炵������Ƃ��Ԃ��������������
						m_shakeDist *= -0.9f;
					}
				}
				else
				{
					m_nCntShake = 0;
					m_shakeDist = VEC3_ZERO;
				}
				break;
			}

			pPlayer = (CPlayer*)pPlayer->GetNext();
		}

		// �u���̒l��ڕW�l�ɋ߂Â���
		m_shake += (m_shakeDist - m_shake)*SHAKE_RATE;
	}
}