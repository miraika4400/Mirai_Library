////////////////////////////////////////////////////
//
//    itembox_effect�N���X�̏���[itembox_effect.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////

//******************************
// �C���N���[�h
//******************************
#include "stan_effect.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "sound.h"

#include <time.h>

//*****************************
// �}�N����`
//*****************************
#define LIFE  60                                 // ����
#define PARTICLE_NUM 10                           // �G�t�F�N�g���o����
#define TEXTURE_PATH "./data/Textures/star01.png" // �e�N�X�`���̃p�X
#define MOVE_Y_DIST -20                           // �ړ���Y���̖ڕW�l
#define MOVE_Y_RATE 0.05f                         // �ړ���Y���̌W��
#define MOVE_Y_MIN 10                             // Y�����x�̍ŏ�
#define MOVE_Y_AMPLITUDE 20                       // Y�����x�̐U��
#define SPEED_MIN 50                              // ���x�̍ŏ�
#define SPEED_AMPLITUDE 50                        // ���x�̐U��
#define SPEED_ADJUST 10.0f                        // �X�s�[�h�����l
#define RAND_ANGLE_360 D3DXToRadian(rand() % 360) // 360�x�����_���Ȋp�x
#define SIZE_MIN 20                               // �T�C�Y�̍ŏ�
#define SIZE_AMPLITUDE 20                         // �T�C�Y�̐U��

//******************************
// �ÓI�����o�ϐ��錾
//******************************
LPDIRECT3DTEXTURE9  CStanEffect::m_pTexture = NULL; // �e�N�X�`���|�C���^

//******************************
// �R���X�g���N�^
//******************************
CStanEffect::CStanEffect() :CBillboard(OBJTYPE_PARTICLE)
{
	m_move = VEC3_ZERO;  // �ړ���
	m_nLife = 0;         // ����
}

//******************************
// �f�X�g���N�^
//******************************
CStanEffect::~CStanEffect()
{
}

//******************************
// �G�t�F�N�g�̃Z�b�g
//******************************
void CStanEffect::SetEffect(D3DXVECTOR3 pos)
{
	for (int nCnt = 0; nCnt < PARTICLE_NUM; nCnt++)
	{
		Create(pos);
	}
}

//******************************
// �N���G�C�g
//******************************
CStanEffect * CStanEffect::Create(D3DXVECTOR3 pos)
{
	// �������̊m��
	CStanEffect *pStart;
	pStart = new CStanEffect;

	// �������E�����̑��
	pStart->Init();
	pStart->SetPos(pos);

	return pStart;
}

//******************************
// �e�N�X�`���̃��[�h
//******************************
HRESULT CStanEffect::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_PATH, &m_pTexture);

	return S_OK;
}

//******************************
// �e�N�X�`���̃A�����[�h
//******************************
void CStanEffect::Unload(void)
{
	// �e�N�X�`���̉������
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}


//******************************
// ����������
//******************************
HRESULT CStanEffect::Init(void)
{
	if (FAILED(CBillboard::Init()))
	{
		return E_FAIL;
	}

	// �e�N�X�`�����蓖��
	BindTexture(m_pTexture);

	// ���Z�����ɂ���
	SetAddMode(true);

	// �ړ��ʂ������_���ɐݒ�

	// �p�x�𓖂������L������Y�������ƂɃ����_��
	float fRandAngle = RAND_ANGLE_360;
	// ���ł����x
	float fSpeed = (float)((rand() % SPEED_AMPLITUDE + SPEED_MIN)) / SPEED_ADJUST;
	// ��L�̏�񂩂�ړ��ʂ̐���*Y���͕ʂŗ�������
	m_move.x = cosf(fRandAngle)*fSpeed;
	m_move.y = (float)(rand() % MOVE_Y_AMPLITUDE + MOVE_Y_MIN);
	m_move.z = sinf(fRandAngle)*fSpeed;

	// �p�x�������_���ɏ�����
	SetAngle((float)(rand()%360));

	// �T�C�Y�������_���ɐݒ�
	float fSize = (float)(rand() % SIZE_AMPLITUDE + SIZE_MIN);
	D3DXVECTOR3 randSize;
	randSize.x = fSize;
	randSize.y = fSize;
	randSize.z = 0.0f;
	SetSize(randSize);

	return S_OK;
}

//******************************
// �I������
//******************************
void CStanEffect::Uninit(void)
{
	CBillboard::Uninit();
}

//******************************
// �X�V����
//******************************
void CStanEffect::Update(void)
{
	if (m_nLife >= 1)
	{
		SetPos(GetPos() + m_move);
		
		m_move.y += (MOVE_Y_DIST - m_move.y)*MOVE_Y_RATE;
	}
	// ����
	m_nLife++;
	if (m_nLife >= LIFE)
	{
		// ����
		Uninit();
	}
}

//******************************
// �`�揈��
//******************************
void CStanEffect::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���C�g������
	pDevice->SetRenderState(D3DRS_LIGHTING, false);

	// �`��
	CBillboard::Draw();

	// ���C�g������
	pDevice->SetRenderState(D3DRS_LIGHTING, true);
}
