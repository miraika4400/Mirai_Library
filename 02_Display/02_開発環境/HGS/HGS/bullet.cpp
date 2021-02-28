//=============================================================================
//
// �e�̏��� [bullet.cpp]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "bullet.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "score.h"
#include "sound.h"
#include "game.h"
#include "enemy.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define PLAYER_BULLET_ANIME_SPEED	(2)				// �A�j���[�V�����̑��x
#define PLAYER_BULLET_ANIME_COUNTER	(4)				// �A�j���[�V�����̐�
#define CURRENT_BULLET_SPEED_X		(5.0f)			// �Ǐ]���̒e�̑��x
#define COUNTER_BULLET_SPEED_Y		(5.0f)			// �Ǐ]���̒e�̑��x
#define BULLET_DAMAGE				(100)			// �e�̈З�

//=============================================================================
// static����������
//=============================================================================
LPDIRECT3DTEXTURE9 CBullet::m_apTexture[MAX_BULLET_TEXTURE] = {};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBullet::CBullet() :CScene2d(OBJTYPE_BULLET)
{
	m_nLife = 0;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBullet::~CBullet()
{
}

//=============================================================================
// �C���X�^���X����
//=============================================================================
CBullet * CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, 
	D3DXVECTOR3 size,	int nLife)
{
	// �C���X�^���X����
	CBullet *pBullet = new CBullet;

	// ����������
	pBullet->Init();			// ���������
	pBullet->SetPos(pos);		// ���W
	pBullet->SetSize(size);		// �ړ�
	pBullet->m_move = move;		// �ړ���
	pBullet->m_nLife = nLife;	// �̗�
	pBullet->BindTexture(m_apTexture[0]);
	return pBullet;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CBullet::Init(void)
{
	// ����������
	CScene2d::Init();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CBullet::Uninit(void)
{
	// 2D�|���S���I������
	CScene2d::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CBullet::Update(void)
{
	// ���W�󂯎��
	D3DXVECTOR3 pos = GetPos();

	// 2D�|���S���X�V����
	CScene2d::Update();

	// �ړ��̍X�V
	pos += m_move;

	// �򋗗��̃J�E���^�[���Z
	m_nLife--;

	// ���W��n��
	SetPos(pos);

	// �����蔻��
	Collision();

	// ���C�t��0�ɂȂ�����
	if (m_nLife <= 0)
	{
		// �I������
		Uninit();
		return;
	}

	// ��ʊO����
	if (pos.x  < 0.0f || pos.x  > SCREEN_WIDTH ||
		pos.y  < 0.0f || pos.y  > SCREEN_HEIGHT)
	{
		// �̗͂�0�ɂ���
		m_nLife = 0;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CBullet::Draw(void)
{
	// 2D�|���S���`�揈��
	CScene2d::Draw();
}

//=============================================================================
// �e�N�X�`�����[�h
//=============================================================================
HRESULT CBullet::Load(void)
{
	// �����_���[�̏����󂯎��
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/Textures/Bullet.png",
		&m_apTexture[0]);

	return S_OK;
}

//=============================================================================
// �e�N�X�`���A�����[�h
//=============================================================================
void CBullet::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_BULLET_TEXTURE; nCount++)
	{
		// �e�N�X�`���̊J��
		if (m_apTexture[nCount] != NULL)
		{
			m_apTexture[nCount]->Release();
			m_apTexture[nCount] = NULL;
		}
	}
}

//=============================================================================
// �����蔻��
//=============================================================================
bool CBullet::Collision(void)
{
	// �擪�����擾
	CScene *pScene = CScene::GetTop(OBJTYPE_ENEMY);

	// NULL�ɂȂ�܂ŌJ��Ԃ�
	while (pScene)
	{
		// ���W�擾
		D3DXVECTOR3 pos = GetPos();

		// �L���X�g
		CEnemy *pEnemy = (CEnemy*)pScene;

		if (pEnemy != NULL)
		{
			//�ʒu�̎擾
			D3DXVECTOR3 targetPos = pEnemy->GetPos();

			// �����蔻��
			if (
				targetPos.x - ENEMY_SIZE_X / 2 <pos.x +BULLET_SIZE_X &&
				targetPos.x + ENEMY_SIZE_X / 2 >pos.x -BULLET_SIZE_X &&
				targetPos.y - ENEMY_SIZE_Y / 2 <pos.y +BULLET_SIZE_Y &&
				targetPos.y + ENEMY_SIZE_Y / 2 >pos.y -BULLET_SIZE_Y)
			{
				// �e������
				m_nLife = 0;

				// �G�Ƀ_���[�W
				pEnemy->HitDamage(BULLET_DAMAGE);

				// �X�R�A�̉��Z
				CScore::AddScore(ENEMY_SCORE_NORMAL);
				return true;
			}
		}

		//���̏����擾
		pScene = pScene->GetNext();
	}

	return false;
}