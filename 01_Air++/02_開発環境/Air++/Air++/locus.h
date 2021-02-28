//=============================================================================
//
// LOCUSヘッダ [LOCUS.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _LOCUS_H_
#define _LOCUS_H_

//*****************************
//インクルード
//*****************************
#include "main.h"
#include "scene.h"

//*****************************
//クラス定義
//*****************************

//2dポリゴンクラス
class CLocus : public CScene
{
public:
	// メンバ関数
	CLocus(int nPliority = OBJTYPE_NONE);
	~CLocus();
	static CLocus *Create(D3DXVECTOR3 aPos[NUM_VERTEX /2], D3DXVECTOR3 aPosOld[NUM_VERTEX / 2], D3DXCOLOR col);
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	LPDIRECT3DTEXTURE9 GetTexture(void) { return m_pTexture; }
	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }

	// 中心座標のセット、取得
	void SetPos(const D3DXVECTOR3 aPos[NUM_VERTEX / 2], const D3DXVECTOR3 aPosOld[NUM_VERTEX / 2]);

	// カラーのセット、取得
	void SetColor(const D3DXCOLOR col);
	D3DXCOLOR GetColor(void) { return m_col; }

	void SetAddMode(bool bBool) { m_bAddMode = bBool; }
private:
	// メンバ変数
	static LPDIRECT3DTEXTURE9		m_pTexture;	       // テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	       // 頂点バッファへのポインタ
	D3DXCOLOR m_col;                           // 色
	D3DXMATRIX  m_mtxWorld;                     // ワールドマトリックス
	bool m_bAddMode;                     // 加算合成か
	int m_nLife;                         // 寿命
};

#endif