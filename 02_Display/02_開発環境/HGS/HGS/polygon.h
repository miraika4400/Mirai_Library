//=============================================================================
//
// polygonヘッダ [polygon.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _POLYGON_H_
#define _POLYGON_H_

//*****************************
//インクルード
//*****************************
#include "main.h"

//*****************************
//クラス定義
//*****************************

//ポリゴンクラス
class CPolygon
{
public:

	//============
	// メンバ関数
	//============
	CPolygon();
	~CPolygon();
	static CPolygon *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));// クラス生成

	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXCOLOR col);  // 初期化
	void Uninit(void);                                                                 // 終了
	void Update(void);                                                                 // 更新
	void Draw(void);                                                                   // 描画

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }  // テクスチャの割り当て
	void SetVertexPos(D3DXVECTOR3 pos[NUM_VERTEX]);                           // 頂点座標の設定
	void SetColor(const D3DXCOLOR col);                                       // 色の設定
private:
	//============
	// メンバ変数
	//============
	LPDIRECT3DTEXTURE9        m_pTexture;  // テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9   m_pVtxBuff;  // 頂点バッファへのポイン
};

#endif