//=============================================================================
//
// lap_uiヘッダ [lap_ui.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _LAP_UI_H_
#define _LAP_UI_H_

//*****************************
// インクルード
//*****************************
#include "main.h"
#include "scene2d.h"

//*****************************
// 前方宣言
//*****************************

//*****************************
// マクロ定義
//*****************************

//*****************************
// クラス定義
//*****************************

// ラップ数UIロゴクラス
class CLapUi : public CScene2d
{
public:
	//============
	// メンバ関数
	//============
	CLapUi();
	~CLapUi();

	static CLapUi *Create(int nPlayerNum); // クラス生成
	static HRESULT Load(void);               // テクスチャロード
	static void Unload(void);                // テクスチャアンロード

	HRESULT Init(void); // 初期化
	void Uninit(void);  // 終了
	void Update(void);  // 更新
	void Draw(void);    // 描画

private:
	//============
	// メンバ変数
	//============
	static LPDIRECT3DTEXTURE9 m_pTexture;	 // テクスチャへのポインタ
	int m_nCntAnim;       // アニメーションカウント
	int m_nAnimX;         // アニメーションX軸
	int m_nAnimY;         // アニメーションY軸
	int m_nPlayerNum;
};

#endif