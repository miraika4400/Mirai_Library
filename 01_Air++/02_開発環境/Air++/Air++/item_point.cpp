////////////////////////////////////////////////////
//
//    item_point�N���X�̏���[item_point.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////

//******************************
// �C���N���[�h
//******************************
#include "item_point.h"
#include "manager.h"
#include "keyboard.h"
#include "debug_log.h"
#include "collision.h"
#include "game.h"
#include "character.h"
#include "player.h"
#include "item.h"

//*****************************
// �}�N����`
//*****************************
#define ITEMPOINT_TEXT "data/Texts/ItemPoint.txt" // �`�F�b�N�|�C���g�e�L�X�g�̃p�X

// �`�F�b�N�|�C���g�z�u���[�h�؂�ւ��}�N��
#define SET_MODE
#undef  SET_MODE

//*****************************
// �ÓI�����o�ϐ��錾
//*****************************

//******************************
// �R���X�g���N�^
//******************************
CItemPoint::CItemPoint() :CScene(OBJTYPE_CHECKPOINT)
{
	// �ϐ��̃N���A
	m_nItemNum = 0;
	memset(&m_aItemPos, 0, sizeof(m_aItemPos));
}

//******************************
// �f�X�g���N�^
//******************************
CItemPoint::~CItemPoint()
{
}

//******************************
// �N���G�C�g
//******************************
CItemPoint * CItemPoint::Create(void)
{
	// �������̊m��
	CItemPoint* pCpuPoint = new CItemPoint;

	// ������
	pCpuPoint->Init();

	return pCpuPoint;
}

//******************************
// ����������
//******************************
HRESULT CItemPoint::Init(void)
{
	// �I�u�W�F�N�g�^�C�v�̐ݒ�
	SetPriority(OBJTYPE_CHECKPOINT);

	m_nItemNum = 0;
#ifndef SET_MODE

	// �`�F�b�N�|�C���g�̓ǂݍ���
	LoadText();

#endif // !SET_MODE

	return S_OK;
}

//******************************
// �I������
//******************************
void CItemPoint::Uninit(void)
{
	Release();
}

//******************************
// �X�V����
//******************************
void CItemPoint::Update(void)
{

#ifdef SET_MODE
	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_SPACE))
	{	
		// ���W
		m_aItemPos[m_nItemNum] = ((CPlayer*)GetTop(OBJTYPE_PLAYER))->GetPos();
		
		// �A�C�e���̐���
		CItem::Create(m_aItemPos[m_nItemNum]);
		// �|�C���g���̉��Z
		m_nItemNum++;
	}


	if (CManager::GetKeyboard()->GetKeyTrigger(DIK_F1))
	{
		SaveText();
	}

#endif
}

//******************************
// �`�揈��
//******************************
void CItemPoint::Draw(void)
{
}

//******************************
// �Z�[�u
//******************************
void CItemPoint::SaveText(void)
{
	// �t�@�C���I�[�v��
	FILE *pFile = NULL;

	// �R�[�X�̃^�C�v�ɂ�����ĕ�����
	pFile = fopen(ITEMPOINT_TEXT, "w");

	if (pFile != NULL)
	{
		// �`�F�b�N�|�C���g���̏�������
		fprintf(pFile, "%d\n", m_nItemNum);

		// ���W�̏�������
		for (int nCnt = 0; nCnt < m_nItemNum; nCnt++)
		{
			fprintf(pFile, "%.2f,%.2f,%.2f\n", m_aItemPos[nCnt].x, m_aItemPos[nCnt].y, m_aItemPos[nCnt].z);
		}
		// �t�@�C���N���[�Y
		fclose(pFile);
	}
}

//******************************
// ���[�h
//******************************
void CItemPoint::LoadText(void)
{
	// �t�@�C���I�[�v��
	FILE *pFile = NULL;

	// �R�[�X�̃^�C�v�ɂ�����ĕ�����
	pFile = fopen(ITEMPOINT_TEXT, "r");

	if (pFile != NULL)
	{
		// �`�F�b�N�|�C���g���̓ǂݍ���
		fscanf(pFile, "%d", &m_nItemNum);

		// �`�F�b�N�|�C���g�������[�v
		for (int nCnt = 0; nCnt < m_nItemNum; nCnt++)
		{
			// ���W�̓ǂݍ���
			fscanf(pFile, "%f,%f,%f\n", &m_aItemPos[nCnt].x, &m_aItemPos[nCnt].y, &m_aItemPos[nCnt].z);

			// �A�C�e���̐���
			CItem::Create(m_aItemPos[nCnt]);
		}

		// �t�@�C���N���[�Y
		fclose(pFile);
	}
}
