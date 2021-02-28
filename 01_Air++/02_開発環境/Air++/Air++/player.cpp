//=============================================================================
//
// player�w�b�_ [player.cpp]
// Author : ���V ����
//
//=============================================================================

//******************************
// �C���N���[�h
//******************************
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "camera.h"
#include "keyboard.h"
#include "rank_ui.h"
#include "course.h"
#include "item_ui.h"
#include "mouse.h"
#include "collision.h"
#include "destination.h"
#include "joypad.h"
#include "lap_ui.h"
#include "goal_ui.h"
#include "sound.h"

//*****************************
// �}�N����`
//*****************************
#define MODEL_PATH     "./data/Models/player.x"     // ���f���̃p�X
#define SHADER_PATH    "./data/HLSL/Shader.fx"      // HLSL�t�@�C���̃p�X
#define CUBE_TEX_PATH  "./data/Textures/cube_sky.dds" // �L���[�u�e�N�X�`���̃p�X

#define VIEW_MOVE_SPEED_MAX 0.03f                   // �}�E�X�ړ��ő�l
#define DRIFT_STICK_TIP 10                          // �X�e�B�b�N���ǂꂭ�炢�X���Ă�����h���t�g�ɂȂ邩
#define DRIFT_ROT_SPEED 0.015f                      // �h���t�g���Ƀv���X����l
#define LOCUS_DISTANCE 80.0f                        // �O�Ղ��o������
#define LOCUS_ADJUST 10.0f                          // �O�Ղ��o�������̒���
#define LOCUS_COL D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f) // ��ՃJ���[

//*****************************
// �ÓI�����o�ϐ��錾
//*****************************
CModel::Model   CPlayer::m_model = {};  //���b�V�����ւ̃|�C���^
CScene::Shader CPlayer::m_shader = {};  // �V�F�[�_�[�\����

//******************************
// �R���X�g���N�^
//******************************
CPlayer::CPlayer() :CCharacter(OBJTYPE_PLAYER)
{
	// �ϐ��̃N���A
	m_nPlayerNum = -1;  // �v���C���[ID
	m_fDir = 0.0f;
	m_bDriftLeft  = false;      // �h���t�g��
	m_bDriftRight = false;      // �h���t�g�E
}

//******************************
// �f�X�g���N�^
//******************************
CPlayer::~CPlayer()
{
}

//******************************
// �N���G�C�g
//******************************
CPlayer * CPlayer::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, int nPlayerNum)
{
	// �������̊m��
	CPlayer *pPlayer;
	pPlayer = new CPlayer;

	// �v���C���[�ԍ��̐ݒ�
	pPlayer->m_nPlayerNum = nPlayerNum; 

	// ������
	pPlayer->Init();

	// �e�l�̑���E�Z�b�g
	pPlayer->SetPos(pos); // ���W�̃Z�b�g
	pPlayer->SetRot(rot); // ��]�̃Z�b�g
	pPlayer->GetCollision()->SetPos(pos); // �����蔻��̈ʒu�̐ݒ�

	// �ړ��ڕW�N���X�̐���
	pPlayer->m_pDest = CDestination::Create(pPlayer->m_nPlayerNum, pos);

	return pPlayer;
}

//******************************
// �e�N�X�`���̃��[�h
//******************************
HRESULT CPlayer::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(MODEL_PATH, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_model.pBuffMat, NULL, &m_model.nNumMat, &m_model.pMesh);

	// �e�N�X�`���ǂݍ���
	if (m_model.nNumMat != 0)
	{
		D3DXMATERIAL*pMat = (D3DXMATERIAL*)m_model.pBuffMat->GetBufferPointer();
		for (int nCnt = 0; nCnt < (int)m_model.nNumMat; nCnt++)
		{
			if (pMat[nCnt].pTextureFilename != NULL)
			{
				char cPath[64] = {};
				sprintf(cPath, "./data/Textures/%s", pMat[nCnt].pTextureFilename);
				// �e�N�X�`���̐���
				D3DXCreateTextureFromFile(pDevice, cPath, &m_model.apTexture[nCnt]);
			}
		}
	}

	// HLSL�t�@�C���̓ǂݍ���
	D3DXCreateEffectFromFile(pDevice, SHADER_PATH, NULL, NULL, 0, NULL, &m_shader.pEffect, NULL);

	sprintf(&m_shader.achTechniqueName[0], "%s", "TShader");

	D3DXCreateCubeTextureFromFile(pDevice, CUBE_TEX_PATH, &m_shader.pCubeTex);
	return S_OK;
}

//******************************
// �e�N�X�`���̃A�����[�h
//******************************
void CPlayer::Unload(void)
{
	//���b�V���̔j��
	if (m_model.pMesh != NULL)
	{
		m_model.pMesh->Release();
		m_model.pMesh = NULL;
	}

	//�}�e���A���̔j��
	if (m_model.pBuffMat != NULL)
	{
		m_model.pBuffMat->Release();
		m_model.pBuffMat = NULL;
	}
}

//******************************
// �v���C���[�̎擾����
//******************************
CPlayer * CPlayer::GetPlayer(int nPlayerNum)
{
	// return�p
	CPlayer*pOut = NULL;
	// �v���C���[���X�g�̎擾
	pOut = (CPlayer*)GetTop(OBJTYPE_PLAYER);

	while(pOut != NULL)
	{
		if (pOut->GetPlayerNum()==nPlayerNum)
		{// �����ƃv���C���[�ԍ�����v���Ă���Ƃ��Ƀ|�C���^��Ԃ�
			return pOut;
		}

		// ���X�g�̃l�N�X�g�ɂ���
		pOut = (CPlayer*)pOut->GetNext();
	}
	return pOut;
}

//******************************
// ����������
//******************************
HRESULT CPlayer::Init(void)
{
	if (FAILED(CCharacter::Init()))
	{
		return E_FAIL;
	}

	// ���f�����蓖��
	BindModel(m_model.pMesh, m_model.pBuffMat, m_model.nNumMat);

	//�e�N�X�`���̊��蓖��
	for (int nCnt = 0; nCnt < (int)m_model.nNumMat; nCnt++)
	{
		BindTexture(nCnt, m_model.apTexture[nCnt]);
	}

	// �V�F�[�_�[�̃Z�b�g
	SetShader(m_shader);

	// ����UI�̐���
	CRankUi::Create(m_nPlayerNum);
	// �A�C�e��UI�̐���
	CItemUi::Create(m_nPlayerNum);
	// ���b�v��UI�̐���
	CLapUi::Create(m_nPlayerNum);

	// �v���C���[�̐��ʂւ̊p�x
	m_fDir = GetRot().y + D3DXToRadian(90);

	// �h���t�g������
	m_bDriftLeft = false;   // �h���t�g��
	m_bDriftRight = false;  // �h���t�g�E

	// ��Ղ̏��
	LocusData locusData = { LOCUS_DISTANCE, LOCUS_ADJUST, LOCUS_COL };
	SetLocusData(locusData);

	return S_OK;
}

//******************************
// �I������
//******************************
void CPlayer::Uninit(void)
{
	// �ړ��ڕW�N���X�̏I������
	if (m_pDest != NULL)
	{
		m_pDest->Uninit();
		m_pDest = NULL;
	}

	CCharacter::Uninit();
}
	

//******************************
// �X�V����
//******************************
void CPlayer::Update(void)
{
	//m_pDest->Update();

	// �ړ��s�\���ӊO�ɏ�������������
	if (!GetGoalFlag()&& GetMoveFlag() && !GetStan())
	{
		// �ړ��ʂ̐ݒ�
		//Move();

		// �h���t�g����
		Drift();

		// �A�C�e���g�p
		UseItem();

		// �v���C���[�̐��ʂւ̊p�x�̕ۑ�
		m_fDir = (-GetRot().y) + D3DXToRadian(90);
	}
	else
	{
		// �ړ��ʂ̃Z�b�g
		SetMoveDist(VEC3_ZERO);
	}

	// �L�����N�^�[�N���X�̍X�V����*�ړ������E�����̏����E�d�͏����E�����蔻��
	CCharacter::Update();
}

//******************************
// �`�揈��
//******************************
void CPlayer::Draw(void)
{
	CCharacter::Draw();
}

void CPlayer::GoalAction(void)
{
	CCharacter::GoalAction();
	
	// �S�[��UI�̐���
	CGoalUi::Create(m_nPlayerNum);
}

//******************************
// �ړ��ʂ̐ݒ菈��
//******************************
void CPlayer::Move(void)
{
	// �ړ������x�N�g��
	D3DXVECTOR3 move = { 0.0f,0.0f,0.0f };
#if 0
	// �ړ������x�N�g��
	D3DXVECTOR3 move = { 0.0f,0.0f,0.0f };

	if (CManager::GetKeyboard()->GetKeyPress(DIK_W)||GetActiveAcceleration())
	{
		//// ���W�̎擾
		//D3DXVECTOR3 pos = GetPos();
		//// �J�����̈ʒu�̎擾
		//D3DXVECTOR3 cameraPos = CGame::GetCamera(m_nPlayerNum)->GetPos();
		//
		//// �J�������玩���̕����x�N�g��
		//move = pos - cameraPos;
		//// Y�����ɂ͈ړ����Ȃ�
		//move.y = 0.0f;
		//// ���K��
		//D3DXVec3Normalize(&move, &move);

		// �ړ�����
		move.x = cosf(m_fDir);
		move.y = 0.0f;
		move.z = sinf(m_fDir);
		// �X�s�[�h��������
		move *= GetSpeed();
	}
	
	// �ړ��ʂ̃Z�b�g
	SetMoveDist(move);
#else
	// �ړ��ʂ̃Z�b�g
	SetMoveDist(move);
	SetMove(move);
#endif
}

//******************************
// �A�C�e���g�p�̏���
//******************************
void CPlayer::UseItem(void)
{
	if (GetItem() != CItem::ITEM_NONE && CManager::GetKeyboard()->GetKeyTrigger(DIK_SPACE) || 
		GetItem() != CItem::ITEM_NONE && CManager::GetJoypad()->GetJoystickPress(4, m_nPlayerNum) ||
		GetItem() != CItem::ITEM_NONE && CManager::GetMouse()->GetMousePress(0))
	{
		// �A�C�e���Z�b�g
		SetItemObject();
	}
}

//******************************
// �����̊Ǘ�*character�N���X����I�[�o�[���C�h
//******************************
void CPlayer::Direction(void)
{
	D3DXVECTOR3 rot = GetRot();
	if (GetMoveFlag() && !GetStan())
	{// �ړ��t���O��true���X�^����ԂȂ��Ⴂ�Ƃ�
		// �}�E�X�Ŏ��_����
		rot.y += min(max(CManager::GetMouse()->GetMouseMove().x, -VIEW_MOVE_SPEED_MAX), VIEW_MOVE_SPEED_MAX);
	}
	// X��

	if (CGame::GetCourse() != NULL)
	{
		// ���C���o���ʒu
		D3DXVECTOR3 rayPos = VEC3_ZERO;
		rayPos.x = GetPos().x + (cosf(m_fDir)*RAY_DISTANCE);
		rayPos.y = GetPos().y + RAY_HEIGHT;
		rayPos.z = GetPos().z + (sinf(m_fDir)*RAY_DISTANCE);

		// �R�[�X���b�V���̎擾
		LPD3DXMESH pCourseMesh = CGame::GetCourse()->GetMesh();
		// 
		BOOL bHit = false;
		float fHitDistance;

		// ���C�ɂ�铖���蔻��
		D3DXIntersect(pCourseMesh,
			&rayPos,
			&D3DXVECTOR3(0.0f, -1.0f, 0.0f),
			&bHit,
			NULL,
			NULL,
			NULL,
			&fHitDistance,
			NULL,
			NULL);

		if (bHit)
		{// �������Ă�����

			if (fHitDistance <= CHARACTER_ROT_X_ADJUST_RANGE)
			{
				// �������Ă�����W
				D3DXVECTOR3 hitPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				hitPos.x = rayPos.x;
				hitPos.y = rayPos.y - fHitDistance;
				hitPos.z = rayPos.z;

				// X���̖ڕW�l
				float fRotDistX = atan2f((-(hitPos.y - (GetPos().y - HOVER_HEIGHT))), RAY_DISTANCE);

				// ���X�ɖڕW�l�ɋ߂Â���
				rot.x += (fRotDistX - rot.x) * CHARACTER_DIRECTION_RATE;
				
			}
		}
	}

	// �����̃Z�b�g
	SetRot(rot);
}

//******************************
// �h���t�g����
//******************************
void CPlayer::Drift(void)
{
	if (!m_bDriftLeft && !m_bDriftRight)
	{// �h���t�g�����E�ǂ����false��Ԃ̎�

		// �L�[�{�[�h
		if (CManager::GetKeyboard()->GetKeyPress(DIK_LSHIFT))
		{
			if (CManager::GetMouse()->GetMouseMove().x > 0)
			{// �E�h���t�g
				m_bDriftRight = true;

			}
			else if (CManager::GetMouse()->GetMouseMove().x < 0)
			{// ���h���t�g
				m_bDriftLeft = true;
			}
		}
		// �R���g���[���[
		if (CManager::GetJoypad()->GetJoystickPress(5, m_nPlayerNum))
		{
			if (CManager::GetJoypad()->GetStick(m_nPlayerNum).lX >= DRIFT_STICK_TIP)
			{// �E�h���t�g
				m_bDriftRight = true;
			}
			else if (CManager::GetJoypad()->GetStick(m_nPlayerNum).lX <= -DRIFT_STICK_TIP)
			{// ���h���t�g
				m_bDriftLeft = true;
			}
		}
	}
	else
	{// �ǂ������̃h���t�g�̃t���O�������Ă�����

		// ���݂̊p�x�̎擾
		D3DXVECTOR3 rot = GetRot();

		// �p�x�̉��Z
		if (m_bDriftLeft)
		{// ���Ƀh���t�g
			rot.y -= DRIFT_ROT_SPEED;
		}
		else if (m_bDriftRight)
		{// �E�Ƀh���t�g
			rot.y += DRIFT_ROT_SPEED;
		}

		// �p�x�̃Z�b�g
		SetRot(rot);

		// �h���t�g��Ԃ̉���
		if (!CManager::GetJoypad()->GetJoystickPress(5, m_nPlayerNum) && !CManager::GetKeyboard()->GetKeyPress(DIK_LSHIFT))
		{
			m_bDriftLeft = false;
			m_bDriftRight = false;
		}
	}
}

//******************************
// �������̃A�N�V����
//******************************
void CPlayer::FallAction(void)
{

	m_pDest->Init();
	m_pDest->SetMove(VEC3_ZERO);

	m_bDriftLeft = false;
	m_bDriftRight = false;

	// SE�Đ�
	CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_FALL);
}
