////////////////////////////////////////////////////
//
//    player�̏���[player.cpp]
//    Author:���V ����
//
////////////////////////////////////////////////////


//=============================
// �C���N���[�h
//=============================
#include "grid.h"
#include "manager.h"
#include "renderer.h"
#include "scene2d.h"
#include "fade.h"
#include "diplay_off.h"
#include "sound.h"

//**********************************
// �}�N����`
//**********************************
#define TEXTURE_PATH "./data/Textures/Scene Animation.png" // �e�N�X�`��
#define TEXTURE_PATH2 "./data/Textures/Screen Crash.png"  // �e�N�X�`��
#define TEXTURE_PATH3 "./data/Textures/sand_screen2.png" // �e�N�X�`��
#define TEXTURE_PATH4 "./data/Textures/Black Out.png" // �e�N�X�`��
#define TEXTURE_PATH5 "./data/Textures/Screen Crash2.png" // �e�N�X�`��

#define GRID_SIZE D3DXVECTOR3(16.0f*4.0f,9.0f*4.0f,0.0f) // �O���b�h�̃T�C�Y
#define SHAKE_COUNT 5
#define ANIM_SPEED 4              // �A�j���[�V�������x
#define MAX_ANIMATION_X 18        // �A�j���[�V������ ��
#define MAX_ANIMATION_Y 1         // �A�j���[�V������ �c
#define END_AREA 1250             // allBreakArea���̒l�ȏ�Ȃ烊�U���g�ɍs��
#define END_AREA_ADD 7.0f         // allBreakArea���̒l�ȏ�Ȃ烊�U���g�ɍs��

//**********************************
// �ÓI�����o�ϐ��錾
//**********************************
LPDIRECT3DTEXTURE9 CGrid::m_apTexture[STATE_MAX] = {};                     // �e�N�X�`��
LPDIRECT3DTEXTURE9 CGrid::m_apTexCrash[CRASH_TYPE_NUM]{};      // �e�N�X�`��
CGrid *CGrid::m_apGrid[GRID_NUM_Y][GRID_NUM_X] = {};             // �O���b�h
D3DXVECTOR3 CGrid::m_shake = D3DXVECTOR3(0.0f,0.0f,0.0f);        // �u��
D3DXVECTOR3 CGrid::m_shakeDist = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // �u���ڕW�l
bool CGrid::m_bShake = false;                                    // �u���t���O
int CGrid::m_nCntShake = 0;                                      // �u���J�E���g
float CGrid::m_fBreakArea = 0.0f;                                // BreakAll�ŏ����͈�
bool CGrid::m_bAllBreak = false;                                         // ���ׂĉ󂷃t���O
D3DXVECTOR3 CGrid::m_breakPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);;                                   // ���ׂĉ󂷎n�_

//=============================
// �R���X�g���N�^
//=============================
CGrid::CGrid() : CScene2d(OBJTYPE_MAP)
{
	///m_pScene2d = NULL;        // �|���S��
	m_gridNum = D3DXVECTOR2(0.0f, 0.0f); // �O���b�h�ԍ�
	m_nCntAnim = 0;       // �A�j���[�V�����J�E���g
	m_nAnimX = 0;         // �A�j���[�V����X��
	m_nAnimY = 0;         // �A�j���[�V����Y��
	m_pScene2d = NULL;
	m_state = STATE_NORMAL;
	m_bAllBreak = false;
	m_fBreakArea = 0.0f;
	m_bAnim = true;
}

//=============================
// �f�X�g���N�^
//=============================
CGrid::~CGrid()
{
}

//=============================
// �N���G�C�g
//=============================
void CGrid::CreateAll(void)
{
	float fPosY = CENTER_POS.y - (((GRID_SIZE.y*2)*GRID_NUM_Y) / 2) + GRID_SIZE.y;// X���ʒu
	float fPosX = CENTER_POS.x - (((GRID_SIZE.x*2)*GRID_NUM_X) / 2) + GRID_SIZE.x;// Y���ʒu
	
	// ���������[�v
	for (int nCntY = 0; nCntY < GRID_NUM_Y; nCntY++)
	{
		// �c�������[�v
		for (int nCntX = 0; nCntX < GRID_NUM_X; nCntX++)
		{
			// �������̊m��
			//CGrid*pGrid = NULL;
			m_apGrid[nCntY][nCntX] = new CGrid;

			// NULL�`�F�b�N
			if (m_apGrid[nCntY][nCntX] != NULL)
			{
				
				// ������
				m_apGrid[nCntY][nCntX]->Init();
				// �ʒu�̐ݒ�
				m_apGrid[nCntY][nCntX]->SetPos(D3DXVECTOR3(fPosX, fPosY, 0.0f));
				m_apGrid[nCntY][nCntX]->SetSize(GRID_SIZE);
				// �e�N�X�`���̐ݒ�
				m_apGrid[nCntY][nCntX]->BindTexture(m_apTexture[STATE_NORMAL]);
				// �O���b�h�ԍ��̐ݒ�
				m_apGrid[nCntY][nCntX]->m_gridNum.y = (float)nCntY;
				m_apGrid[nCntY][nCntX]->m_gridNum.x = (float)nCntX;
			}
			else
			{
				return;
			}
			// Y�������炷
			fPosX += GRID_SIZE.x*2;
		}
		// Y���ʒu�̏�����
		fPosX = CENTER_POS.x - (((GRID_SIZE.x * 2)*GRID_NUM_X) / 2) + GRID_SIZE.x;
		// X�������炷
		fPosY += GRID_SIZE.y*2;
	}
	m_bAllBreak = false;
	m_fBreakArea = 0.0f;

}

//=======================================================================================
// �e�N�X�`���̃��[�h
//=======================================================================================
HRESULT CGrid::Load(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_PATH , &m_apTexture[STATE_NORMAL]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_PATH2, &m_apTexture[STATE_CRACK]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_PATH3, &m_apTexture[STATE_BREAK]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_PATH4, &m_apTexture[STATE_END]);

	D3DXCreateTextureFromFile(pDevice, TEXTURE_PATH2, &m_apTexCrash[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_PATH5, &m_apTexCrash[1]);
	return S_OK;
}

//=======================================================================================
// �e�N�X�`���̃A�����[�h
//=======================================================================================
void CGrid::Unload(void)
{
	for (int nCnt = 0; nCnt < STATE_MAX; nCnt++)
	{
		if (m_apTexture[nCnt] != NULL)
		{
			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = NULL;
		}
	}
}

//=======================================================================================
// �j��
//=======================================================================================
void CGrid::Break(D3DXVECTOR3 pos)
{
	// ���������[�v
	for (int nCntY = 0; nCntY < GRID_NUM_Y; nCntY++)
	{
		// �c�������[�v
		for (int nCntX = 0; nCntX < GRID_NUM_X; nCntX++)
		{
			// NULL�`�F�b�N
			if (m_apGrid[nCntY][nCntX] != NULL)
			{
				// �O���b�h�̍��W�̎擾
				D3DXVECTOR3 gridPos = m_apGrid[nCntY][nCntX]->GetPos();
				// �O���b�h�̃T�C�Y�̎擾
				D3DXVECTOR3 gridSize = m_apGrid[nCntY][nCntX]->GetSize();
				if (pos.x > gridPos.x - gridSize.x && pos.x < gridPos.x + gridSize.x  &&
					pos.y > gridPos.y - gridSize.y && pos.y < gridPos.y + gridSize.y)
				{
					if (m_apGrid[nCntY][nCntX]->m_state == STATE_NORMAL)
					{
						// ��Ԃ��Ђъ���ɂ���
						m_apGrid[nCntY][nCntX]->SetState(STATE_CRACK);
					}
					else if(m_apGrid[nCntY][nCntX]->m_state == STATE_CRACK)
					{
						// ����Ԃɂ���
						m_apGrid[nCntY][nCntX]->SetState(STATE_BREAK);
					}
					
					Shake(true);
					break;
				}
			}
		}
	}
}

//=======================================================================================
// �h�炷
//=======================================================================================
void CGrid::Shake(bool bRand)
{
	if (bRand)
	{// ������true�������Ƃ�
		//if (!m_bShake)
		{
			// �ǂꂭ�炢�Ԃ�邩�����Ō��߂�
			m_shakeDist.x = -0.6f;
			m_shakeDist.y = 0.6f;
			m_shakeDist.z = 0.0f;
			m_bShake = true;
			m_nCntShake = 0;

			float fPosY = CENTER_POS.y - (((GRID_SIZE.y * 2)*GRID_NUM_Y) / 2) + GRID_SIZE.y;// X���ʒu
			float fPosX = CENTER_POS.x - (((GRID_SIZE.x * 2)*GRID_NUM_X) / 2) + GRID_SIZE.x;// Y���ʒu

			// ���������[�v
			for (int nCntY = 0; nCntY < GRID_NUM_Y; nCntY++)
			{
				// �c�������[�v
				for (int nCntX = 0; nCntX < GRID_NUM_X; nCntX++)
				{
					// NULL�`�F�b�N
					if (m_apGrid[nCntY][nCntX] != NULL)
					{
						// �ʒu�̐ݒ�
						m_apGrid[nCntY][nCntX]->SetPos(D3DXVECTOR3(fPosX, fPosY, 0.0f));
						if (m_apGrid[nCntY][nCntX]->m_pScene2d != NULL)
						{
							m_apGrid[nCntY][nCntX]->m_pScene2d->SetPos(m_apGrid[nCntY][nCntX]->GetPos() + m_shakeDist);
						}

					}
					// Y�������炷
					fPosX += GRID_SIZE.x * 2;
				}
				// Y���ʒu�̏�����
				fPosX = CENTER_POS.x - (((GRID_SIZE.x * 2)*GRID_NUM_X) / 2) + GRID_SIZE.x;
				// X�������炷
				fPosY += GRID_SIZE.y * 2;
			}
		}
	}
	else
	{
		// �Ԃ炷�t���O
		if (m_bShake)
		{
			// �J�E���g��i�߂�
			m_nCntShake++;
			// ���̃J�E���g��
			if (m_nCntShake % SHAKE_COUNT == 0)
			{// ���Ε����ɂԂ炵������Ƃ��Ԃ��������������
				m_shakeDist *= -0.9f;
			}
			if (m_nCntShake > SHAKE_COUNT * 5)
			{
				m_bShake = false;

				float fPosY = CENTER_POS.y - (((GRID_SIZE.y * 2)*GRID_NUM_Y) / 2) + GRID_SIZE.y;// X���ʒu
				float fPosX = CENTER_POS.x - (((GRID_SIZE.x * 2)*GRID_NUM_X) / 2) + GRID_SIZE.x;// Y���ʒu

				// ���������[�v
				for (int nCntY = 0; nCntY < GRID_NUM_Y; nCntY++)
				{
					// �c�������[�v
					for (int nCntX = 0; nCntX < GRID_NUM_X; nCntX++)
					{
						// NULL�`�F�b�N
						if (m_apGrid[nCntY][nCntX] != NULL)
						{
							// �ʒu�̐ݒ�
							m_apGrid[nCntY][nCntX]->SetPos(D3DXVECTOR3(fPosX, fPosY, 0.0f));
							if (m_apGrid[nCntY][nCntX]->m_pScene2d != NULL)
							{
								m_apGrid[nCntY][nCntX]->m_pScene2d->SetPos(m_apGrid[nCntY][nCntX]->GetPos() + m_shakeDist);
							}

						}
						// Y�������炷
						fPosX += GRID_SIZE.x * 2;
					}
					// Y���ʒu�̏�����
					fPosX = CENTER_POS.x - (((GRID_SIZE.x * 2)*GRID_NUM_X) / 2) + GRID_SIZE.x;
					// X�������炷
					fPosY += GRID_SIZE.y * 2;
				}
			}

			// ���������[�v
			for (int nCntY = 0; nCntY < GRID_NUM_Y; nCntY++)
			{
				// �c�������[�v
				for (int nCntX = 0; nCntX < GRID_NUM_X; nCntX++)
				{
					m_apGrid[nCntY][nCntX]->SetPos( m_apGrid[nCntY][nCntX]->GetPos()+ m_shakeDist);
					if (m_apGrid[nCntY][nCntX]->m_pScene2d != NULL)
					{
						m_apGrid[nCntY][nCntX]->m_pScene2d->SetPos(m_apGrid[nCntY][nCntX]->GetPos() + m_shakeDist);
					}
				}
			}
		}
		else
		{
			m_nCntShake = 0;
			m_shakeDist = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}

		m_shake += (m_shakeDist - m_shake)*0.05f;

	}

	if (m_bAllBreak)
	{
		m_fBreakArea+= END_AREA_ADD;

		// ���������[�v
		for (int nCntY = 0; nCntY < GRID_NUM_Y; nCntY++)
		{
			// �c�������[�v
			for (int nCntX = 0; nCntX < GRID_NUM_X; nCntX++)
			{
				// NULL�`�F�b�N
				if (m_apGrid[nCntY][nCntX] != NULL)
				{
					float fDistance = sqrtf(powf(m_breakPos.x - m_apGrid[nCntY][nCntX]->GetPos().x, 2) + powf(m_breakPos.y - m_apGrid[nCntY][nCntX]->GetPos().y, 2));

					if (fDistance <= m_fBreakArea)
					{
						if (m_apGrid[nCntY][nCntX]->m_state == STATE_NORMAL)
						{
							m_apGrid[nCntY][nCntX]->SetState(STATE_CRACK);
						}
						if (m_apGrid[nCntY][nCntX]->m_state == STATE_CRACK)
						{
							m_apGrid[nCntY][nCntX]->SetState(STATE_BREAK);
						}
					}
				}
			}
		}

		if (m_fBreakArea >= END_AREA)
		{

			// ���������[�v
			for (int nCntY = 0; nCntY < GRID_NUM_Y; nCntY++)
			{
				// �c�������[�v
				for (int nCntX = 0; nCntX < GRID_NUM_X; nCntX++)
				{
					// NULL�`�F�b�N
					if (m_apGrid[nCntY][nCntX] != NULL)
					{
						if (m_apGrid[nCntY][nCntX]->m_state != STATE_END)
						{
							m_apGrid[nCntY][nCntX]->SetState(STATE_END);
							m_bAllBreak = false;

						}
					}
				}
			}
		}
	}
	
}

void CGrid::BreakAll(D3DXVECTOR3 pos)
{
	if (!m_bAllBreak)
	{
		m_breakPos = pos;
		m_bAllBreak = true;
	}
}

//=============================
// ����������
//=============================
HRESULT CGrid::Init(void)
{
	CScene2d::Init();

	// �ϐ��̏�����
	// ���
	m_state = STATE_NORMAL;

	// �e�N�X�`��UV���W�̏�����
	m_nAnimX = rand()% MAX_ANIMATION_X;
	m_nAnimY = 0;

	// UV���W�̐ݒ�
	D3DXVECTOR2 uv[NUM_VERTEX];
	float fu = 1.0f / MAX_ANIMATION_X;
	float fv = 1.0f / MAX_ANIMATION_Y;

	uv[0] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY);
	uv[1] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY);
	uv[2] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY + fv);
	uv[3] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY + fv);

	// UV���W�Z�b�g
	SetTextureUV(uv);
	//�A�j���[�V�����t���O�̏�����
	m_bAnim = true;
	return S_OK;
}

//=============================
// �I������
//=============================
void CGrid::Uninit(void)
{
	//// NULL�`�F�b�N
	//if (m_pScene2d != NULL)
	//{
	//	// �I������
	//	m_pScene2d->Uninit();
	//}
	CScene2d::Uninit();
}


//=============================
// �X�V����
//=============================
void CGrid::Update(void)
{
	if (m_bAnim)
	{
		Animation();
	}
}


//=============================
// �`�揈��
//=============================
void CGrid::Draw(void)
{
	//if (m_pScene2d != NULL)
	//{
	//	m_pScene2d->Draw();
	//}
	CScene2d::Draw();
}

void CGrid::Animation(void)
{
	// �A�j���[�V�����J�E���g��i�߂�
	m_nCntAnim++;

	int nAnimeSpeed = ANIM_SPEED;
	if (m_state == STATE_END)
	{
		nAnimeSpeed *= 3;
	}
	if (m_nCntAnim % ANIM_SPEED == 0)
	{
		// �A�j���[�V����X���̉��Z
		m_nAnimX++;

		if (m_nAnimX >=MAX_ANIMATION_X)
		{
			// �A�j���[�V����X���̏�����
			m_nAnimX = 0;

			if (m_state == STATE_END)
			{
				m_bAnim = false;
				m_nAnimX = MAX_ANIMATION_X - 1;
				CDisplayOff::Create();
				
			}

			m_nAnimY++;
			if (m_nAnimY >= MAX_ANIMATION_Y)
			{
				m_nAnimY = 0;
			}
		}

		// UV���W�̐ݒ�
		D3DXVECTOR2 uv[NUM_VERTEX];
		float fu = 1.0f / MAX_ANIMATION_X;
		float fv = 1.0f / MAX_ANIMATION_Y;

		uv[0] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY);
		uv[1] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY);
		uv[2] = D3DXVECTOR2(fu*m_nAnimX, fv*m_nAnimY + fv);
		uv[3] = D3DXVECTOR2(fu*m_nAnimX + fu, fv*m_nAnimY + fv);

		// UV���W�Z�b�g
		SetTextureUV(uv);
	}
}

//=============================
// ��ԊǗ�
//=============================
void CGrid::SetState(STATE state)
{
	m_state = state;
	switch (state)
	{
	case STATE_NORMAL:
		
		break;

	case STATE_CRACK:
	{
		// ���W
		D3DXVECTOR3 pos = GetPos();
		// �T�C�Y
		D3DXVECTOR3 size = GetSize();

		// �Ђъ���|���S���̐���
		m_pScene2d = CScene2d::Create();                   // ����
		m_pScene2d->SetPos(pos);                           // �|�X
		m_pScene2d->SetSize(size);                         // �T�C�Y
		m_pScene2d->SetPriority(OBJTYPE_UI);               // �v���C�I���e�B
		m_pScene2d->BindTexture(m_apTexCrash[rand()%CRASH_TYPE_NUM]); // �e�N�X�`��

																	  // SE�Đ�
		CManager::GetSound()->Play(CSound::LABEL_SE_BREAK_01);
	}
		break;
	case STATE_BREAK:

		BindTexture(m_apTexture[STATE_BREAK]);

		// SE�Đ�
		CManager::GetSound()->Play(CSound::LABEL_SE_BREAK_02);
		break;
	case STATE_END:


		BindTexture(m_apTexture[STATE_END]);
		m_nAnimX = 0;
		m_nAnimY = 0;

		break;
	default:
		break;
	}
}

