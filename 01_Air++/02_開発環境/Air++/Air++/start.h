//=============================================================================
//
// startヘッダ [start.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _START_H_
#define _START_H_

//*****************************
//インクルード
//*****************************
#include "main.h"
#include "scene3d.h"
#include "course.h"

//*****************************
//クラス定義
//*****************************

// スタートカウントダウンクラス
class CStart : public CScene3d
{
public:
	//***************
	// メンバ関数
	//***************
	CStart();
	~CStart();

	static CStart *Create(void); // クラス生成
	static HRESULT Load(void);   // ロード
	static void Unload(void);    // アンロード

	HRESULT Init(void); // 初期化
	void Uninit(void);  // 終了
	void Update(void);  // 更新
	void Draw(void);    // 描画

private:
	//***************
	// メンバ変数
	//***************
	static LPDIRECT3DTEXTURE9 m_pTexture;	 // テクスチャへのポインタ
	int m_nCntAnim;       // アニメーションカウント
	int m_nAnimX;         // アニメーションX軸
	int m_nAnimY;         // アニメーションY軸
	DWORD m_currentTime;  // 時間を計る用
	int m_nTime;          // テクスチャ切り替えタイム
};
#endif