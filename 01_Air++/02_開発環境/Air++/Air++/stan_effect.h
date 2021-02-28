//=============================================================================
//
// stan_effectヘッダ [stan_effect.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _STAN_EFFECT_H_
#define _STAN_EFFECT_H_

//*****************************
//インクルード
//*****************************
#include "main.h"
#include "billboard.h"

//*****************************
//クラス定義
//*****************************

// ゴールUIクラス
class CStanEffect : public CBillboard
{
public:
	//============
	// メンバ関数
	//============
	CStanEffect();
	~CStanEffect();

	static void SetEffect(D3DXVECTOR3 pos); // クラス生成
	static CStanEffect *Create(D3DXVECTOR3 pos); // クラス生成
	static HRESULT Load(void);                      // テクスチャロード
	static void Unload(void);                       // テクスチャアンロード

	HRESULT Init(void); // 初期化
	void Uninit(void);  // 終了
	void Update(void);  // 更新
	void Draw(void);    // 描画

private:
	//============
	// メンバ変数
	//============
	static LPDIRECT3DTEXTURE9 m_pTexture; // テクスチャへのポインタ
	D3DXVECTOR3 m_move;      // 移動力
	int m_nLife;             // 寿命
};

#endif