////////////////////////////////////////////////////
//
//    FeverLogo�̏���[FeverLogo.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////


//=============================
// �C���N���[�h
//=============================
#include "rank.h"
#include "manager.h"
#include "renderer.h"
#include "polygon.h"
#include "keyboard.h"
#include "mouse.h"
#include "joypad.h"
#include "fade.h"
#include "sound.h"
#include "game.h"
#include "player.h"
#include "checkpoint.h"
#include "collision.h"

//**********************************
// �}�N����`
//**********************************

//**********************************
// �ÓI�����o�ϐ��錾
//**********************************


//=============================
// �R���X�g���N�^
//=============================
CRank::CRank() :CScene(OBJTYPE_SYSTEM)
{
	
}

//=============================
// �f�X�g���N�^
//=============================
CRank::~CRank()
{
}

//=============================
// �N���G�C�g
//=============================
CRank * CRank::Create()
{
	// �������̊m��
	CRank *pRank = new CRank;

	// ������
	pRank->Init();
	return pRank;
}

//=============================
// ����������
//=============================
HRESULT CRank::Init(void)
{

	return S_OK;
}

//=============================
// �I������
//=============================
void CRank::Uninit(void)
{
	Release();
}

//=============================
// �X�V����
//=============================
void CRank::Update(void)
{
	// ���ʂ̊Ǘ�
	RankManager();
}

//=============================
// �`�揈��
//=============================
void CRank::Draw(void)
{
}

//==================================
// ���ʂ̊Ǘ�
//==================================
void CRank::RankManager(void)
{
	// �v���C���[�z��
	CCharacter*pCharacter[MAX_CHARACTER_NUM] = {};

	// �v���C���[�l���̎擾
	int nNumCharacter = CGame::GetCharacterNum();

	// �z��Ƀv���C���[���i�[
	for (int nCnt = 0; nCnt < nNumCharacter; nCnt++)
	{
		if (CGame::GetCharacter(nCnt) != NULL)
		{
			pCharacter[nCnt] = CGame::GetCharacter(nCnt);
		}
	}

	//	���݂̃����N���Ƀ\�[�g����
	for (int nCnt = 0; nCnt < nNumCharacter; nCnt++)
	{
		for (int nCnt2 = 0; nCnt2 < nNumCharacter; nCnt2++)
		{
			if (pCharacter[nCnt]->GetRankData().nRank < pCharacter[nCnt2]->GetRankData().nRank)
			{// �`�F�b�N�|�C���g�̐��Ń\�[�g

				CCharacter*pSave = pCharacter[nCnt];
				pCharacter[nCnt] = pCharacter[nCnt2];
				pCharacter[nCnt2] = pSave;

				continue;
			}
		}
	}

	//	���񐔁A���b�v���A�`�F�b�N�|�C���g�܂ł̋����Ń\�[�g����
	for (int nCnt = 0; nCnt < nNumCharacter; nCnt++)
	{
		for (int nCnt2 = 0; nCnt2 < nNumCharacter; nCnt2++)
		{

			if (pCharacter[nCnt] != NULL && pCharacter[nCnt2] != NULL)
			{

				if (!pCharacter[nCnt]->GetGoalFlag() && !pCharacter[nCnt2]->GetGoalFlag())
				{
					if (pCharacter[nCnt]->GetRankData().nLap > pCharacter[nCnt2]->GetRankData().nLap)
					{// �`�F�b�N�|�C���g�̐��Ń\�[�g

						CCharacter*pSave = pCharacter[nCnt];
						pCharacter[nCnt] = pCharacter[nCnt2];
						pCharacter[nCnt2] = pSave;

						continue;
					}
					else if (pCharacter[nCnt]->GetRankData().nLap == pCharacter[nCnt2]->GetRankData().nLap &&
						pCharacter[nCnt]->GetRankData().nCheck > pCharacter[nCnt2]->GetRankData().nCheck)
					{// �`�F�b�N�|�C���g�̐��Ń\�[�g
						CCharacter*pSave = pCharacter[nCnt];
						pCharacter[nCnt] = pCharacter[nCnt2];
						pCharacter[nCnt2] = pSave;

						continue;
					}
					else if (pCharacter[nCnt]->GetRankData().nCheck == pCharacter[nCnt2]->GetRankData().nCheck)
					{// �`�F�b�N�|�C���g�����ꏏ��������

						int nCheck = pCharacter[nCnt]->GetRankData().nCheck;
						
						float fProgress = CheckProgress(nCheck, pCharacter[nCnt]->GetPos());
						
						if (fProgress > CheckProgress(nCheck, pCharacter[nCnt2]->GetPos()))
						{
							CCharacter*pSave = pCharacter[nCnt];
							pCharacter[nCnt] = pCharacter[nCnt2];
							pCharacter[nCnt2] = pSave;
							continue;
						}

						//D3DXVECTOR3 checkPos = CGame::GetCheckPoint()->GetCollision(pCharacter[nCnt]->GetRankData().nCheck)->GetPos();
						//
						//// �`�F�b�N�|�C���g�Ƃ̋����Ń\�[�g
						//float fDistance = sqrtf(powf(checkPos.x - pCharacter[nCnt]->GetPos().x, 2) +
						//	powf(checkPos.y - pCharacter[nCnt]->GetPos().y, 2) +
						//	powf(checkPos.z - pCharacter[nCnt]->GetPos().z, 2));
						//
						//if (fDistance < sqrtf(powf(checkPos.x - pCharacter[nCnt2]->GetPos().x, 2) +
						//	powf(checkPos.y - pCharacter[nCnt2]->GetPos().y, 2) +
						//	powf(checkPos.z - pCharacter[nCnt2]->GetPos().z, 2)))
						//
						//{
						//	CCharacter*pSave = pCharacter[nCnt];
						//	pCharacter[nCnt] = pCharacter[nCnt2];
						//	pCharacter[nCnt2] = pSave;
						//	continue;
						//}
					}
				}
			}
		}
	}

	// �z��̏��Ԃ����̂܂܏���
	for (int nCnt = 0; nCnt < nNumCharacter; nCnt++)
	{
		if (pCharacter[nCnt] != NULL)
		{
			// �����N�f�[�^�̎擾
			CCharacter::RankData rankData = pCharacter[nCnt]->GetRankData();
			// ���ʂ͓���ԍ�+1
			rankData.nRank = nCnt + 1;
			// �����N�f�[�^�̃Z�b�g
			pCharacter[nCnt]->SetRankData(rankData);
		}
	}
}

//==================================
// �i�s�x�̎Z�o
//==================================
float CRank::CheckProgress(int nCheckNum , D3DXVECTOR3 pos)
{
	// �ŐV�Œʉ߂��Ă���`�F�b�N�|�C���g�̍��W
	D3DXVECTOR3 nextCheckPos = CGame::GetCheckPoint()->GetCollision(nCheckNum)->GetPos();
	
	// ���̃`�F�b�N�|�C���g�ԍ�
	int nowCheckNum = nCheckNum - 1;
	if (nowCheckNum < 0) nowCheckNum = CGame::GetCheckPoint()->GetCheckPointNum()-1;
	// ���̃`�F�b�N�|�C���g�̍��W�̎擾
	D3DXVECTOR3 nowCheckPos = CGame::GetCheckPoint()->GetCollision(nowCheckNum)->GetPos();

	// �i�s�����ւ̒P�ʃx�N�g��
	D3DXVECTOR3 progressDir;
	progressDir = nextCheckPos - nowCheckPos;
	
	// ���ς�p���Đi�s�x�̎Z�o���ĕԂ�
	return (float)D3DXVec3Dot(&progressDir, &(pos - nowCheckPos));
}
