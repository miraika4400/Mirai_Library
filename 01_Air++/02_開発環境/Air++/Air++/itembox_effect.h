//=============================================================================
//
// itembox_effectヘッダ [itembox_effect.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _ITEMBOX_EFFECT_H_
#define _ITEMBOX_EFFECT_H_

//*****************************
//インクルード
//*****************************
#include "main.h"
#include "scene3d.h"

//*****************************
//クラス定義
//*****************************

// ゴールUIクラス
class CItemBoxEffect : public CScene3d
{
public:
	//============
	// メンバ関数
	//============
	CItemBoxEffect();
	~CItemBoxEffect();

	static void SetEffect(D3DXVECTOR3 pos, float fCharaAngle); // クラス生成
	static CItemBoxEffect *Create(D3DXVECTOR3 pos, float fCharaAngle); // クラス生成
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
	float m_fCharaAngle; // 当たったキャラクターの向き
	D3DXVECTOR3 m_move;      // 移動力
	int m_nLife;             // 寿命
};

#endif