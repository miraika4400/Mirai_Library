//=============================================================================
//
// item_pointヘッダ [item_point.h]
// Author : 増澤 未来
//
//=============================================================================

//二重インクルード防止
#ifndef _ITEM_POINT_H_
#define _ITEM_POINT_H_

//*****************************
// インクルード
//*****************************
#include "main.h"
#include "scene.h"

//*****************************
// マクロ定義
//*****************************
#define MAX_ITEMPOINT_NUM 256 // ITEMポイント最大数

//*****************************
// 前方宣言
//*****************************


//*****************************
// クラス定義
//*****************************

// CPU用コースデータクラス
class CItemPoint : public CScene
{
public:
	//============
	// 状態列挙
	//============

	//============
	// メンバ関数
	//============
	// CPU用コースデータ構造体

	//============
	// メンバ関数
	//============
	CItemPoint();
	~CItemPoint();
	static CItemPoint *Create(void); // クラス生成

	HRESULT Init(void); // 初期化
	void Uninit(void);  // 終了
	void Update(void);  // 更新
	void Draw(void);    // 描画

						// アイテム数の取得
	int GetItemNum(void) { return m_nItemNum; }

	// アイテム座標の取得 *引数：配列番号
	D3DXVECTOR3 GetItemPos(int nIndex) { return m_aItemPos[nIndex]; }

private:
	void SaveText(void); // テキストファイルにポイントを書き込む
	void LoadText(void); // テキストファイルのポイントを読み込む

						 //============
						 // メンバ変数
						 //============
	D3DXVECTOR3 m_aItemPos[MAX_ITEMPOINT_NUM]; // アイテム座標
	int m_nItemNum;                            // アイテム数
};

#endif