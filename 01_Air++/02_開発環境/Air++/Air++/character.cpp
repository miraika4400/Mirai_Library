//=============================================================================
//
// character�w�b�_ [character.cpp]
// Author : ���V ����
//
//=============================================================================

//******************************
// �C���N���[�h
//******************************
#include "character.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "camera.h"
#include "light.h"
#include "collision.h"
#include "course.h"
#include "checkpoint.h"
#include "cpu_point.h"
#include "player.h"
#include "destination.h"
#include "attack.h"
#include "trap.h"
#include "locus.h"
#include "stan_effect.h"
#include "sound.h"

//*****************************
// �}�N����`
//*****************************
#define CHARACTER_GRAVITY D3DXVECTOR3(0.0f,-50.0f,0.0f) // �d�͗�
#define CHARACTER_GRAVITY_RATE 0.03f                    // �d�͂̌W��
//#define CHARACTER_MOVE_RATE 1.0f                      // �ړ��ʂ̌W��
#define CHARACTER_SPEED_BASE 30.0f                      // �ړ��X�s�[�h
#define CHARACTER_SPEED_ACCELERATION 45.0f              // �ړ��X�s�[�h*������
#define CHARACTER_ACCELERATION_RATE 0.06f               // �������̃X�s�[�h�ω��̌W��
#define CHARACTER_ACCELERATION_COUNT 80                 // �����t���[����
#define CHARACTER_STAN_FRAME 60                         // �X�^���̒���(�t���[��)
#define LOCUS_DISTANCE_DEFAULT 80.0f                        // �O�Ղ��o�������f�t�H���g
#define LOCUS_ADJUST_DEFAULT 10.0f                          // �O�Ղ��o�������̒����f�t�H���g
#define LOCUS_COL_DEFAULT D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f) // ��ՃJ���[�f�t�H���g

//*****************************
// �ÓI�����o�ϐ��錾
//*****************************

//******************************
// �R���X�g���N�^
//******************************
CCharacter::CCharacter(int nPliority) :CModelShader(nPliority)
{
	// �ϐ��̃N���A
	m_move       = VEC3_ZERO;  // �ړ���
	m_bMove      = false;      // �ړ��t���O
	m_moveDist   = VEC3_ZERO;  // �ړ��ʂ̖ڕW�l
	m_gravityVec = VEC3_ZERO;  // �d�͗�
	m_bGravity   = false;       // �d�̓t���O
	m_pCollision = NULL;       // �R���W����
	m_rankData   = {};         // ���ʕt���p�f�[�^
	m_bGoal = false;           // �S�[���t���O
	m_impact = VEC3_ZERO;      // �Փ˂����Ƃ��̈ړ���
	m_item = CItem::ITEM_NONE; // �������Ă���A�C�e��
	m_bAcceleration = false;   // �����t���O
	m_fSpeed = CHARACTER_SPEED_BASE; // �ړ����x
	m_nCntAccleration = 0;     // �������̃J�E���g
	m_bStan = false;           // �X�^���t���O
	m_nCntStan = 0;            // �X�^���J�E���g
	m_nCntGravity = 0;         // �d�̓t���O���o���Ă���Ԃ̃J�E���g
	m_locusData = { LOCUS_DISTANCE_DEFAULT,LOCUS_ADJUST_DEFAULT ,LOCUS_COL_DEFAULT }; // ��Տ��

	// �I�u�W�F�N�g�^�C�v���v���C���[�̎��A�v���C���[�t���O��true�ɂ���
	if (nPliority == OBJTYPE_PLAYER) m_bIsPlayer = true;
	else m_bIsPlayer = false;
}

//******************************
// �f�X�g���N�^
//******************************
CCharacter::~CCharacter()
{
}

//******************************
// ����������
//******************************
HRESULT CCharacter::Init(void)
{
	// ���f���N���X�̏���������
	if (FAILED(CModel::Init()))
	{
		return E_FAIL;
	}
	
	// �R���W�����̐���
	m_pCollision = CCollision::CreateSphere(GetPos(), CHARACTER_RADIUS);

	// �ϐ��̎擾
	m_bMove = false;                 // �ړ��t���O
	m_bGoal = false;                 // �S�[���t���O
	m_item = CItem::ITEM_NONE;       // �������Ă���A�C�e��
	m_bAcceleration = false;         // �����t���O
	m_fSpeed = CHARACTER_SPEED_BASE; // �ړ����x
	m_nCntAccleration = 0;           // �������̃J�E���g
	m_bStan = false;                 // �X�^���t���O
	m_nCntStan = 0;                  // �X�^���J�E���g
	m_bGravity = false;              // �d�̓t���O
	m_nCntGravity = 0;               // �d�̓t���O���o���Ă���Ԃ̃J�E���g

	// ���ʕt���f�[�^�̏�����
	m_rankData.nCheck = 0;
	m_rankData.nLap = 0;
	m_rankData.nRank = 1;

	return S_OK;
}

//******************************
// �I������
//******************************
void CCharacter::Uninit(void)
{
	if (m_pCollision != NULL)
	{
		m_pCollision->Uninit();
		m_pCollision = NULL;
	}
	CModelShader::Uninit();
}

//******************************
// �X�V����
//******************************
void CCharacter::Update(void)
{
	// �O�՗p�f�[�^�̕ێ�
	D3DXVECTOR3 posOld = GetPos();
	float fAngeleOld = -GetRot().y;

	if (m_bIsPlayer)
	{// �v���C���[�̎�
		((CPlayer*)this)->GetDest()->Update();
	}

	// �d�͏���
	Gravity();
	
	if (m_bMove)
	{// �ړ��t���O��true�̎�

		// �����̏���
		Direction();
		// �X�s�[�h�̊Ǘ�
		SpeedManager();

		// �ړ���*���X�ɖڕW�l�ɋ߂Â���
		m_move += (m_moveDist - m_move)*CHARACTER_MOVE_RATE;

		// ���W�Ɉړ��ʂ𑫂�
		SetPos(GetPos() + m_move);
	}
	
	// ���f���N���X�̍X�V����
	CModelShader::Update();

	if (m_pCollision != NULL)
	{
		// �����蔻��̈ʒu�X�V
		m_pCollision->SetPos(GetPos());
	}

	// �R�[�X�̂Ɠ����蔻��
	CGame::GetCourse()->CollisionCharacter(this);

	// �L�������m�̓����蔻��̏���
	CollisionCharacter();

	// ������Ԏ��Ɋ�Ղ��o��

	if (GetActiveAcceleration())
	{// ������Ԃ̎�

		// �O�Ղ��o��
		// �v���C���[�̌����Ă�������̎擾
		float fAngele = -GetRot().y;

		// ���_���W�̌v�Z
		D3DXVECTOR3 aPos[NUM_VERTEX / 2] =
		{
			{ GetPos().x + cosf(fAngele) *  m_locusData.fWidth, GetPos().y + m_locusData.fHeight, GetPos().z + sinf(fAngele) *  m_locusData.fWidth },
			{ GetPos().x + cosf(fAngele) * -m_locusData.fWidth, GetPos().y + m_locusData.fHeight, GetPos().z + sinf(fAngele) * -m_locusData.fWidth },
		};

		D3DXVECTOR3 aPosOld[NUM_VERTEX / 2] =
		{
			{ posOld.x + cosf(fAngeleOld) *  m_locusData.fWidth , posOld.y + m_locusData.fHeight, posOld.z + sinf(fAngeleOld) *  m_locusData.fWidth },
			{ posOld.x + cosf(fAngeleOld) * -m_locusData.fWidth , posOld.y + m_locusData.fHeight, posOld.z + sinf(fAngeleOld) * -m_locusData.fWidth },
		};

		CLocus::Create(aPos, aPosOld, m_locusData.col);
	}

}

//******************************
// �`�揈��
//******************************
void CCharacter::Draw(void)
{
	CModelShader::Draw();
}

//******************************
// �����t���O�̃Z�b�g
//******************************
void CCharacter::SetActiveAcceleration(bool bBool)
{
	// �����̃Z�b�g
	m_bAcceleration = bBool;

	if (m_bAcceleration)
	{// ������true��������
		// �J�E���g�̏�����
		m_nCntAccleration = 0;

		// �X�^����Ԃ̉���
		m_bStan = false;
	}
}

//******************************
// �X�^���t���O�̃Z�b�g
//******************************
void CCharacter::SetStan(bool bBool)
{
	// �����̃Z�b�g
	m_bStan = bBool;
	
	if (m_bStan)
	{// ������true��������

		// �J�E���g�̏�����
		m_nCntStan = 0;
		// ������Ԃ̉���
		m_bAcceleration = false;
		// �G�t�F�N�g����
		CStanEffect::SetEffect(GetPos());
	}
}

//******************************
// �A�C�e���̃Z�b�g����
//******************************
void CCharacter::SetItemObject(void)
{
	switch (m_item)
	{
	case CItem::ITEM_ACCELERATION:
		// �������[�h�ɂ���
		SetActiveAcceleration(true);

		if (m_bIsPlayer)
		{
			// SE�Đ�
			CManager::GetSound()->Play(CSound::LABEL_SE_ACCELERATION);
		}

		break;
	case CItem::ITEM_ATTACK:
	{
		// ������o��
		D3DXVECTOR3 attackPos = GetPos();
		// �v���C���[�̍��W�̎擾
		float fAngle = (-GetRot().y) + D3DXToRadian(90);
		attackPos.x += cosf(fAngle) * ATTACK_SET_DISTACE;
		attackPos.z += sinf(fAngle) * ATTACK_SET_DISTACE;

		// �U������
		CAttack::Create(attackPos, D3DXVECTOR3(0.0f, -GetRot().y + D3DXToRadian(90), 0.0f), m_rankData.nRank, GetID());

		if (m_bIsPlayer)
		{
			// SE�Đ�
			CManager::GetSound()->Play(CSound::LABEL_SE_ATTACK);
		}

	}
	break;

	case CItem::ITEM_TRAP:
	{
		// ������o��
		D3DXVECTOR3 trapPos = GetPos();
		// �v���C���[�̌����W�̎擾
		float fAngle = (-GetRot().y) + D3DXToRadian(-90);
		trapPos.x += cosf(fAngle) * TRAP_SET_DISTACE;
		trapPos.z += sinf(fAngle) * TRAP_SET_DISTACE;

		// �g���b�v����
		CTrap::Create(trapPos);

		if (m_bIsPlayer)
		{
			// SE�Đ�
			CManager::GetSound()->Play(CSound::LABEL_SE_ATTACK);
		}
	}
	break;
	default:
		break;
	}
	// �A�C�e�����̏�����
	SetItem(CItem::ITEM_NONE);
}

//******************************
// �d�͂̊Ǘ�
//******************************
void CCharacter::Gravity(void)
{
	if (m_bGravity)
	{// �d�̓t���O�������Ă���

		// �d�͗ʂ̐ݒ�
		m_gravityVec += (CHARACTER_GRAVITY - m_gravityVec)*CHARACTER_GRAVITY_RATE;

		// ���W�ɏd�͗ʂ̃v���X
		SetPos(GetPos() + m_gravityVec);
		// �J�E���g��i�߂�
		m_nCntGravity++;
		// ���J�E���g�ŗ���������
		if (m_nCntGravity >= CHARACTER_FALL_COUNT)
		{
			if (m_rankData.nCheck != 0)
			{
				SetPos(CGame::GetCheckPoint()->GetCollision(m_rankData.nCheck - 1)->GetPos());
			}
			else
			{
				SetPos(CGame::GetCheckPoint()->GetCollision(0)->GetPos());
			}

			// �@�̂̌�����i�s�����ɍ��킹��

			// ��ԋ߂����W�̎��̍��W�z��ԍ��̎擾
			int nDist = CGame::GetCpuPoint()->GetNearPosIndex(0, GetPos()) + 1;
			// �ő�l���z���Ȃ��悤�ɂ���
			if (nDist > CGame::GetCpuPoint()->GetPointNum(0)) nDist = 0;
			// �ڕW�ʒu
			D3DXVECTOR3 distPos = CGame::GetCpuPoint()->GetPointData(0, nDist).pos;

			D3DXVECTOR3 rot = GetRot();
			rot.y = atan2f(distPos.x - GetPos().x, distPos.z - GetPos().z);

			SetRot(rot);

			// �ړ��ʂ̏�����
			m_move = VEC3_ZERO;
			m_moveDist = VEC3_ZERO;

			// �����E�X�^���̃t���O����
			m_bAcceleration = false;
			SetStan(true);

			// �e�h���N���X�̗������Ƃ��̃A�N�V����
			FallAction();
		}
	}
	else
	{// �d�̓t���O�������ĂȂ��Ƃ�
	 // ������
		if (m_nCntGravity != 0)m_nCntGravity = 0;

		m_gravityVec = VEC3_ZERO;
	}
}

//******************************
// �����̊Ǘ�
//******************************
void CCharacter::Direction(void)
{
	// �����̎擾
	D3DXVECTOR3 rot = GetRot();

	// Y��

	// �ړ��ʂ�0����Ȃ��Ƃ�&&�X�^����Ԃ���Ȃ��Ƃ�
	if (m_move != VEC3_ZERO && !m_bStan)
	{
		
		// Y���̖ڕW�l
		float fRotYDist = atan2f(m_move.x, m_move.z);

		// �O����Ɖ�]���Ȃ��悤����
		while (rot.y - fRotYDist > D3DXToRadian(180))
		{
			fRotYDist += D3DXToRadian(360);
		}
		while (rot.y - fRotYDist < D3DXToRadian(-180))
		{
			fRotYDist -= D3DXToRadian(360);
		}

		// ���X�ɖڕW�l�ɋ߂Â���
		rot.y += (fRotYDist - rot.y) * CHARACTER_DIRECTION_RATE;

		// �����̃Z�b�g
		SetRot(rot);
	}

	// X��

	if (CGame::GetCourse() != NULL)
	{
		// ���C���o���ʒu
		D3DXVECTOR3 rayPos = VEC3_ZERO;
		rayPos.x = GetPos().x + (cosf(-GetRot().y + D3DXToRadian(90))*RAY_DISTANCE);
		rayPos.y = GetPos().y + RAY_HEIGHT;
		rayPos.z = GetPos().z + (sinf(-GetRot().y + D3DXToRadian(90))*RAY_DISTANCE);

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
				D3DXVECTOR3 hitPos = VEC3_ZERO;
				hitPos.x = rayPos.x;
				hitPos.y = rayPos.y - fHitDistance;
				hitPos.z = rayPos.z;

				// X���̖ڕW�l
				float fRotDistX = atan2f((-(hitPos.y - (GetPos().y - HOVER_HEIGHT))), RAY_DISTANCE);

				// ���X�ɖڕW�l�ɋ߂Â���
				rot.x += (fRotDistX - rot.x) * CHARACTER_DIRECTION_RATE;
				// �����̃Z�b�g
				SetRot(rot);
			}
		}
	}
}

//*****************************************
// �L�����N�^�[���m�̓����蔻�菈��
//*****************************************
void CCharacter::CollisionCharacter(void)
{
	// �L�����N�^�[�������[�v
	for (int nCnt = 0; nCnt < CGame::GetCharacterNum(); nCnt++)
	{
		// �L�����N�^�[���̎擾
		CCharacter *pTarget = CGame::GetCharacter(nCnt);

		// NULL�`�F�b�N&��������Ȃ��Ƃ�
		if (pTarget != NULL&&pTarget != this)
		{
			if (CCollision::CollisionSphere(m_pCollision, pTarget->m_pCollision))
			{// �������Ă�����
			 
				// �O�ɉ����o��
				D3DXVECTOR3 vec = (GetPos() - pTarget->GetPos());
				D3DXVec3Normalize(&vec, &vec);
				vec *= (m_pCollision->GetCollisionRadius() + pTarget->GetCollision()->GetCollisionRadius())+10;
				// ���W�̍X�V
				SetPos(pTarget->GetPos() + vec);
			}
		}
	}
}

//*****************************************
// �X�s�[�h�Ǘ�����
//*****************************************
void CCharacter::SpeedManager(void)
{
	if (m_bAcceleration)
	{// ����

		// ������Ԃ̃X�s�[�h�ɋ߂Â���
		m_fSpeed = CHARACTER_SPEED_ACCELERATION;
		
		// �J�E���g��i�߂�
		m_nCntAccleration++;
		if (m_nCntAccleration > CHARACTER_ACCELERATION_COUNT)
		{// �����J�E���g�����ɒB������
			// �����I��
			m_bAcceleration = false;
			// �J�E���g�̏�����
			m_nCntAccleration = 0;
		}
	}
	else if (m_bStan)
	{// �X�^�����

		// ������Ԃ̃X�s�[�h�ɋ߂Â���
		m_fSpeed =0.0f;

		// �X�^�����@�̂���]������
		D3DXVECTOR3 rot = GetRot();
		rot.y += D3DXToRadian(360 / (CHARACTER_STAN_FRAME));
		SetRot(rot);

		// �J�E���g��i�߂�
		m_nCntStan++;
		if (m_nCntStan > CHARACTER_STAN_FRAME)
		{// �����J�E���g�����ɒB������
		 // �����I��
			m_bStan = false;
			// �J�E���g�̏�����
			m_nCntStan = 0;

			if (m_bIsPlayer)
			{//�v���C���[��������
				//rot.y = ((CPlayer*)this)->GetDir() - D3DXToRadian(90);
				SetRot(rot);
			}
		}
	}
	else
	{
		// �������Ԃ̃X�s�[�h�ɋ߂Â���
		m_fSpeed = CHARACTER_SPEED_BASE;
	}
}
