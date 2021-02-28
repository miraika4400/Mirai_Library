////////////////////////////////////////////////////
//
//    destination�N���X�̏���[destination.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////

//******************************
// �C���N���[�h
//******************************
#include "destination.h"
#include "manager.h"
#include "renderer.h"
#include "keyboard.h"
#include "joypad.h"
#include "mouse.h"
#include "model.h"
#include "player.h"
#include "game.h"

//*****************************
// �}�N����`
//*****************************
#define GRAVITY D3DXVECTOR3(0.0f,-40.0f,0.0f)  // �d�͗�
#define GRAVITY_RATE   0.1f                    // �d�͂̌W��
#define DIRECTION_RATE 0.1f                    // ������ς���W��
#define DISTANCE_RATE  0.05f                   // �����̌W��
#define ADD_DRIFT      0.0075f                 // �h���t�g���̉��Z�l
#define VIEW_MOVE_SPEED_MAX 0.03f               // �}�E�X�ړ��ő�l

//*****************************
// �ÓI�����o�ϐ��錾
//*****************************


//******************************
// �R���X�g���N�^
//******************************
CDestination::CDestination()
{
	// �ϐ��̃N���A
	m_pos = VEC3_ZERO;                        // ���W
	m_rot = VEC3_ZERO;                        // ���b�g
	m_rotDest = VEC3_ZERO;                    // ���b�g�ڕW�l
	m_move = VEC3_ZERO;                       // �ړ���
	m_nPlayerNum = 0;                         // �v���C���[�ԍ�
	m_gravityVec = VEC3_ZERO;                 // �d�͗�
	m_bGravity = true;                        // �d�̓t���O
	m_fDistancePlayer = 0;                    // �v���C���[�Ƃ̋���
	m_fDistanceDest = DISTANCE_PLAYER_DEFAULT;   // �v���C���[�Ƃ̋����̖ڕW�l
	m_bGoal = false;                          // �S�[���t���O
#ifdef _DEBUG
	m_pMeshModel = NULL;	//���b�V�����ւ̃|�C���^
	m_pBuffMatModel = NULL;	//�}�e���A�����ւ̃|�C���^
	m_nNumMatModel = NULL;	//�}�e���A�����̐�
	pModel = NULL;          // ���f���N���X�̃|�C���^
#endif
}

//******************************
// �f�X�g���N�^
//******************************
CDestination::~CDestination()
{
}

//******************************
// �N���G�C�g�֐�
//******************************
CDestination * CDestination::Create(const int nPlayerNum, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	// �������̊m��
	CDestination*pDest;
	pDest = new CDestination;

	if (pDest != NULL)
	{
		pDest->m_nPlayerNum = nPlayerNum;  // �v���C���[�ԍ�
		// ������
		pDest->Init();
		// �����̑��
		pDest->m_pos = pos;     // ���W
		pDest->m_rot = rot;     // ���b�g
		pDest->m_rotDest = rot; // ���b�g�ڕW�l
		
	}

	return pDest;
}

//******************************
// ����������
//******************************
HRESULT CDestination::Init(void)
{
#ifdef _DEBUG

	if (pModel == NULL)
	{
		//���b�V���̐���
		CreateMesh();

		pModel = new CModel;
		if (pModel != NULL)
		{
			pModel->Init();
			// ���f�����蓖��
			pModel->BindModel(m_pMeshModel, m_pBuffMatModel, 1);
		}
	}
#endif
	// �v���C���[�Ƃ̋����̏�����
	m_fDistancePlayer = DISTANCE_PLAYER_INIT;     // �v���C���[�Ƃ̋���
	m_fDistanceDest = DISTANCE_PLAYER_DEFAULT;    // �v���C���[�Ƃ̋����̖ڕW�l

	// �S�[���t���O�̏�����
	m_bGoal = false;


	// �v���C���[�̍��W�̎擾
	D3DXVECTOR3 playerPos = CPlayer::GetPlayer(m_nPlayerNum)->GetPos();

	// �����̐ݒ�
	m_rot.y = CPlayer::GetPlayer(m_nPlayerNum)->GetRot().y + D3DXToRadian(180);
	m_rotDest.y = m_rot.y;
	// �������v���C���[�ɍ��킹��
	m_pos.y = playerPos.y;

	// X���EZ���̐ݒ�
	m_pos.x = playerPos.x + cosf(-m_rot.y - D3DXToRadian(90)) * -m_fDistanceDest;
	m_pos.z = playerPos.z + sinf(-m_rot.y - D3DXToRadian(90)) * -m_fDistanceDest;

#ifdef _DEBUG
	//m_pos = D3DXVECTOR3(0.0f, 400.0f, 0.0f);
	if (pModel != NULL)
	{
		pModel->SetPos(m_pos);
		pModel->SetRot(m_rot);
	}

#endif // _DEBUG

	return S_OK;
}

//******************************
// �I������
//******************************
void CDestination::Uninit(void)
{
#ifdef _DEBUG
	//���b�V���̔j��
	if (m_pMeshModel != NULL)
	{
		m_pMeshModel->Release();
		m_pMeshModel = NULL;
	}
	//�}�e���A���̔j��
	if (m_pBuffMatModel != NULL)
	{
		m_pBuffMatModel->Release();
		m_pBuffMatModel = NULL;
	}
#endif // _DEBUG

	delete this;
}

//******************************
// �X�V����
//******************************
void CDestination::Update(void)
{
	// �v���C���[���̎擾
	CPlayer*pPlayer = CPlayer::GetPlayer(m_nPlayerNum);

	// �ړ��Ǘ�
	MoveControll();
	if (pPlayer != NULL)
	{
		if (!pPlayer->GetStan())
		{
			// �����̊Ǘ�
			Direction();
			
			// �v���C���[�̍��W�̎擾
			D3DXVECTOR3 playerPos = pPlayer->GetPos();
			// �������v���C���[�ɍ��킹��
			m_pos.y = playerPos.y;

			playerPos.x = m_pos.x + cosf(-m_rot.y - D3DXToRadian(90)) * m_fDistancePlayer;
			playerPos.z = m_pos.z + sinf(-m_rot.y - D3DXToRadian(90)) * m_fDistancePlayer;
			pPlayer->SetPos(playerPos);

			D3DXVECTOR3 playerRot = pPlayer->GetRot();
			playerRot.y = atan2f(m_pos.z - playerPos.z, m_pos.x - playerPos.x) - D3DXToRadian(90);
			playerRot.y *= -1;
			pPlayer->SetRot(playerRot);
		}
	}

#ifdef _DEBUG
	//m_pos = D3DXVECTOR3(0.0f, 400.0f, 0.0f);
	if (pModel != NULL)
	{
		pModel->SetPos(m_pos);
		pModel->SetRot(m_rot);
	}

#endif // _DEBUG
}

//******************************
// �`�揈��
//******************************
void CDestination::Draw(void)
{
#ifdef _DEBUG

#endif // _DEBUG
}



//******************************
// �����̊Ǘ�
//******************************
void CDestination::Direction(void)
{

	// �v���C���[���̎擾
	CPlayer*pPlayer = CPlayer::GetPlayer(m_nPlayerNum);
	if (pPlayer != NULL)
	{
		if (!pPlayer->GetGoalFlag())
		{// �v���C���[���S�[����Ԃ���Ȃ��Ƃ�

			if (pPlayer->GetDriftLeft())
			{
				m_rotDest.y += -ADD_DRIFT;
				m_rotDest.y += -ADD_DRIFT;
				m_fDistanceDest = DISTANCE_PLAYER_DDRIFT;
			}
			else if (pPlayer->GetDriftRight())
			{
				m_rotDest.y += ADD_DRIFT;
				m_rotDest.y += ADD_DRIFT;
				m_fDistanceDest = DISTANCE_PLAYER_DDRIFT;
			}
			else
			{
				m_fDistanceDest = DISTANCE_PLAYER_DEFAULT;
			}
			// �}�E�X�ł̌�������
			m_rotDest.y += min(max(CManager::GetMouse()->GetMouseMove().x , -VIEW_MOVE_SPEED_MAX), VIEW_MOVE_SPEED_MAX);

			// �R���g���[���[�ł̌�������
			m_rotDest.y += min(max(CManager::GetJoypad()->GetStick(m_nPlayerNum).lX / JOYPAD_SENSI_RATE, -VIEW_MOVE_SPEED_MAX), VIEW_MOVE_SPEED_MAX);

			m_rot += (m_rotDest - m_rot)*DIRECTION_RATE;
		}
	}


}

#ifdef _DEBUG
//******************************
// ���b�V������
//******************************
void CDestination::CreateMesh(void)
{
	const float c_fRadius = 50.0f;
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXCreateCylinder(pDevice, 1, c_fRadius, c_fRadius*1.5f, 3, 1, &m_pMeshModel, &m_pBuffMatModel);

	// �F�̐ݒ�
	D3DXMATERIAL* mat = (D3DXMATERIAL*)m_pBuffMatModel->GetBufferPointer();
	mat->MatD3D.Ambient = D3DXCOLOR(0.3f, 1.0f, 1.0f, 0.5f);
	mat->MatD3D.Diffuse = D3DXCOLOR(0.3f, 1.0f, 1.0f, 0.5f);
	mat->MatD3D.Specular = D3DXCOLOR(0.3f, 1.0f, 1.0f, 0.5f);
	mat->MatD3D.Emissive = D3DXCOLOR(0.3f, 1.0f, 1.0f, 0.5f);
}

#endif // _DEBUG

//******************************
// �v���C���[�Ƃ̋����̃Z�b�g
//******************************
void CDestination::SetDistancePlayer(float fDistance)
{
	// �v���C���[�Ƃ̋����Z�b�g
	m_fDistancePlayer = fDistance;
	// �v���C���[�Ƃ̋����̖ڕW�l�Z�b�g
	//m_fDistanceDest = m_fDistancePlayer;

	// �v���C���[���̎擾
	CPlayer*pPlayer = CPlayer::GetPlayer(m_nPlayerNum);

	// �v���C���[�̍��W�̎擾
	D3DXVECTOR3 playerPos = pPlayer->GetPos();
	// �������v���C���[�ɍ��킹��
	m_pos.y = playerPos.y;

	m_pos.x = playerPos.x + cosf(-m_rot.y - D3DXToRadian(90)) * -m_fDistancePlayer;
	m_pos.z = playerPos.z + sinf(-m_rot.y - D3DXToRadian(90)) * -m_fDistancePlayer;
	SetPos(m_pos);
}

//******************************
// �ړ��̊Ǘ�
//******************************
void CDestination::MoveControll(void)
{
	//�ړ��ʂ̖ڕW�l
	D3DXVECTOR3 moveDest = VEC3_ZERO;

#if 1
	if (CManager::GetKeyboard()->GetKeyPress(DIK_W) || CManager::GetJoypad()->GetJoystickPress(3, m_nPlayerNum) || CPlayer::GetPlayer(m_nPlayerNum)->GetActiveAcceleration())
	{
		if (CPlayer::GetPlayer(m_nPlayerNum)->GetMoveFlag())
		{// �ړ��t���O��true�̎�
			if (!CPlayer::GetPlayer(m_nPlayerNum)->GetGoalFlag())
			{// �S�[�����ĂȂ��Ƃ�
			 // �ړ��ʂ̖ڕW�l�̐ݒ�
				moveDest.x = cosf(-m_rot.y - D3DXToRadian(90))* CPlayer::GetPlayer(m_nPlayerNum)->GetSpeed();
				moveDest.z = sinf(-m_rot.y - D3DXToRadian(90))* CPlayer::GetPlayer(m_nPlayerNum)->GetSpeed();

				// �v���C���[�̋�����ڕW�l�ɋ߂Â���
				m_fDistancePlayer += (m_fDistanceDest - m_fDistancePlayer)*DISTANCE_RATE;
				SetDistancePlayer(m_fDistancePlayer);
			}
		}
		else
		{// �v���C���[�Ƃ̋����̃��Z�b�g
			SetDistancePlayer(DISTANCE_PLAYER_INIT);
		}
	}
	else
	{// �v���C���[�Ƃ̋����̃��Z�b�g
		SetDistancePlayer(DISTANCE_PLAYER_INIT);
	}
#else
	if (CManager::GetKeyboard()->GetKeyPress(DIK_W))
	{
		moveDest.x = cosf(-m_rot.y - D3DXToRadian(90))* PLAYER_SPEED;
		moveDest.z = sinf(-m_rot.y - D3DXToRadian(90))* PLAYER_SPEED;
	}

#endif

	// ����
	m_move += (moveDest - m_move) * CHARACTER_MOVE_RATE;

	// �ړ��ʂ𑫂�
	m_pos += m_move;
}