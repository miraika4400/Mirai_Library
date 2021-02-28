//=============================================================================
//
// break_effectヘッダ [break_effect.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _BREAK_EFFECT_H_
#define _BREAK_EFFECT_H_

//*****************************
// インクルード
//*****************************
#include "main.h"
#include "scene.h"

//*****************************
// クラス定義
//*****************************

// 破壊演出クラス
class CBreakEffect : public CScene
{
public:

	//============
	// メンバ関数
	//============
	CBreakEffect();
	~CBreakEffect();

	// staic
	static CBreakEffect *Create(const D3DXVECTOR3 pos); // クラス生成

	HRESULT Init(void); // 初期化
	void Uninit(void);  // 終了
	void Update(void);  // 更新
	void Draw(void);    // 描画

private:
	//============
	// メンバ変数
	//============
	static LPDIRECT3DTEXTURE9 m_pTexture; // テクスチャ
	D3DXVECTOR3 m_size;                   // サイズ
	D3DXVECTOR3 m_pos;                    // 座標
	float m_fMaxsize;                     // 最大サイズ
	float m_fAlfa;                        // アルファ値

};

#endif