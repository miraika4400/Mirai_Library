//=============================================================================
//
// �G�l�~�[�̏����@[enemy.cpp]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "enemy.h"
#include "renderer.h"
#include "manager.h"
#include "bullet.h"
#include "player.h"
#include "score.h"
#include "game.h"
#include "sound.h"
#include "grid.h"
#include "break_effect.h"
#include "collision.h"
#include "tutorial.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define ENEMY_NORMAL_LIFE			(4)					// �ʏ�̃G�l�~�[�̃��C�t
#define ENEMY_SCORE					(200)				// �G�l�~�[�̃X�R�A
#define ENEMY_SPEED					(2.0f)				// �G�l�~�[�̑���
#define ENEMY_LIFE					(50)				// �G�l�~�[�̗̑�
#define ENEMY_DAMAGE				(200)				// �G�l�~�[�̍U����
#define ENEMY_SPEED_UP				(3)					// �G�l�~�[�̃X�s�[�h�A�b�v
#define ENEMY_SPEED_DOWN			(1)					// �G�l�~�[�̃X�s�[�h�_�E��

//=============================================================================
// static������
//=============================================================================
LPDIRECT3DTEXTURE9 CEnemy::m_apTexture[MAX_ENEMY_TEXTURE] = {};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CEnemy::CEnemy():CScene2d(OBJTYPE_ENEMY)
{
	m_nLife = 0;
	m_nSpeed = 0;
	m_fRasian = 0.0f;
	m_fDiagonal = 0.0f;
	m_fAngle = 0.0f;

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEnemy::~CEnemy()
{

}

//=============================================================================
// �C���X�^���X����
//=============================================================================
CEnemy * CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// ����������
	CEnemy *pEnemy = new CEnemy;
	if (pEnemy != NULL)
	{
		// ����������
		pEnemy->Init();							// ���������
		pEnemy->SetPos(pos);
		pEnemy->SetSize(size);					// �T�C�Y�ݒ�
		pEnemy->m_nLife = ENEMY_LIFE;			// �̗�
		pEnemy->BindTexture(m_apTexture[0]);	// �e�N�X�`���ݒ�
		pEnemy->InitAnimation(5, 7, -1);
	}

	return pEnemy;
}

//=============================================================================
// �e�N�X�`�����[�h
//=============================================================================
HRESULT CEnemy::Load(void)
{
	// �����_���[�̏����󂯎��
	CRenderer *pRenderer = NULL;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/Textures/Enemy_Animation.png",
		&m_apTexture[0]);

	return S_OK;
}

//=============================================================================
// �e�N�X�`���A�����[�h
//=============================================================================
void CEnemy::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_ENEMY_TEXTURE; nCount++)
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
// ����������
//=============================================================================
HRESULT CEnemy::Init()
{
	// ����������
	CScene2d::Init();

	CGame *pGame = CManager::GetGame();

	if (pGame != NULL)
	{
		if (pGame->GetSpeedUp() == true)
		{
			m_nSpeed = ENEMY_SPEED + rand() % ENEMY_SPEED_UP - rand() % ENEMY_SPEED_DOWN;
		}
		else
		{
			m_nSpeed = ENEMY_SPEED;
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CEnemy::Uninit(void)
{
	// 2D�|���S���I������
	CScene2d::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CEnemy::Update(void)
{
	// 2D�|���S���X�V����
	CScene2d::Update();

	// ���W�X�V
	D3DXVECTOR3 pos = GetPos();

	// ���݂̃��[�h�󂯎��
	CManager::MODE mode = CManager::GetMode();

	// �ϐ��錾
	CPlayer *pPlayer = NULL;

	if(CPlayer::GetAlive())
	{
		switch (mode)
		{
		case CManager::MODE_GAME:
			// �v���C���[�����擾
			pPlayer = CGame::GetPlayer();
			break;

		case CManager::MODE_TUTORIAL:
			// �v���C���[�����擾
			pPlayer = CTutorial::GetPlayer();
			break;

		}
	}
	if (mode == CManager::MODE_GAME)
	{
		if (pPlayer != NULL)
		{

			D3DXVECTOR3 Ppos = pPlayer->GetPos();

			//���@���擾����
			float fPposx = Ppos.x, fPposy = Ppos.y;		// ���@�̍��W
			float fEposx = pos.x, fEposy = pos.y;	// �G�̍��W
			float fAngle;								// �p�x

														//�p�x�����߂�
			fAngle = atan2f((fEposx - fPposx), (fEposy - fPposy));
			// �����ȓG�̈ړ�
			D3DXVECTOR3 FollowMove = D3DXVECTOR3(
				(sinf(fAngle)*-ENEMY_SPEED),
				cosf(fAngle)*-ENEMY_SPEED, 0.0f);

			// �ړ��ʂ̐ݒ�
			pos += FollowMove;

			// ���W��n��
			SetPos(pos);

			// ��]
			Rotation();

			if (CollisionCircularAndCircular(pos, Ppos, ENEMY_SIZE_X, DECISION_PLAYER_SIZE_X) == true)
			{
				// �G�Ƀ_���[�W
				pPlayer->HitDamage(ENEMY_DAMAGE);
			}
		}
	}
	// ���C�t��0��������
	if (m_nLife <= 0)
	{
		// �G�l�~�[�폜
		Uninit();
		CGrid::Break(GetPos());
		CBreakEffect::Create(GetPos());
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CEnemy::Draw(void)
{
	// 2D�|���S���`�揈��
	CScene2d::Draw();
}

//=============================================================================
// �G�l�~�[�q�b�g��
//=============================================================================
void CEnemy::HitDamage(int nCount)
{
	m_nLife -= nCount;
}

//=============================================================================
// �����蔻��
//=============================================================================
bool CEnemy::Collision(void)
{
	// �擪�����擾
	CScene *pScene = CScene::GetTop(OBJTYPE_PLAYER);

	// NULL�ɂȂ�܂ŌJ��Ԃ�
	while (pScene)
	{
		// ���W�擾
		D3DXVECTOR3 pos = GetPos();

		// �L���X�g
		CPlayer *pPlayer = (CPlayer*)pScene;

		if (pPlayer != NULL)
		{
			//�ʒu�̎擾
			D3DXVECTOR3 targetPos = pPlayer->GetPos();

			// �����蔻��
			if (
				targetPos.x - ENEMY_SIZE_X / 2 <pos.x + BULLET_SIZE_X &&
				targetPos.x + ENEMY_SIZE_X / 2 >pos.x - BULLET_SIZE_X &&
				targetPos.y - ENEMY_SIZE_Y / 2 <pos.y + BULLET_SIZE_Y &&
				targetPos.y + ENEMY_SIZE_Y / 2 >pos.y - BULLET_SIZE_Y)
			{
				// �e������
		//		m_nLife = 0;

				// �G�Ƀ_���[�W
				pPlayer->HitDamage(ENEMY_DAMAGE);
				return true;
			}
		}

		//���̏����擾
		pScene = pScene->GetNext();
	}

	return false;
}

void CEnemy::Rotation(void)
{
	// ���W�X�V
	D3DXVECTOR3 pos = GetPos();

	// ���݂̃��[�h�󂯎��
	CManager::MODE mode = CManager::GetMode();

	// �ϐ��錾
	CPlayer *pPlayer = NULL;

	switch (mode)
	{
	case CManager::MODE_GAME:
		// �v���C���[�����擾
		pPlayer = CGame::GetPlayer();
		break;

	case CManager::MODE_TUTORIAL:
		// �v���C���[�����擾
		pPlayer = CTutorial::GetPlayer();
		break;

	}
	if (pPlayer != NULL)
	{

		D3DXVECTOR3 Ppos = pPlayer->GetPos();

		//���@���擾����
		float fPposx = Ppos.x, fPposy = Ppos.y;		// ���@�̍��W
		float fEposx = pos.x, fEposy = pos.y;	// �G�̍��W
		float fAngle;								// �p�x

		//�p�x�����߂�
		fAngle = atan2f((fEposx - fPposx), (fEposy - fPposy));

		// �A���O���̐ݒ�
		SetAngle(D3DXToDegree(-fAngle));
		SetPos(pos);
	}
}