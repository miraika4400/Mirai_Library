////////////////////////////////////////////////////
//
//    particle�N���X�̏���[particle.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////

//******************************
// �C���N���[�h
//******************************
#include "checkpoint.h"
#include "manager.h"
#include "keyboard.h"
#include "debug_log.h"
#include "collision.h"
#include "game.h"
#include "character.h"
#include "player.h"

//*****************************
// �}�N����`
//*****************************
#define CHECKPOINT_RADIUS 700                        // �����蔻��̔��a
#define CHECKPOINT_TEXT "data/Texts/CoursePoint.txt" // �`�F�b�N�|�C���g�e�L�X�g�̃p�X
#define GOAL_LAP_NUM 3                               // ���T�ŃS�[����

// �`�F�b�N�|�C���g�z�u���[�h�؂�ւ��}�N��
#define SET_MODE
#undef  SET_MODE

//*****************************
// �ÓI�����o�ϐ��錾
//*****************************

//******************************
// �R���X�g���N�^
//******************************
CCheckPoint::CCheckPoint() :CScene(OBJTYPE_CHECKPOINT)
{
	// �ϐ��̃N���A
	m_nNumPoint = 0;
	memset(&m_pCollision, 0, sizeof(m_pCollision));
	memset(&m_pointPos, 0, sizeof(m_pointPos));
}

//******************************
// �f�X�g���N�^
//******************************
CCheckPoint::~CCheckPoint()
{
}

//******************************
// �N���G�C�g
//******************************
CCheckPoint * CCheckPoint::Create(void)
{
	// �������̊m��
	CCheckPoint* pCheckPoint = new CCheckPoint;

	// ������
	pCheckPoint->Init();

	return pCheckPoint;
}

//******************************
// ����������
//******************************
HRESULT CCheckPoint::Init(void)
{
	// �I�u�W�F�N�g�^�C�v�̐ݒ�
	SetPriority(OBJTYPE_CHECKPOINT);

#ifndef SET_MODE
	// �`�F�b�N�|�C���g�̓ǂݍ���
	LoadCheckpoint();

#endif // !SET_MODE

	return S_OK;
}

//******************************
// �I������
//******************************
void CCheckPoint::Uninit(void)
{
	Release();
}

//******************************
// �X�V����
//******************************
void CCheckPoint::Update(void)
{

#ifdef SET_MODE
	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_SPACE))
	{
		m_pointPos[m_nNumPoint] = ((CPlayer*)GetTop(OBJTYPE_PLAYER))->GetPos();

		m_pCollision[m_nNumPoint] = CCollision::CreateSphere(m_pointPos[m_nNumPoint], CHECKPOINT_RADIUS);
	
		m_nNumPoint++;
	}
	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_BACKSPACE))
	{
		m_nNumPoint--;
		m_pCollision[m_nNumPoint]->Uninit();
	}
	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_F1))
	{
		SaveCheckpoint();

		CDebugLog::Init();
		CDebugLog::Print("save");
	}
#else
	// �L�����N�^�[�������[�v
	for (int nCntCharacter = 0; nCntCharacter < CGame::GetCharacterNum(); nCntCharacter++)
	{
		// �L�����N�^�[���̎擾
		CCharacter*pChara = CGame::GetCharacter(nCntCharacter);
		if (pChara != NULL && !pChara->GetGoalFlag())
		{
			// ���ʕt���f�[�^�̎擾
			CCharacter::RankData rankData = pChara->GetRankData();

			// �`�F�b�N�|�C���g�������[�v
			for (int nCntPoint = 0; nCntPoint < m_nNumPoint; nCntPoint++)
			{
				// �`���b�N�|�C���g�ԍ��̔�r
				if (rankData.nCheck == nCntPoint)
				{
					// �`���b�N�|�C���g�Ƃ̓����蔻��
					if (CCollision::CollisionSphere(pChara->GetCollision(), m_pCollision[nCntPoint]))
					{
						// �`�F�b�N�|�C���g�𑝂₷
						rankData.nCheck++;

						// �������
						if (rankData.nCheck >= m_nNumPoint)
						{
							rankData.nCheck = 0;
							rankData.nLap++;

							// ���b�v�����S�[���ɒB������
							if (rankData.nLap >= GOAL_LAP_NUM)
							{
								// �S�[����Ԃɂ���
								pChara->GoalAction();
							}
						}
						// �L�����N�^�[�N���X�Ƀ����N�f�[�^���Z�b�g����
						pChara->SetRankData(rankData);
					}
				}
			}
		}
	}

#endif
}

//******************************
// �`�揈��
//******************************
void CCheckPoint::Draw(void)
{
}

//******************************
// �Z�[�u
//******************************
void CCheckPoint::SaveCheckpoint(void)
{
	// �t�@�C���I�[�v��
	FILE *pFile = NULL;

	// �R�[�X�̃^�C�v�ɂ�����ĕ�����
	pFile = fopen(CHECKPOINT_TEXT, "w");

	if (pFile != NULL)
	{
		// �`�F�b�N�|�C���g���̏�������
		fprintf(pFile, "%d\n", m_nNumPoint);

		// ���W�̏�������
		for (int nCnt = 0; nCnt < m_nNumPoint; nCnt++)
		{
			fprintf(pFile, "%.2f,%.2f,%.2f\n", m_pointPos[nCnt].x, m_pointPos[nCnt].y, m_pointPos[nCnt].z);
		}
		// �t�@�C���N���[�Y
		fclose(pFile);
	}
}

//******************************
// ���[�h
//******************************
void CCheckPoint::LoadCheckpoint(void)
{
	// �t�@�C���I�[�v��
	FILE *pFile = NULL;

	// �R�[�X�̃^�C�v�ɂ�����ĕ�����
	pFile = fopen(CHECKPOINT_TEXT, "r");

	if (pFile != NULL)
	{
		// �`�F�b�N�|�C���g���̓ǂݍ���
		fscanf(pFile, "%d", &m_nNumPoint);

		// �`�F�b�N�|�C���g�������[�v
		for (int nCnt = 0; nCnt < m_nNumPoint; nCnt++)
		{
			// ���W�̓ǂݍ���
			fscanf(pFile, "%f,%f,%f\n", &m_pointPos[nCnt].x, &m_pointPos[nCnt].y, &m_pointPos[nCnt].z);
			
			// �R���W�����̐���
			m_pCollision[nCnt] = CCollision::CreateSphere(m_pointPos[nCnt], CHECKPOINT_RADIUS);
		}

		// �t�@�C���N���[�Y
		fclose(pFile);
	}
}
