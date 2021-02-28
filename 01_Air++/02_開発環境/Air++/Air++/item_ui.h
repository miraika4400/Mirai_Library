#pragma once
//=============================================================================
//
// item_uiヘッダ [item_ui.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _ITEM_UI_H_
#define _ITEM_UI_H_

//*****************************
// インクルード
//*****************************
#include "main.h"
#include "scene.h"
#include "item.h"

//*****************************
// 前方宣言
//*****************************
class CPolygon;

//*****************************
// マクロ定義
//*****************************

//*****************************
// クラス定義
//*****************************

// アイテムUIロゴクラス
class CItemUi : public CScene
{
public:
	//============
	// メンバ関数
	//============
	CItemUi();
	~CItemUi();

	static CItemUi *Create(int nPlayerNum); // クラス生成
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
	static LPDIRECT3DTEXTURE9 m_pFrameTex;                  // テクスチャ アイテムUIフレーム
	static LPDIRECT3DTEXTURE9 m_apTexture[CItem::ITEM_MAX]; // テクスチャ アイテム本体
	static char *m_apItemPath[CItem::ITEM_MAX];             // アイテム本体テクスチャのパス
	int m_nPlayerNum;                                       // プレイヤー番号
	D3DXVECTOR3 m_pos;                                      // 中心座標
	CPolygon*m_pFramePolygon;                               // フレーム
	CPolygon*m_pItemPolygon;                                // アイテム
};

#endif