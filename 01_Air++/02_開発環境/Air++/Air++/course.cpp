//=============================================================================
//
// course�w�b�_ [course.cpp]
// Author : ���V ����
//
//=============================================================================

//******************************
// �C���N���[�h
//******************************
#include "course.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "camera.h"
#include "character.h"

//*****************************
// �}�N����`
//*****************************
#define MODEL_PATH     "./data/Models/course002.x"         // ���f���̃p�X
#define CUBE_TEX_PATH  "./data/Textures/cube_sky.dds" // �L���[�u�e�N�X�`���̃p�X
#define SHADER_PATH    "./data/HLSL/Shader_course.fx"             // HLSL�t�@�C���̃p�X
#define ADJUST_HEIGHT 30  // �L�����N�^�[��肿����Əォ�烌�C���o��

//*****************************
// �ÓI�����o�ϐ��錾
//*****************************
CModel::Model   CCourse::m_model = {};  //���b�V�����ւ̃|�C���^
CScene::Shader CCourse::m_shader = {};  // �V�F�[�_�[�\����

//******************************
// �R���X�g���N�^
//******************************
CCourse::CCourse() :CModelShader(OBJTYPE_MAP)
{
	// �ϐ��̃N���A
}

//******************************
// �f�X�g���N�^
//******************************
CCourse::~CCourse()
{
}

//******************************
// �N���G�C�g
//******************************
CCourse * CCourse::Create(void)
{
	// �������̊m��
	CCourse *pCourse;
	pCourse = new CCourse;

	// ������
	pCourse->Init();

	return pCourse;
}

//******************************
// �e�N�X�`���̃��[�h
//******************************
HRESULT CCourse::Load(void)
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
void CCourse::Unload(void)
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
HRESULT CCourse::Init(void)
{
	if (FAILED(CModelShader::Init()))
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

	return S_OK;
}

//******************************
// �I������
//******************************
void CCourse::Uninit(void)
{
	CModelShader::Uninit();
}

//******************************
// �X�V����
//******************************
void CCourse::Update(void)
{
	// �L�����N�^�[�Ƃ̓����蔻�菈��
	CollisionCharacter();

	CModelShader::Update();
}

//******************************
// �`�揈��
//******************************
void CCourse::Draw(void)
{
	CModelShader::Draw();
}

//******************************
// �L�����N�^�[�Ƃ̓����蔻��*
//******************************
void CCourse::CollisionCharacter(void)
{
	for (int nCnt = 0; nCnt < CGame::GetCharacterNum(); nCnt++)
	{
		// �L�����N�^�[�������[�v
		CCharacter *pChara = CGame::GetCharacter(nCnt);

		if (pChara != NULL)
		{
			// �L�����N�^�[���W�̎擾
			D3DXVECTOR3 charaPos = pChara->GetPos();
			// �L�����N�^�[��肿����ƍ����ʒu����o��
			charaPos.y += ADJUST_HEIGHT;

			BOOL bHit = FALSE;     // ���C���������Ă��邩
			float fDistance = 0.0f;// ���C���������Ă��鋗��

			 // ���C
			D3DXIntersect(m_model.pMesh,
				&charaPos,
				&D3DXVECTOR3(0.0f, -1.0f, 0.0f),
				&bHit,
				NULL,
				NULL,
				NULL,
				&fDistance,
				NULL,
				NULL);

			if (bHit)
			{// ���C���������Ă�����

				if (fDistance <= HOVER_HEIGHT + ADJUST_HEIGHT - 1)
				{// ���ƃL�����N�^�[�̋��������ȏ�ɕۂ�

				 // �L�����N�^�[�̍��W�̍X�V
					charaPos.y = (charaPos.y - fDistance) + HOVER_HEIGHT /*- ADJUST_HEIGHT*/;
					pChara->SetPos(charaPos);

					if (pChara->GetActiveGravity())
					{// �d�̓t���O�������Ă���Ƃ�
					 // �d�͂�؂�
						pChara->SetActiveGravity(false);
					}
				}
				else if (fDistance >= HOVER_HEIGHT + ADJUST_HEIGHT - 1 && fDistance <= HOVER_HEIGHT + ADJUST_HEIGHT + 50)
				{// ������x�͈̔͂�������R�[�X�ɋz���t����

				 // �L�����N�^�[�̍��W�̍X�V
					charaPos.y = (charaPos.y - fDistance) + HOVER_HEIGHT /*- ADJUST_HEIGHT*/;
					pChara->SetPos(charaPos);

					if (pChara->GetActiveGravity())
					{// �d�̓t���O�������Ă���Ƃ�
					 // �d�͂�؂�
						pChara->SetActiveGravity(false);
					}
				}
				else
				{// �����痣�ꂷ���Ă���
					if (!pChara->GetActiveGravity())
					{// �d�̓t���O�������ĂȂ��Ƃ�
					 // �d�̓t���O�����Ă�
						pChara->SetActiveGravity(true);
					}
				}
			}
			else
			{// �����̉��ɏ����Ȃ�
				if (!pChara->GetActiveGravity())
				{// �d�̓t���O�������ĂȂ��Ƃ�
				 // �d�̓t���O�����Ă�
					pChara->SetActiveGravity(true);
				}
			}
		}
	}

}
//******************************
// �L�����N�^�[�Ƃ̓����蔻��*
//******************************
void CCourse::CollisionCharacter(CCharacter * pChara)
{
	if (pChara != NULL)
	{
		// �L�����N�^�[���W�̎擾
		D3DXVECTOR3 charaPos = pChara->GetPos();
		// �L�����N�^�[��肿����ƍ����ʒu����o��
		charaPos.y += ADJUST_HEIGHT;

		BOOL bHit = FALSE;     // ���C���������Ă��邩
		float fDistance = 0.0f;// ���C���������Ă��鋗��

							   // ���C
		D3DXIntersect(m_model.pMesh,
			&charaPos,
			&D3DXVECTOR3(0.0f, -1.0f, 0.0f),
			&bHit,
			NULL,
			NULL,
			NULL,
			&fDistance,
			NULL,
			NULL);

		if (bHit)
		{// ���C���������Ă�����

			if (fDistance <= HOVER_HEIGHT + ADJUST_HEIGHT - 1)
			{// ���ƃL�����N�^�[�̋��������ȏ�ɕۂ�

			 // �L�����N�^�[�̍��W�̍X�V
				charaPos.y = (charaPos.y - fDistance) + HOVER_HEIGHT /*- ADJUST_HEIGHT*/;
				pChara->SetPos(charaPos);

				if (pChara->GetActiveGravity())
				{// �d�̓t���O�������Ă���Ƃ�
				 // �d�͂�؂�
					pChara->SetActiveGravity(false);
				}
			}
			else if (fDistance >= HOVER_HEIGHT + ADJUST_HEIGHT - 1 && fDistance <= HOVER_HEIGHT + ADJUST_HEIGHT + 50)
			{// ������x�͈̔͂�������R�[�X�ɋz���t����

			 // �L�����N�^�[�̍��W�̍X�V
				charaPos.y = (charaPos.y - fDistance) + HOVER_HEIGHT /*- ADJUST_HEIGHT*/;
				pChara->SetPos(charaPos);

				if (pChara->GetActiveGravity())
				{// �d�̓t���O�������Ă���Ƃ�
				 // �d�͂�؂�
					pChara->SetActiveGravity(false);
				}
			}
			else
			{// �����痣�ꂷ���Ă���
				if (!pChara->GetActiveGravity())
				{// �d�̓t���O�������ĂȂ��Ƃ�
				 // �d�̓t���O�����Ă�
					pChara->SetActiveGravity(true);
				}
			}
		}
		else
		{// �����̉��ɏ����Ȃ�
			if (!pChara->GetActiveGravity())
			{// �d�̓t���O�������ĂȂ��Ƃ�
			 // �d�̓t���O�����Ă�
				pChara->SetActiveGravity(true);
			}
		}
	}
}
