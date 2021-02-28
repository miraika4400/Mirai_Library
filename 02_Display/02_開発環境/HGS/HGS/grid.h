//=================================================
//
// grid�w�b�_[grid.h]
// Author: ���V ����
//
//=================================================

//��d�C���N���[�h�h�~
#ifndef _GRID_H_
#define _GRID_H_

//=============================
// �C���N���[�h
//=============================
#include "scene2d.h"
//=============================
// �O���錾
//=============================
class CScene2d;

//=============================
// �}�N����`
//=============================
#define GRID_NUM_Y 10                           // �O���b�h���E�c
#define GRID_NUM_X 10                           // �O���b�h���E��
#define GRID_GOAL D3DXVECTOR2(10, 8)
#define CRASH_TYPE_NUM 2
//=============================
// �N���X��`
//=============================

//�O���b�h�N���X
class CGrid : public CScene2d
{
public:
	//======================
	// �񋓐錾
	//======================
	typedef enum
	{
		STATE_NORMAL = 0, // �ʏ�
		STATE_CRACK,      // �q�r
		STATE_BREAK,      // �j��
		STATE_END,        // �j��
		STATE_MAX         // ��Ԑ�
	}STATE;

	//======================
	// �����o�֐�
	//======================
	CGrid();
	~CGrid();
	static void CreateAll(void);

	static HRESULT Load(void);
	static void Unload(void);
	static void Break(D3DXVECTOR3 pos);
	static void Shake(bool bRand = false);
	static void BreakAll(D3DXVECTOR3 pos);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// ���W�̃Q�b�g�E�Z�b�g
	D3DXVECTOR2 GetGridNumber(void) { return m_gridNum; }
	void SetGridNumber(D3DXVECTOR2 num) { m_gridNum = num; }

	static CGrid*GetGrid(int nIndexX,int nIndexY ) { return m_apGrid[nIndexY][nIndexX]; }
	
	//static CList<CGrid>*GetList(int nIndex) { return &m_list[nIndex]; }

private:
	void Animation(void);
	void SetState(STATE state);

	//======================
	// �����o�ϐ�
	//======================
	static LPDIRECT3DTEXTURE9 m_apTexture[STATE_MAX];      // �e�N�X�`��
	static LPDIRECT3DTEXTURE9 m_apTexCrash[CRASH_TYPE_NUM];      // �e�N�X�`��
	static CGrid *m_apGrid[GRID_NUM_Y][GRID_NUM_X];       // �O���b�h
	static D3DXVECTOR3 m_shake;                           // �u��
	static D3DXVECTOR3 m_shakeDist;                       // �u��
	static bool m_bShake;                                 // �u���t���O
	static int m_nCntShake;                               // �u���̃J�E���g
	static float m_fBreakArea;                            // BreakAll�ŏ����͈�
	static bool m_bAllBreak;                              // ���ׂĉ�
	static D3DXVECTOR3 m_breakPos;                        // ���ׂĉ󂷎n�_

	CScene2d *m_pScene2d;
	//CScene2d*m_pScene2d;   // �|���S��
	D3DXVECTOR2 m_gridNum; // �O���b�h�ԍ�
	int      m_nCntAnim;       // �A�j���[�V�����J�E���g
	int      m_nAnimX;         // �A�j���[�V����X��
	int      m_nAnimY;         // �A�j���[�V����Y��
	STATE m_state;           // ���
	bool m_bAnim;           // �A�j���[�V����
};


#endif