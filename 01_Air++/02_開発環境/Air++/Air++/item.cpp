////////////////////////////////////////////////////
//
//    Item�N���X�̏���[Item.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////

//******************************
// �C���N���[�h
//******************************
#include "Item.h"
#include "manager.h"
#include "renderer.h"
#include "collision.h"
#include "billboard.h"
#include "character.h"
#include "game.h"
#include "cpu.h"
#include "itembox_effect.h"
#include "sound.h"

//*****************************
// �}�N����`
//*****************************
#define MODEL_PATH   "./data/Models/ItemBox.x"            // ���f���̃p�X
#define TEXTURE_PATH "./data/Textures/QuestionMark.png"   // �N�G�X�`�����}�[�N�e�N�X�`���̃p�X
#define ITEM_RADIUS 60                                    // ���a
#define ITEM_BOX_SIZE D3DXVECTOR3(0.4f,0.4f,0.4f)         // �T�C�Y*�{�b�N�X(�g�嗦)
#define QUESTION_SIZE D3DXVECTOR3(30.0f,30.0f,0.0f)       // �T�C�Y*?�}�[�N
#define ITEM_ROTASION_VEC D3DXVECTOR3(0.02f,0.03f,0.008f) // �A�C�e���̉�]���x
#define ITEM_REPOP_COUNT 100                              // �A�C�e���̃��|�b�v�̃J�E���g
#define ITEM_SIZE_RATE_UP   0.15f                         // �A�C�e���T�C�Y"�g��"���̌W��
#define ITEM_SIZE_RATE_DOWN 0.2f                          // �A�C�e���T�C�Y"�k��"���̌W��
#define ITEM_SIZE_DIST D3DXVECTOR3(1.0f,1.0f,1.0f)        // �A�C�e���T�C�Y�g�厞�ڕW�l
#define ITEM_SIZE_CHANGE D3DXVECTOR3(0.8f,0.8f,0.8f)      // �T�C�Y�����̒l�𒴂�����k���Ɉڍs����

//*****************************
// �ÓI�����o�ϐ��錾
//*****************************
CModel::Model   CItem::m_model = {};   	     //���b�V�����ւ̃|�C���^
LPDIRECT3DTEXTURE9 CItem::m_pTexture = NULL; // �e�N�X�`��
D3DXVECTOR3 CItem::m_rot = VEC3_ZERO;        // �A�C�e���̌���

//******************************
// �R���X�g���N�^
//******************************
CItem::CItem() :CModel(OBJTYPE_ITEM)
{
	// �ϐ��̃N���A
	m_pCollision = NULL; // �����蔻��
	m_bActive = true;    // �A�C�e�����L����
	m_nCntRePop = 0;     // �A�C�e�����|�b�v�J�E���g
	m_bExpansion = false;// �T�C�Y�g��t���O
}

//******************************
// �f�X�g���N�^
//******************************
CItem::~CItem()
{
}

//******************************
// �N���G�C�g
//******************************
CItem * CItem::Create(const D3DXVECTOR3 pos)
{
	// �������̊m��
	CItem *pItem;
	pItem = new CItem;

	// ������
	pItem->Init();

	// �e�l�̑���E�Z�b�g
	pItem->SetPriority(OBJTYPE_ITEM); // �I�u�W�F�N�g�^�C�v
	pItem->SetPos(pos);               // ���W�̃Z�b�g

	// �����蔻��̐���
	pItem->m_pCollision = CCollision::CreateSphere(pos, ITEM_RADIUS);

	// �͂Ăȃ}�[�N�̐���
	pItem->m_pBillboard = CBillboard::Create(pos, QUESTION_SIZE);
	// �e�N�X�`���̊��蓖��
	pItem->m_pBillboard->BindTexture(m_pTexture);

	return pItem;
}

//******************************
// �e�N�X�`���̃��[�h
//******************************
HRESULT CItem::Load(void)
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

	// ?�}�[�N�e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_PATH, &m_pTexture);
	return S_OK;
}

//******************************
// �e�N�X�`���̃A�����[�h
//******************************
void CItem::Unload(void)
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

	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//******************************
// �A�C�e���̉�]����
//******************************
void CItem::RotationItem(void)
{
	m_rot += ITEM_ROTASION_VEC;
}

//******************************
// ����������
//******************************
HRESULT CItem::Init(void)
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
	SetSize(ITEM_BOX_SIZE);

	// �ϐ��̏�����
	m_bActive = true;    // �A�C�e�����L����
	m_nCntRePop = 0;     // �A�C�e�����|�b�v�J�E���g

	return S_OK;
}

//******************************
// �I������
//******************************
void CItem::Uninit(void)
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
void CItem::Update(void)
{
	if (m_bActive)
	{// �L����Ԃ̎�
		// �L�����N�^�[�Ƃ̓����蔻��
		CollisionCharacter();
		// ��]�̃Z�b�g
		SetRot(m_rot);
	}
	else
	{
		m_nCntRePop++;
		if (m_nCntRePop > ITEM_REPOP_COUNT)
		{
			// �J�E���g�̏�����
			m_nCntRePop = 0;

			// �L���ɂ���
			m_bActive = true;

			// �͂Ăȃ}�[�N�̐���
			m_pBillboard = CBillboard::Create(GetPos(), QUESTION_SIZE);
			// �e�N�X�`���̊��蓖��
			m_pBillboard->BindTexture(m_pTexture);

			SetSize(ITEM_BOX_SIZE);
		}
		else
		{
			// �T�C�Y�̎擾
			D3DXVECTOR3 size = GetSize();
			
			if (m_bExpansion)
			{// �T�C�Y�g��͈�
				size += (ITEM_SIZE_DIST - size)*ITEM_SIZE_RATE_UP;
				
				if (size.x >= ITEM_SIZE_CHANGE.x&&size.y >= ITEM_SIZE_CHANGE.y&&size.z >= ITEM_SIZE_CHANGE.z)
				{
					m_bExpansion = false;
				}
			}
			else
			{// �T�C�Y�k���͈�
				size += (VEC3_ZERO - size)*ITEM_SIZE_RATE_DOWN;
			}
			SetSize(size);

			// �A�C�e���擾���͉�]�𑬂�����
			SetRot(GetRot() + ITEM_ROTASION_VEC * 5);
		}
	}
	
}

//******************************
// �`�揈��
//******************************
void CItem::Draw(void)
{
	CModel::Draw();
}

//******************************
// �v���C���[�Ƃ̓����蔻��
//******************************
void CItem::CollisionCharacter(void)
{
	// �L�����N�^�[�������[�v
	for (int nCnt = 0; nCnt < CGame::GetCharacterNum(); nCnt++)
	{
		// �L�����N�^�[���̎擾
		CCharacter*pChara = CGame::GetCharacter(nCnt);

		if (CCollision::CollisionSphere(m_pCollision, pChara->GetCollision()))
		{// ���������Ƃ�
			// �A�C�e���𖳌��ɂ���
			m_bActive = false;
			// �T�C�Y�g��t���O�𗧂Ă�
			m_bExpansion = true;
			// �͂Ăȃ}�[�N������
			m_pBillboard->Uninit();

			// �j�ЃG�t�F�N�g�̐���
			CItemBoxEffect::SetEffect(GetPos(), -pChara->GetRot().y + D3DXToRadian(90));

			if (pChara->GetItem() == ITEM_NONE)
			{// �L�������A�C�e���������Ă��Ȃ��Ƃ�
				// �����_���ŃA�C�e���̎擾
				pChara->SetItem((ITEM_TYPE)(rand() % ITEM_MAX));

				if (!pChara->GetIsPlayer())
				{// NPC��������
					//	�A�C�e���J�E���g�̃Z�b�g
					((CCpu*)pChara)->SetItemCount((rand() % CPU_ITEM_USE_RAND_AMPLITUDE) + CPU_ITEM_USE_RAND_MIN);
				}
			}

			if (pChara->GetIsPlayer())
			{// �v���C���[��������
				// SE�Đ�
				CManager::GetSound()->Play(CSound::LABEL_SE_ITEM);
			}
			break;
		}
	}
	
}
