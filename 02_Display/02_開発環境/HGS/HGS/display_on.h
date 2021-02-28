//=================================================
//
// displayonヘッダ[displayon.h]
// Author: 増澤 未来
//
//=================================================

//二重インクルード防止
#ifndef _DISPLAY_ON_H_
#define _DISPLAY_ON_H_

//=============================
// インクルード
//=============================
#include "scene2d.h"
//=============================
// 前方宣言
//=============================

//=============================
// マクロ定義
//=============================

//=============================
// クラス定義
//=============================

//グリッドクラス
class CDisplayON : public CScene2d
{
public:
	//======================
	// 列挙宣言
	//======================
	//======================

	// メンバ関数
	//======================
	CDisplayON();
	~CDisplayON();
	static CDisplayON* Create(void);

	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:


	//======================
	// メンバ変数
	//======================
	static LPDIRECT3DTEXTURE9 m_pTexture;
	int      m_nCntAnim;       // アニメーションカウント
	int      m_nAnimX;         // アニメーションX軸
	int      m_nAnimY;         // アニメーションY軸
};


#endif