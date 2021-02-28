//=============================================================================
//
// attack�w�b�_ [attack.cpp]
// Author : ���V ����
//
//=============================================================================

//******************************
// �C���N���[�h
//******************************
#include "attack.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "camera.h"
#include "keyboard.h"
#include "collision.h"
#include "cpu_point.h"
#include "player.h"
#include "sound.h"

//*****************************
// �}�N����`
//*****************************
#define MODEL_PATH   "./data/Models/attack.x" // ���f���̃p�X*���[�v�X�^�[
#define SHADER_PATH  "./data/HLSL/Shader.fx"  // HLSL�t�@�C���̃p�X
#define ATTACK_SIZE D3DXVECTOR3( 0.5f, 0.5f, 0.5f) // ���f���T�C�Y
#define ATTACK_DIST_RADIUS 100.0f             // �ڕW�̒n�_���a
#define ATTACK_TARGET_RADIUS 500.0f           // �U���Ώی��������蔻��
#define DIST_RAND_AMPLITUDE 800               // �ڕW�n�_�ݒ�̗����̐U��
#define ATTACK_SPEED 45.0f                    // �ړ����x
#define ATTACK_LIFE 1200.0f                   // ����

//*****************************
// �ÓI�����o�ϐ��錾
//*****************************
CModel::Model  CAttack::m_model = {};  //���b�V�����ւ̃|�C���^
CScene::Shader CAttack::m_shader = {}; // �V�F�[�_�[�\����

//******************************
// �R���X�g���N�^
//******************************
CAttack::CAttack() :CCharacter(OBJTYPE_ATTACK)
{
	m_nPointNum = 0;           // �R�[�X�f�[�^�z��Ǘ��p
	m_pDIstCollision = NULL;   // �ړ��ڕW�l�����蔻��
	m_nRank = 0;               // �������l�̏���
	m_nLife = 0;               // ����
	m_pTargetCollision = NULL; // �U���ڕW�����蔻��
	m_nRoot = 0;               // ���[�g
}

//******************************
// �f�X�g���N�^
//******************************
CAttack::~CAttack()
{
}

//******************************
// �N���G�C�g
//******************************
CAttack * CAttack::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot,int nRank,int nUseID)
{
	// �������̊m��
	CAttack *pAttack;
	pAttack = new CAttack;

	// ������
	pAttack->Init();

	// �e�l�̑���E�Z�b�g
	pAttack->SetPos(pos);       // ���W�̃Z�b�g
	pAttack->SetRot(rot);       // ���W�̃Z�b�g
	pAttack->m_nRank = nRank;   // ���ʂ̃Z�b�g 
	pAttack->m_nUseID = nUseID; // �������l�̃I�u�W�F�N�g�ԍ�
	pAttack->GetCollision()->SetPos(pos); // �����蔻��̈ʒu�̐ݒ�

	// ��ԋ߂��n�_�̎��̔ԍ��������C���f�b�N�X�ɂ���*��Ԗڂ��ƈ��t��������*
	pAttack->m_nPointNum = CGame::GetCpuPoint()->GetNearPosIndex(pAttack->m_nRoot,pos) + 1;
	if (CGame::GetCpuPoint()->GetPointNum(pAttack->m_nRoot) < pAttack->m_nPointNum) pAttack->m_nPointNum = 0;
	// �ڕW�n�_
	pAttack->m_pDIstCollision = CCollision::CreateSphere(CGame::GetCpuPoint()->GetPointData(pAttack->m_nRoot , pAttack->m_nPointNum).pos, ATTACK_DIST_RADIUS);

	// �U���Ώی��������蔻��̐���
	pAttack->m_pTargetCollision = CCollision::CreateSphere(pos, ATTACK_TARGET_RADIUS);
	return pAttack;
}

//******************************
// �e�N�X�`���̃��[�h
//******************************
HRESULT CAttack::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//X�t�@�C���̓ǂݍ���
	// ���[�v�X�^�[
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
	D3DXCreateEffectFromFile(CManager::GetRenderer()->GetDevice(), SHADER_PATH, NULL, NULL, 0, NULL, &m_shader.pEffect, NULL);

	sprintf(&m_shader.achTechniqueName[0], "%s", "TShader");

	return S_OK;
}

//******************************
// �e�N�X�`���̃A�����[�h
//******************************
void CAttack::Unload(void)
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
// ����������
//******************************
HRESULT CAttack::Init(void)
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
	// �R�[�X�f�[�^�z��Ǘ��p�ϐ��̏�����
	m_nPointNum = 0;
	// �T�C�Y�̃Z�b�g
	SetSize(ATTACK_SIZE);

	// ���[�g�����_���Őݒ�
	m_nRoot = rand() % ROOT_NUM;

	// �ړ��t���O
	SetMoveFlag(true);
	return S_OK;
}

//******************************
// �I������
//******************************
void CAttack::Uninit(void)
{
	CCharacter::Uninit();

	if (m_pTargetCollision != NULL)
	{
		m_pTargetCollision->Uninit();
		m_pTargetCollision = NULL;
	}

	if (m_pDIstCollision != NULL)
	{
		m_pDIstCollision->Uninit();
		m_pDIstCollision = NULL;
	}
}

//******************************
// �X�V����
//******************************
void CAttack::Update(void)
{

	// �ړ��ʂ̐ݒ�
	Move();

	// �ڕW�n�_�̊Ǘ�
	DistManager();

	if (m_pTargetCollision != NULL)
	{
		// �����蔻��̈ʒu���X�V
		m_pTargetCollision->SetPos(GetPos());
	}

	// �L�����N�^�[�N���X�̍X�V����*�ړ��A�d�́A�����蔻��Aetc.
	CCharacter::Update();
}

//******************************
// �`�揈��
//******************************
void CAttack::Draw(void)
{
	CCharacter::Draw();
}

//******************************
// �ړ��ʂ̐ݒ菈��
//******************************
void CAttack::Move(void)
{
	// �ړ��ʂ̎擾
	D3DXVECTOR3 move = GetMove();

	if (m_pDIstCollision != NULL)
	{
		// ���ݒn�_����ڕW�n�_�ւ̃x�N�g���̎擾
		D3DXVECTOR3 moveDist = m_pDIstCollision->GetPos() - GetPos();
		// Y���͈ړ����Ȃ�
		moveDist.y = 0;
		// ���K��
		D3DXVec3Normalize(&moveDist, &moveDist);
		// �X�s�[�h��������
		moveDist *= ATTACK_SPEED;

		// �ړ��ʂ̃Z�b�g*�W���𖳎�����
		SetMoveDist(moveDist);
	}

}

//******************************
// �ڕW�n�_�̊Ǘ�
//******************************
void CAttack::DistManager(void)
{
	// �ڕW�n�_�ɂԂ������Ƃ�
	if (CCollision::CollisionSphere(GetCollision(), m_pDIstCollision))
	{
		// �ڕW�n�_�C���f�b�N�X��i�߂�
		m_nPointNum++;

		// �ڕW�l�_���𒴂�����C���f�b�N�X��0�ɂ���
		if (m_nPointNum >= CGame::GetCpuPoint()->GetPointNum(m_nRoot))
		{
			m_nPointNum = 0;
		}

		// �R�[�X�f�[�^�̎擾
		CCpuPoint::CpuPointData pointData = CGame::GetCpuPoint()->GetPointData(m_nRoot,m_nPointNum);
		if (m_pDIstCollision != NULL)
		{
			// ���̖ڕW�n�_
			m_pDIstCollision->SetPos(pointData.pos);
		}
	}

	// �����𑪂�p
	float fDistance = 99999.0f;
	for (int nCnt = 0; nCnt < CGame::GetCharacterNum(); nCnt++)
	{
		CCharacter * pChara = CGame::GetCharacter(nCnt);
		
		if (pChara != NULL&&pChara->GetRankData().nRank<m_nRank)
		{// Null�`�F�b�N&���ʂ��������l����̎�

			if (pChara->GetID() != m_nUseID)
			{// �������l�ɂ͒Ǐ]�s��
				if (m_pTargetCollision != NULL&&pChara->GetCollision() != NULL)
				{
					if (CCollision::CollisionSphere(pChara->GetCollision(), m_pTargetCollision) && fDistance > D3DXVec3Length(&(GetPos() - pChara->GetPos())))
					{// ���G�͈͂ɃL�����������Ă�����&&���������̃L�������߂�������

						// �����̕ۑ�
						fDistance = D3DXVec3Length(&(GetPos() - pChara->GetPos()));

						// �|�C���g�̃C���f�b�N�X����ԋ߂��n�_�̎��̔ԍ����ɂ���*��Ԗڂ��ƈ��t��������*
						m_nPointNum = CGame::GetCpuPoint()->GetNearPosIndex(m_nRoot, GetPos()) + 1;
						// �N�����v����
						if (CGame::GetCpuPoint()->GetPointNum(m_nRoot) < m_nPointNum) m_nPointNum = 0;

						if (m_pDIstCollision != NULL)
						{
							// ���̖ڕW�n�_
							m_pDIstCollision->SetPos(pChara->GetPos());
						}
					}
				}
			}
		}
	}
}

//******************************
// �v���C���[�ENPC�Ƃ̓����蔻��*�L�����N�^�[�N���X�̍X�V�ŌĂ΂�Ă�
//******************************
void CAttack::CollisionCharacter(void)
{
	// �L�����N�^�[�������[�v
	for (int nCnt = 0; nCnt < CGame::GetCharacterNum(); nCnt++)
	{
		// �L�����N�^�[���̎擾
		CCharacter*pChara = CGame::GetCharacter(nCnt);
		if (pChara->GetID() != m_nUseID)
		{// �������l�ɂ͓�����Ȃ��悤�ɂ���

			if (CCollision::CollisionSphere(GetCollision(), pChara->GetCollision()))
			{// ���������Ƃ�

				if (!pChara->GetStan())
				{// �L�����N�^�[���X�^����Ԃɂ���
					pChara->SetStan(true);
					if (pChara->GetIsPlayer())
					{// �v���C���[��������

						// �J������h�炷
						CGame::GetCamera(((CPlayer*)pChara)->GetPlayerNum())->Shake(true);

						// SE�Đ�
						CManager::GetSound()->Play(CSound::LABEL_SE_HIT);
					}
				}

				// ����
				Uninit();
				break;
			}
		}
	}
}

//******************************
// �������̃A�N�V����
//******************************
void CAttack::FallAction(void)
{
	// ����
	Uninit();
}
