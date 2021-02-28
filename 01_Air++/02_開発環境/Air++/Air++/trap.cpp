////////////////////////////////////////////////////
//
//    Trap�N���X�̏���[Trap.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////

//******************************
// �C���N���[�h
//******************************
#include "trap.h"
#include "manager.h"
#include "renderer.h"
#include "collision.h"
#include "billboard.h"
#include "character.h"
#include "game.h"
#include "player.h"
#include "camera.h"
#include "attack.h"
#include "sound.h"

//*****************************
// �}�N����`
//*****************************
#define MODEL_PATH   "./data/Models/trap.x"               // ���f���̃p�X
#define TEXTURE_PATH "./data/Textures/QuestionMark.png"   // �N�G�X�`�����}�[�N�e�N�X�`���̃p�X
#define TRAP_RADIUS 25                                    // ���a
#define TRAP_SIZE D3DXVECTOR3(0.35f,0.35f,0.35f)          // �T�C�Y*�{�b�N�X(�g�嗦)

//*****************************
// �ÓI�����o�ϐ��錾
//*****************************
CModel::Model   CTrap::m_model = {};   	     //���b�V�����ւ̃|�C���^

//******************************
// �R���X�g���N�^
//******************************
CTrap::CTrap() :CModel(OBJTYPE_ITEM)
{
	// �ϐ��̃N���A
	m_pCollision = NULL; // �����蔻��
}

//******************************
// �f�X�g���N�^
//******************************
CTrap::~CTrap()
{
}

//******************************
// �N���G�C�g
//******************************
CTrap * CTrap::Create(const D3DXVECTOR3 pos)
{
	// �������̊m��
	CTrap *pTrap;
	pTrap = new CTrap;

	// ������
	pTrap->Init();

	// �e�l�̑���E�Z�b�g
	pTrap->SetPriority(OBJTYPE_ITEM); // �I�u�W�F�N�g�^�C�v
	pTrap->SetPos(pos);               // ���W�̃Z�b�g

	// �����蔻��̐���
	pTrap->m_pCollision = CCollision::CreateSphere(pos, TRAP_RADIUS);

	return pTrap;
}

//******************************
// �e�N�X�`���̃��[�h
//******************************
HRESULT CTrap::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �R�[�X�������[�v

	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(MODEL_PATH,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_model.pBuffMat,
		NULL,
		&m_model.nNumMat,
		&m_model.pMesh);

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

	return S_OK;
}

//******************************
// �e�N�X�`���̃A�����[�h
//******************************
void CTrap::Unload(void)
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
HRESULT CTrap::Init(void)
{
	if (FAILED(CModel::Init()))
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

	// �T�C�Y�̐ݒ�
	SetSize(TRAP_SIZE);

	return S_OK;
}

//******************************
// �I������
//******************************
void CTrap::Uninit(void)
{
	if (m_pCollision != NULL)
	{
		m_pCollision->Uninit();
		m_pCollision = NULL;
	}

	CModel::Uninit();
}

//******************************
// �X�V����
//******************************
void CTrap::Update(void)
{
	// �L�����N�^�[�Ƃ̓����蔻��
	if(CollisionCharacter()) return;
	// �U���Ƃ̓����蔻��
	if (CollisionAttack()) return;
}

//******************************
// �`�揈��
//******************************
void CTrap::Draw(void)
{
	CModel::Draw();
}

//******************************
// �v���C���[�Ƃ̓����蔻��
//******************************
bool CTrap::CollisionCharacter(void)
{
	// �L�����N�^�[�������[�v
	for (int nCnt = 0; nCnt < CGame::GetCharacterNum(); nCnt++)
	{
		// �L�����N�^�[���̎擾
		CCharacter*pChara = CGame::GetCharacter(nCnt);

		if (CCollision::CollisionSphere(m_pCollision, pChara->GetCollision()))
		{// ���������Ƃ�

			if (!pChara->GetStan())
			{// �L�����N�^�[���X�^����Ԃɂ���
				pChara->SetStan(true);
				if (pChara->GetIsPlayer())
				{// �v���C���[��������
					// �J�����̃u���̐ݒ�
					CGame::GetCamera(((CPlayer*)pChara)->GetPlayerNum())->Shake(true);
					// SE�Đ�
					CManager::GetSound()->Play(CSound::LABEL_SE_HIT);
				}
			}
			// ����
			Uninit();

			return true;
		}
	}
	return false;
}

//*********************************
// �U���I�u�W�F�N�g�Ƃ̓����蔻��
//*********************************
bool CTrap::CollisionAttack(void)
{
	CAttack*pAttack = (CAttack*)GetTop(OBJTYPE_ATTACK);

	while (pAttack != NULL)
	{
		if (m_pCollision != NULL&&pAttack->GetCollision() != NULL)
		{
			if (CCollision::CollisionSphere(m_pCollision, pAttack->GetCollision()))
			{// �������Ă�����

			 // �U��������
				pAttack->Uninit();
				// �g���b�v������
				Uninit();

				// �����̏I��
				return true;
			}
		}
		
		pAttack = (CAttack*)pAttack->GetNext();
	}

	return false;
}
