////////////////////////////////////////////////////
//
//    particle�N���X�̏���[particle.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////

//******************************
// �C���N���[�h
//******************************
#include "particle.h"
#include "manager.h"
#include "renderer.h"

//*****************************
// �}�N����`
//*****************************
#define PARTICLE_BOX_TEXTURE_PATH "./data/Textures/particle001.png"    //�e�N�X�`���̃p�X
#define PARTICLE_STAR_TEXTURE_PATH "./data/Textures/particle000.png"   //�e�N�X�`���̃p�X
#define PARTICLE_CIRCLE_TEXTURE_PATH "./data/Textures/particle004.png" //�e�N�X�`���̃p�X

//******************************
// �ÓI�����o�ϐ��錾
//******************************
LPDIRECT3DTEXTURE9  CParticle::m_apTexture[PARTICLE_MAX] = {}; // �e�N�X�`���|�C���^

//******************************
// �R���X�g���N�^
//******************************
CParticle::CParticle()
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nLife = 0;
	m_type = PARTICLE_BOX;
}

//******************************
// �f�X�g���N�^
//******************************
CParticle::~CParticle()
{
}

//******************************
// �N���G�C�g
//******************************
CParticle * CParticle::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const D3DXVECTOR3 size, const int nLife, const D3DCOLOR col,const PARTICLE_TYPE type)
{
	// �������̊m��
	CParticle *pParticle;
	pParticle = new CParticle;

	// �^�C�v�̐ݒ�
	pParticle->m_type = type;

	// ������
	pParticle->Init();

	// �e�l�̑���E�Z�b�g
	pParticle->SetPos(pos);                  // ���W
	pParticle->m_move = move;                // �ړ���
	pParticle->SetSize(size);                // �T�C�Y
	pParticle->m_nLife = nLife;              // ����
	pParticle->SetColor(col);                // �J���[
	pParticle->SetPriority(OBJTYPE_PARTICLE); // �I�u�W�F�N�g�^�C�v
	pParticle->SetAngle((float)(rand() % 360));       // ��]�p�x�������_��
	pParticle->SetAddMode(true);             // ���Z����
	return pParticle;
}

//******************************
// �e�N�X�`���̃��[�h
//******************************
HRESULT CParticle::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, PARTICLE_BOX_TEXTURE_PATH, &m_apTexture[PARTICLE_BOX]);
	D3DXCreateTextureFromFile(pDevice, PARTICLE_STAR_TEXTURE_PATH, &m_apTexture[PARTICLE_STAR]);
	D3DXCreateTextureFromFile(pDevice, PARTICLE_CIRCLE_TEXTURE_PATH, &m_apTexture[PARTICLE_CIRCLE]);
	return S_OK;
}

//******************************
// �e�N�X�`���̃A�����[�h
//******************************
void CParticle::Unload(void)
{
	for (int nCntParticle = 0; nCntParticle < PARTICLE_MAX; nCntParticle++)
	{
		// �e�N�X�`���̉������
		if (m_apTexture[nCntParticle] != NULL)
		{
			m_apTexture[nCntParticle]->Release();
			m_apTexture[nCntParticle] = NULL;
		}
	}
}


//******************************
// ����������
//******************************
HRESULT CParticle::Init(void)
{
	if (FAILED(CScene2d::Init()))
	{
		return E_FAIL;
	}

	// �e�N�X�`�����蓖��
	BindTexture(m_apTexture[m_type]);

	return S_OK;
}

//******************************
// �I������
//******************************
void CParticle::Uninit(void)
{

	CScene2d::Uninit();
}

//******************************
// �X�V����
//******************************
void CParticle::Update(void)
{
	// �ړ�
	SetPos(GetPos() + m_move);
	D3DXCOLOR col = GetColor();
	
	col.a -= 0.02f;
	SetColor(col);

	// ����
	m_nLife--;
	if (m_nLife <= 0)
	{
		Uninit();
	}
}

//******************************
// �`�揈��
//******************************
void CParticle::Draw(void)
{
	CScene2d::Draw();
}
