////////////////////////////////////////////////////
//
//    itembox_effect�N���X�̏���[itembox_effect.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////

//******************************
// �C���N���[�h
//******************************
#include "itembox_effect.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "sound.h"

#include <time.h>

//*****************************
// �}�N����`
//*****************************
#define LIFE  50                               // ����
#define PARTICLE_NUM 30                        // �G�t�F�N�g���o����
#define TEXTURE_PATH "./data/Textures/Item_Box_Shard.png" // �e�N�X�`���̃p�X
#define MOVE_Y_DIST -5                            // �ړ���Y���̖ڕW�l
#define MOVE_Y_RATE 0.05f                         // �ړ���Y���̌W��
#define MOVE_Y_MIN 1                              // Y�����x�̍ŏ�
#define MOVE_Y_AMPLITUDE 10                       // Y�����x�̐U��
#define SPEED_MIN 100                             // ���x�̍ŏ�
#define SPEED_AMPLITUDE 30                        // ���x�̐U��
#define SPEED_ADJUST 10.0f                        // �X�s�[�h�����l
#define RAND_ANGLE_360 D3DXToRadian(rand() % 360) // 360�x�����_���Ȋp�x
#define SIZE_MIN 10                               // �T�C�Y�̍ŏ�
#define SIZE_AMPLITUDE 40                         // �T�C�Y�̐U��
#define ANGLE_AMPLITUDE 60                        // �p�x�̐U��
#define ADD_ROT_ADJUST 100.0f                     // ��]���̉��Z�̒����l

//******************************
// �ÓI�����o�ϐ��錾
//******************************
LPDIRECT3DTEXTURE9  CItemBoxEffect::m_pTexture = NULL; // �e�N�X�`���|�C���^

//******************************
// �R���X�g���N�^
//******************************
CItemBoxEffect::CItemBoxEffect() :CScene3d(OBJTYPE_PARTICLE)
{
	m_fCharaAngle = 0.0f;  // ���������L�����N�^�[�̌���
	m_move   = VEC3_ZERO;  // �ړ���
	m_nLife  = 0;          // ����
}

//******************************
// �f�X�g���N�^
//******************************
CItemBoxEffect::~CItemBoxEffect()
{
}

//******************************
// �G�t�F�N�g�̃Z�b�g
//******************************
void CItemBoxEffect::SetEffect(D3DXVECTOR3 pos, float fCharaAngle)
{
	for (int nCnt = 0; nCnt < PARTICLE_NUM; nCnt++)
	{
		Create(pos, fCharaAngle);
	}
}

//******************************
// �N���G�C�g
//******************************
CItemBoxEffect * CItemBoxEffect::Create(D3DXVECTOR3 pos, float fCharaAngle)
{
	// �������̊m��
	CItemBoxEffect *pStart;
	pStart = new CItemBoxEffect;

	// �������E�����̑��
	pStart->m_fCharaAngle = fCharaAngle;
	pStart->Init();
	pStart->SetPos(pos);

	return pStart;
}

//******************************
// �e�N�X�`���̃��[�h
//******************************
HRESULT CItemBoxEffect::Load(void)
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
void CItemBoxEffect::Unload(void)
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
HRESULT CItemBoxEffect::Init(void)
{
	if (FAILED(CScene3d::Init()))
	{
		return E_FAIL;
	}

	// �e�N�X�`�����蓖��
	BindTexture(m_pTexture);

	// ���Z�����ɂ���
	SetAddMode(true);

	// �ړ��ʂ������_���ɐݒ�
	// �p�x�𓖂������L������Y�������ƂɃ����_��
	float fRandAngle = m_fCharaAngle + D3DXToRadian(rand() % ANGLE_AMPLITUDE - ANGLE_AMPLITUDE/2);
	// ���ł����x
	float fSpeed = (float)((rand() % SPEED_AMPLITUDE + SPEED_MIN)) / SPEED_ADJUST;
	// ��L�̏�񂩂�ړ��ʂ̐���*Y���͕ʂŗ�������
	m_move.x = cosf(fRandAngle)*fSpeed;
	m_move.y = (float)(rand() % MOVE_Y_AMPLITUDE + MOVE_Y_MIN);
	m_move.z = sinf(fRandAngle)*fSpeed;
	
	// �p�x�������_���ɏ�����
	D3DXVECTOR3 randRot;
	randRot.x = RAND_ANGLE_360;
	randRot.y = RAND_ANGLE_360;
	randRot.z = RAND_ANGLE_360;
	SetRot(randRot);
	
	// �T�C�Y�������_���ɐݒ�
	D3DXVECTOR3 randSize;
	randSize.x = (float)(rand() % SIZE_AMPLITUDE + SIZE_MIN);
	randSize.y = (float)(rand() % SIZE_AMPLITUDE + SIZE_MIN);
	randSize.z = 0.0f;
	SetSize(randSize);

	return S_OK;
}

//******************************
// �I������
//******************************
void CItemBoxEffect::Uninit(void)
{
	CScene3d::Uninit();
}

//******************************
// �X�V����
//******************************
void CItemBoxEffect::Update(void)
{
	if (m_nLife >= 1)
	{
		SetPos(GetPos() + m_move);
		SetRot(GetRot() + m_move / ADD_ROT_ADJUST);

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
void CItemBoxEffect::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	
	// ���C�g������
	pDevice->SetRenderState(D3DRS_LIGHTING, false);
	// �J�����O
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// �`��
	CScene3d::Draw();

	// ���C�g������
	pDevice->SetRenderState(D3DRS_LIGHTING, true);
	// �J�����O
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
