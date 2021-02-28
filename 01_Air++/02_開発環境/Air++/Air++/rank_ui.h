//=============================================================================
//
// rank_uiヘッダ [rank_ui.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _RANK_UI_H_
#define _RANK_UI_H_

//*****************************
//インクルード
//*****************************
#include "main.h"
#include "scene2d.h"

//*****************************
//クラス定義
//*****************************

// ランクUIクラス
class CRankUi : public CScene2d
{
public:
	//============
	// 列挙
	//============

	// 使うテクスチャの種類の列挙
	typedef enum
	{
		RANK_UI_TEX_NORMAL,
		RANK_UI_TEX_RESULT,
		RANK_UI_TEX_MAX
	}RANK_UI_TEX_TYPE;

	//============
	// メンバ関数
	//============
	CRankUi();
	~CRankUi();

	static CRankUi *Create(int nPlayerNum); // クラス生成
	static HRESULT Load(void);            // テクスチャロード
	static void Unload(void);             // テクスチャアンロード

	HRESULT Init(void); // 初期化
	void Uninit(void);  // 終了
	void Update(void);  // 更新
	void Draw(void);    // 描画

private:
	//============
	// メンバ変数
	//============
	static LPDIRECT3DTEXTURE9 m_apTexture[RANK_UI_TEX_MAX];        // テクスチャへのポインタ

	D3DXVECTOR3 m_resultPos;       // リザルト時の座標
	D3DXVECTOR3 m_resultSize;      // リザルト時のサイズ

	int m_nCntAnim;                // アニメーションカウント
	int m_nAnimX;                  // アニメーションX軸
	int m_nAnimY;                  // アニメーションY軸
	int m_nPlayerNum;              // プレイヤー番号
};

#endif