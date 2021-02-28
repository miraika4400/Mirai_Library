////////////////////////////////////////////////////
//
//    bomb�N���X�̏���[bomb.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////

//******************************
// �C���N���[�h
//******************************
#include "break_effect.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "score.h"
#include "game.h"
#include "particle.h"
#include "score.h"
#include "game.h"
#include "player.h"

//*****************************
// �}�N����`
//*****************************
#define MAX_SIZE 1500.0f  // �ő�T�C�Y
#define RATE_SIZE 0.03f   // �T�C�Y�W��
#define RATE_ALFA 0.05f    // �A���t�@�l�W��

//******************************
// �ÓI�����o�ϐ��錾
//******************************
LPDIRECT3DTEXTURE9 CBreakEffect::m_pTexture = NULL; // �e�N�X�`���|�C���^

//******************************
// �R���X�g���N�^
//******************************
CBreakEffect::CBreakEffect() :CScene(OBJTYPE_PARTICLE)
{

	m_fMaxsize = 0.0f;
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fAlfa = 1.0f;
}

//******************************
// �f�X�g���N�^
//******************************
CBreakEffect::~CBreakEffect()
{
}

//******************************
// �N���G�C�g
//******************************
CBreakEffect * CBreakEffect::Create(const D3DXVECTOR3 pos)
{
	// �������̊m��
	CBreakEffect *pBreak;
	pBreak = new CBreakEffect;
	// ������
	pBreak->Init();

	// �e�l�̑���E�Z�b�g
	pBreak->m_pos = pos;                     // ���W
	pBreak->SetPriority(OBJTYPE_PARTICLE);   // �I�u�W�F�N�g�^�C�v

	return pBreak;
}



//******************************
// ����������
//******************************
HRESULT CBreakEffect::Init(void)
{
	// �T�C�Y�̏�����
	m_size = D3DXVECTOR3(5.0f, 5.0f, 0.0f);
	m_fMaxsize = MAX_SIZE;
	m_fAlfa = 1.0f;
	return S_OK;
}

//******************************
// �I������
//******************************
void CBreakEffect::Uninit(void)
{

	Release();
}

//******************************
// �X�V����
//******************************
void CBreakEffect::Update(void)
{

	int nRnd = rand() % 10 + 15;
	for (int nCntparticle = 0; nCntparticle < nRnd; nCntparticle++)
	{
		// �����_���ȋ���
		float fRandDistance = (float)(rand() % 500 - 250) / 100;
		// �����_���Ȋp�x
		int nRandAngle = rand() % 360;

		// �����_���ȍ��W
		D3DXVECTOR3 randPos;
		randPos.x = m_pos.x + cosf(D3DXToRadian(nRandAngle))*(m_size.x / 2 + fRandDistance);
		randPos.y = m_pos.y + sinf(D3DXToRadian(nRandAngle))*(m_size.y / 2 + fRandDistance);
		randPos.z = 0.0f;


		// �����_���ȃT�C�Y
		int nRandSize = rand() % 7 + 20;
		// �p�[�e�B�N���̐���
		CParticle::Create(randPos,
			D3DXVECTOR3((float)(rand() % 3 - 1),
			(float)(rand() % 3 - 1), 0.0f),
			D3DXVECTOR3((float)nRandSize, (float)nRandSize, 0.0f),
			rand() % 5 + 1,
			D3DXCOLOR(1.0f,(float)(rand() % 100) / 100.0f, 0.5f,  m_fAlfa))->SetAddMode(false);
	}

	nRnd = rand() % 10 + 20;
	for (int nCntparticle = 0; nCntparticle < nRnd; nCntparticle++)
	{
		// �����_���ȋ���
		float fRandDistance = (float)(rand() % 500 - 250) / 100;
		// �����_���Ȋp�x
		int nRandAngle = rand() % 360;

		// �����_���ȍ��W
		D3DXVECTOR3 randPos;
		randPos.x = m_pos.x + cosf(D3DXToRadian(nRandAngle))*( m_fMaxsize/4 - m_size.x/4  + fRandDistance);
		randPos.y = m_pos.y + sinf(D3DXToRadian(nRandAngle))*( m_fMaxsize/4 - m_size.y/4  + fRandDistance);
		randPos.z = 0.0f;

		// �����_���ȃT�C�Y
		int nRandSize = rand() % 7 + 15;
		// �p�[�e�B�N���̐���
		CParticle::Create(randPos,
			D3DXVECTOR3((float)(rand() % 3 - 1),(float)(rand() % 3 - 1), 0.0f),
			D3DXVECTOR3((float)nRandSize, (float)nRandSize, 0.0f),
			rand() % 5 + 1,
			D3DXCOLOR(1.0f, (float)(rand() % 100) / 100.0f, 0.5f, m_fAlfa))->SetAddMode(false);
	}

	


	m_fAlfa += ((0.0f) - m_fAlfa)*RATE_ALFA;

	m_size.x += ((m_fMaxsize + 3.0f) - m_size.x)*RATE_SIZE;
	m_size.y += ((m_fMaxsize + 3.0f) - m_size.y)*RATE_SIZE;

	// �ő�T�C�Y�ɓ��B���������
	if (m_size.x > m_fMaxsize)
	{
		Uninit();
		return;
	}

}

//******************************
// �`�揈��
//******************************
void CBreakEffect::Draw(void)
{

}

