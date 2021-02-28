//=================================================
//
// gridヘッダ[grid.h]
// Author: 増澤 未来
//
//=================================================

//二重インクルード防止
#ifndef _GRID_H_
#define _GRID_H_

//=============================
// インクルード
//=============================
#include "scene2d.h"
//=============================
// 前方宣言
//=============================
class CScene2d;

//=============================
// マクロ定義
//=============================
#define GRID_NUM_Y 10                           // グリッド数・縦
#define GRID_NUM_X 10                           // グリッド数・横
#define GRID_GOAL D3DXVECTOR2(10, 8)
#define CRASH_TYPE_NUM 2
//=============================
// クラス定義
//=============================

//グリッドクラス
class CGrid : public CScene2d
{
public:
	//======================
	// 列挙宣言
	//======================
	typedef enum
	{
		STATE_NORMAL = 0, // 通常
		STATE_CRACK,      // ヒビ
		STATE_BREAK,      // 破壊
		STATE_END,        // 破壊
		STATE_MAX         // 状態数
	}STATE;

	//======================
	// メンバ関数
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

	// 座標のゲット・セット
	D3DXVECTOR2 GetGridNumber(void) { return m_gridNum; }
	void SetGridNumber(D3DXVECTOR2 num) { m_gridNum = num; }

	static CGrid*GetGrid(int nIndexX,int nIndexY ) { return m_apGrid[nIndexY][nIndexX]; }
	
	//static CList<CGrid>*GetList(int nIndex) { return &m_list[nIndex]; }

private:
	void Animation(void);
	void SetState(STATE state);

	//======================
	// メンバ変数
	//======================
	static LPDIRECT3DTEXTURE9 m_apTexture[STATE_MAX];      // テクスチャ
	static LPDIRECT3DTEXTURE9 m_apTexCrash[CRASH_TYPE_NUM];      // テクスチャ
	static CGrid *m_apGrid[GRID_NUM_Y][GRID_NUM_X];       // グリッド
	static D3DXVECTOR3 m_shake;                           // ブレ
	static D3DXVECTOR3 m_shakeDist;                       // ブレ
	static bool m_bShake;                                 // ブレフラグ
	static int m_nCntShake;                               // ブレのカウント
	static float m_fBreakArea;                            // BreakAllで消す範囲
	static bool m_bAllBreak;                              // すべて壊す
	static D3DXVECTOR3 m_breakPos;                        // すべて壊す始点

	CScene2d *m_pScene2d;
	//CScene2d*m_pScene2d;   // ポリゴン
	D3DXVECTOR2 m_gridNum; // グリッド番号
	int      m_nCntAnim;       // アニメーションカウント
	int      m_nAnimX;         // アニメーションX軸
	int      m_nAnimY;         // アニメーションY軸
	STATE m_state;           // 状態
	bool m_bAnim;           // アニメーション
};


#endif